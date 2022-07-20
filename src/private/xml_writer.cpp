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

    /// Check if a option/flag is set
    /**
     * Checks if the option @a flag is set within @a options.
     *
     * This is equivalent to an bitwise AND followed by a conversion to bool,
     * but should improve readability.
     */
    inline bool isSet(WriterOptions options, WriterOptions flag) {
      return static_cast<bool>(options & flag);
    }

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

      root.addBaseElements<AudioProgramme, AudioProgrammeId>(
          document, "audioProgramme", [this](XmlNode& node, auto programme) {
            formatAudioProgramme(node, programme);
          });

      // clang-format off
      root.addBaseElements<AudioContent, AudioContentId>(document, "audioContent", &formatAudioContent);
      root.addBaseElements<AudioObject, AudioObjectId>(document, "audioObject", &formatAudioObject);
      root.addBaseElements<AudioPackFormat, AudioPackFormatId>(document, "audioPackFormat", &formatAudioPackFormat);
      root.addBaseElements<AudioChannelFormat, AudioChannelFormatId>(document, "audioChannelFormat", &formatAudioChannelFormat);
      root.addBaseElements<AudioStreamFormat, AudioStreamFormatId>(document, "audioStreamFormat", &formatAudioStreamFormat);
      root.addBaseElements<AudioTrackFormat, AudioTrackFormatId>(document, "audioTrackFormat", &formatAudioTrackFormat);
      root.addBaseElements<AudioTrackUid, AudioTrackUidId>(document, "audioTrackUID", &formatAudioTrackUid);
      // clang-format on
      return stream << xmlDocument;
    }

    void XmlWriter::formatAudioProgramme(
        XmlNode& node, const std::shared_ptr<const AudioProgramme> programme) {
      // clang-format off
      node.addAttribute<AudioProgrammeId>(programme, "audioProgrammeID");
      node.addOptionalAttribute<AudioProgrammeName>(programme, "audioProgrammeName");
      node.addOptionalAttribute<AudioProgrammeLanguage>(programme, "audioProgrammeLanguage");
      node.addOptionalAttribute<Start>(programme, "start");
      node.addOptionalAttribute<End>(programme, "end");
      node.addOptionalAttribute<MaxDuckingDepth>(programme, "maxDuckingDepth");
      node.addReferences<AudioContent, AudioContentId>(programme, "audioContentIDRef");
      node.addVectorElements<LoudnessMetadatas>(programme, "loudnessMetadata", &formatLoudnessMetadata);
      node.addVectorElements<Labels>(programme, "audioProgrammeLabel", &formatLabel);
      // clang-format on
    }

  }  // namespace xml
}  // namespace adm
