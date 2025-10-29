#include "adm/private/xml_writer.hpp"
#include "adm/elements.hpp"
#include "adm/document.hpp"
#include "adm/private/rapidxml_formatter.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "adm/private/rapidxml_wrapper.hpp"

namespace adm {
  namespace xml {

    namespace {
      /**
       * Checks if the option @a flag is set within @a options.
       *
       * This is equivalent to an bitwise AND followed by a conversion to bool,
       * but should improve readability.
       */
      inline bool isSet(WriterOptions options, WriterOptions flag) {
        return static_cast<bool>(options & flag);
      }
      inline bool isSet(SadmWriterOptions options, SadmWriterOptions flag) {
        return static_cast<bool>(options & flag);
      }

      void add_document_to_node(
          XmlNode& audioFormatExtended,
          std::shared_ptr<Document const> document,
          TimeReference timeReference = TimeReference::TOTAL) {
        // clang-format off
        audioFormatExtended.addOptionalAttribute<Version>(document, "version");
        audioFormatExtended.addBaseElement<ProfileList>(document, "profileList", &formatProfileList);
        audioFormatExtended.addBaseElement<TagList>(document, "tagList", &formatTagList);
        audioFormatExtended.addBaseElements<AudioProgramme, AudioProgrammeId>(document, "audioProgramme", &formatAudioProgramme);
        audioFormatExtended.addBaseElements<AudioContent, AudioContentId>(document, "audioContent", &formatAudioContent);
        audioFormatExtended.addBaseElements<AudioObject, AudioObjectId>(document, "audioObject", &formatAudioObject);
        audioFormatExtended.addBaseElements<AudioPackFormat, AudioPackFormatId>(document, "audioPackFormat", &formatAudioPackFormat);
        audioFormatExtended.addBaseElements<AudioChannelFormat, AudioChannelFormatId>(document, "audioChannelFormat",
                                                                                      [timeReference](XmlNode& node, std::shared_ptr<const AudioChannelFormat> channelFormat){formatAudioChannelFormat(
                                                                                          node, std::move(channelFormat), timeReference);});
        audioFormatExtended.addBaseElements<AudioStreamFormat, AudioStreamFormatId>(document, "audioStreamFormat", &formatAudioStreamFormat);
        audioFormatExtended.addBaseElements<AudioTrackFormat, AudioTrackFormatId>(document, "audioTrackFormat", &formatAudioTrackFormat);

        for (auto &element : document->template getElements<AudioTrackUid>()) {
          auto id = element->template get<AudioTrackUidId>();
          if (!isCommonDefinitionsId(id) && !element->isSilent()) {
            audioFormatExtended.addElement(element, "audioTrackUID", &formatAudioTrackUid);
          }
        }
        // clang-format on
      }
    }  // namespace

    using NodePtr = rapidxml::xml_node<>*;

    XmlWriter::XmlWriter(WriterOptions options) : options_(options) {}

    std::ostream& XmlWriter::write(std::shared_ptr<const Document> document,
                                   std::ostream& stream) {
      XmlDocument xmlDocument;
      xmlDocument.setDiscardDefaults(
          !isSet(options_, WriterOptions::write_default_values));
      xmlDocument.addDeclaration();
      XmlNode root;
      if (isSet(options_, WriterOptions::itu_structure)) {
        root = xmlDocument.addItuStructure();
      } else {
        root = xmlDocument.addEbuStructure();
      }
      add_document_to_node(root, document);
      return stream << xmlDocument;
    }

    SadmXmlWriter::SadmXmlWriter(SadmWriterOptions options)
        : options_{options} {}
    std::ostream& SadmXmlWriter::write(std::shared_ptr<const Document> document,
                                       const FrameHeader& frameHeader,
                                       std::ostream& stream) {
      XmlDocument xmlDocument;
      xmlDocument.setDiscardDefaults(
          !isSet(options_, SadmWriterOptions::write_default_values));
      xmlDocument.addDeclaration();
      auto root = xmlDocument.addNode("frame");
      root.addAttribute("version", "ITU-R_BS.2125-1");
      root.addElement(frameHeader, "frameHeader", &formatFrameHeader);
      XmlNode formatExtended;
      if (isSet(options_, SadmWriterOptions::core_metadata)) {
        formatExtended = xmlDocument.addCoreMetadataAudioFormatExtended(root);
      } else {
        formatExtended = root.addNode("audioFormatExtended");
      }
      add_document_to_node(formatExtended, document,
                           frameHeader.get<FrameFormat>().get<TimeReference>());
      return stream << xmlDocument;
    }
  }  // namespace xml
}  // namespace adm
