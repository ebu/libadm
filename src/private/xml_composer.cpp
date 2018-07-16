#include "adm/private/xml_composer.hpp"
#include "adm/elements.hpp"
#include "adm/document.hpp"
#include "adm/frame.hpp"
#include "adm/private/rapidxml_formatter.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

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

    XmlComposer::XmlComposer(WriterOptions options) : options_(options) {
      xmlDocument_.setDiscardDefaults(
          !isSet(options_, WriterOptions::write_default_values));
      xmlDocument_.addDeclaration();
    }

    void XmlComposer::compose(std::shared_ptr<const Document> admDocument) {
      XmlNode root;
      if (isSet(options_, WriterOptions::itu_structure)) {
        root = xmlDocument_.addItuStructure();
      } else {
        root = xmlDocument_.addEbuStructure();
      }
      // clang-format off
      root.addBaseElements<AudioProgramme, AudioProgrammeId>(admDocument, "audioProgramme", &formatAudioProgramme);
      root.addBaseElements<AudioContent, AudioContentId>(admDocument, "audioContent", &formatAudioContent);
      root.addBaseElements<AudioObject, AudioObjectId>(admDocument, "audioObject", &formatAudioObject);
      root.addBaseElements<AudioPackFormat, AudioPackFormatId>(admDocument, "audioPackFormat", &formatAudioPackFormat);
      root.addBaseElements<AudioChannelFormat, AudioChannelFormatId>(admDocument, "audioChannelFormat", &formatAudioChannelFormat);
      root.addBaseElements<AudioStreamFormat, AudioStreamFormatId>(admDocument, "audioStreamFormat", &formatAudioStreamFormat);
      root.addBaseElements<AudioTrackFormat, AudioTrackFormatId>(admDocument, "audioTrackFormat", &formatAudioTrackFormat);
      root.addBaseElements<AudioTrackUid, AudioTrackUidId>(admDocument, "audioTrackUID", &formatAudioTrackUid);
      // clang-format on
    }

    void XmlComposer::compose(std::shared_ptr<const Frame> frame) {
      auto root = xmlDocument_.addNode("frame");
      root.addElement(frame->frameHeader(), "frameHeader", &formatFrameHeader);
      auto formatExtended = root.addNode("audioFormatExtended");
      // clang-format off
      formatExtended.addBaseElements<AudioProgramme, AudioProgrammeId>(frame, "audioProgramme", &formatAudioProgramme);
      formatExtended.addBaseElements<AudioContent, AudioContentId>(frame, "audioContent", &formatAudioContent);
      formatExtended.addBaseElements<AudioObject, AudioObjectId>(frame, "audioObject", &formatAudioObject);
      formatExtended.addBaseElements<AudioPackFormat, AudioPackFormatId>(frame, "audioPackFormat", &formatAudioPackFormat);
      formatExtended.addBaseElements<AudioChannelFormat, AudioChannelFormatId>(frame, "audioChannelFormat", &formatAudioChannelFormat);
      formatExtended.addBaseElements<AudioStreamFormat, AudioStreamFormatId>(frame, "audioStreamFormat", &formatAudioStreamFormat);
      formatExtended.addBaseElements<AudioTrackFormat, AudioTrackFormatId>(frame, "audioTrackFormat", &formatAudioTrackFormat);
      formatExtended.addBaseElements<AudioTrackUid, AudioTrackUidId>(frame, "audioTrackUID", &formatAudioTrackUid);
      // clang-format on
    }

    std::ostream& XmlComposer::writeXmlTo(std::ostream& stream) {
      return stream << xmlDocument_;
    }

  }  // namespace xml
}  // namespace adm
