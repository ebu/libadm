#include "adm/private/base_xml_parser.hpp"
#include "adm/private/xml_parser.hpp"
#include "adm/common_definitions.hpp"
#include "adm/private/xml_parser_helper.hpp"
#include "adm/detail/named_type_validators.hpp"
#include "adm/errors.hpp"
namespace adm {
  namespace xml {

    /// Check if a option/flag is set
    /**
     * Checks if the option @a flag is set within @a options.
     *
     * This is equivalent to an bitwise AND followed by a conversion to bool,
     * but should improve readability.
     */
    inline bool isSet(ParserOptions options, ParserOptions flag) {
      return static_cast<bool>(options & flag);
    }

    XmlParser::XmlParser(rapidxml::file<> file, ParserOptions options,
                         std::shared_ptr<Document> destDocument)
        : BaseXmlParser(file, options) {
          document_ = destDocument;
          idMap_ = *destDocument;
        }

    XmlParser::XmlParser(const std::string& filename, ParserOptions options,
                         std::shared_ptr<Document> destDocument)
        : XmlParser(rapidxml::file<>{filename.c_str()}, options,
                    std::move(destDocument)) {}

    XmlParser::XmlParser(std::istream& stream, ParserOptions options,
                         std::shared_ptr<Document> destDocument)
        : XmlParser(rapidxml::file<>{stream}, options,
                    std::move(destDocument)) {}



    template <typename Element>
    void XmlParser::add(std::shared_ptr<Element> el) {
      document_->add(el);
      idMap_.add(std::move(el));
    }

    std::shared_ptr<Document> XmlParser::parse() {
      rapidxml::xml_document<> xmlDocument;
      xmlDocument.parse<0>(xmlFile_.data());

      if (!xmlDocument.first_node())
        throw error::XmlParsingError("xml document is empty");

      NodePtr root = nullptr;
      if (isSet(options_, ParserOptions::recursive_node_search)) {
        root =
            findAudioFormatExtendedNodeFullRecursive(xmlDocument.first_node());
      } else {
        root = findAudioFormatExtendedNodeEbuCore(xmlDocument.first_node());
      }
      if (root) {
        setOptionalAttribute<Version>(root, "version", document_);

        // add ADM elements to ADM document
        for (NodePtr node = root->first_node(); node;
             node = node->next_sibling()) {
          std::string nodeName(node->name(), node->name_size());

          if (nodeName == "audioProgramme") {
            add(parseAudioProgramme(node));
          } else if (nodeName == "audioContent") {
            add(parseAudioContent(node));
          } else if (nodeName == "audioObject") {
            add(parseAudioObject(node));
          } else if (nodeName == "audioTrackUID") {
            add(parseAudioTrackUid(node));
          } else if (nodeName == "audioPackFormat") {
            add(parseAudioPackFormat(node));
          } else if (nodeName == "audioChannelFormat") {
            add(parseAudioChannelFormat(node));
          } else if (nodeName == "audioStreamFormat") {
            add(parseAudioStreamFormat(node));
          } else if (nodeName == "audioTrackFormat") {
            add(parseAudioTrackFormat(node));
          } else if (nodeName == "profileList") {
            if (document_->has<ProfileList>())
              throw error::XmlParsingError(
                  "found more than one profileList element");
            document_->set(parseProfileList(node));
          }
        }
        resolveReferences(programmeContentRefs_);
        resolveReferences(contentObjectRefs_);
        resolveReferences(objectObjectRefs_);
        // resolve complementary object references
        for (auto& entry : objectComplementaryObjectRefs_) {
          for (const auto& id : entry.second) {
            if (auto element = document_->lookup(id)) {
              entry.first->addComplementary(element);
            } else {
              throw error::XmlParsingUnresolvedReference(formatId(id));
            }
          }
        }
        resolveReferences(objectPackFormatRefs_);
        resolveTrackUidReferences(objectTrackUidRefs_, document_);
        resolveReference(trackUidTrackFormatRef_);
        resolveReference(trackUidChannelFormatRef_);
        resolveReference(trackUidPackFormatRef_);
        resolveReferences(packFormatChannelFormatRefs_);
        resolveReferences(packFormatPackFormatRefs_);
        resolveReference(trackFormatStreamFormatRef_);
        resolveReference(streamFormatChannelFormatRef_);
        resolveReference(streamFormatPackFormatRef_);
        resolveReferences(streamFormatTrackFormatRefs_);
      } else {
        throw error::XmlParsingError("audioFormatExtended node not found");
      }
      return document_;
    }  // namespace xml

  }  // namespace xml
}  // namespace adm
