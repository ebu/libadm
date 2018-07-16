#include "adm/utilities/id_assignment.hpp"

namespace adm {

  void reassignAudioProgrammeIds(std::shared_ptr<Document> document);
  void reassignAudioContentIds(std::shared_ptr<Document> document);
  void reassignAudioObjectIds(std::shared_ptr<Document> document);
  void reassignAudioPackFormatIds(std::shared_ptr<Document> document);
  void reassignAudioStreamFormatIds(std::shared_ptr<Document> document);

  void reassignAudioTrackUidIds(std::shared_ptr<Document> document);
  void reassignAudioBlockFormatIds(
      std::shared_ptr<AudioChannelFormat> channelFormat);

  template <typename It>
  void undefineIds(It begin, It end) {
    for (auto it = begin; it != end; ++it) {
      auto element = *it;
      using ElementId = typename decltype(element)::element_type::id_type;
      auto id = element->template get<ElementId>();
      if (!isCommonDefinitionsId(id)) {
        element->set(ElementId());
      }
    }
  }

  template <typename It>
  void reassignAudioProgrammeIds(It begin, It end) {
    auto audioProgrammeIdValue = AudioProgrammeIdValue(0x1001u);
    undefineIds(begin, end);
    for (auto it = begin; it != end; ++it) {
      auto audioProgramme = *it;
      auto audioProgrammeId = audioProgramme->template get<AudioProgrammeId>();
      if (!isCommonDefinitionsId(audioProgrammeId)) {
        audioProgrammeId.set(audioProgrammeIdValue);
        audioProgramme->set(audioProgrammeId);
        ++audioProgrammeIdValue;
      }
    }
  }

  template <typename It>
  void reassignAudioContentIds(It begin, It end) {
    undefineIds(begin, end);
    auto audioContentIdValue = AudioContentIdValue(0x1001u);
    for (auto it = begin; it != end; ++it) {
      auto audioContent = *it;
      auto audioContentId = audioContent->template get<AudioContentId>();
      if (!isCommonDefinitionsId(audioContentId)) {
        audioContentId.set(audioContentIdValue);
        audioContent->set(audioContentId);
        ++audioContentIdValue;
      }
    }
  }

  template <typename It>
  void reassignAudioObjectIds(It begin, It end) {
    undefineIds(begin, end);
    auto audioObjectIdValue = AudioObjectIdValue(0x1001u);
    for (auto it = begin; it != end; ++it) {
      auto audioObject = *it;
      auto audioObjectId = audioObject->template get<AudioObjectId>();
      if (!isCommonDefinitionsId(audioObjectId)) {
        audioObjectId.set(audioObjectIdValue);
        audioObject->set(audioObjectId);
        ++audioObjectIdValue;
      }
    }
  }

  template <typename It>
  void reassignAudioPackFormatIds(It begin, It end) {
    undefineIds(begin, end);
    auto audioPackFormatIdValue = AudioPackFormatIdValue(0x1001u);
    for (auto it = begin; it != end; ++it) {
      auto audioPackFormat = *it;
      auto typeDescriptor = audioPackFormat->template get<TypeDescriptor>();
      auto audioPackFormatId =
          audioPackFormat->template get<AudioPackFormatId>();
      if (!isCommonDefinitionsId(audioPackFormatId)) {
        audioPackFormat->set(
            AudioPackFormatId(typeDescriptor, audioPackFormatIdValue));
        ++audioPackFormatIdValue;
      }
    }
  }

  template <typename It>
  void reassignAudioStreamFormatIds(It begin, It end) {
    undefineIds(begin, end);
    auto audioStreamFormatIdValue = AudioStreamFormatIdValue(0x1001u);
    for (auto it = begin; it != end; ++it) {
      auto audioStreamFormat = *it;
      auto audioStreamFormatId =
          audioStreamFormat->template get<AudioStreamFormatId>();
      if (!isCommonDefinitionsId(audioStreamFormatId)) {
        audioStreamFormatId.set(audioStreamFormatIdValue);
        if (auto audioChannelFormat =
                audioStreamFormat
                    ->template getReference<AudioChannelFormat>()) {
          audioStreamFormatId.set(
              audioChannelFormat->template get<TypeDescriptor>());
        }
        audioStreamFormat->set(audioStreamFormatId);
      }
      auto audioTrackFormatIdCounter = AudioTrackFormatIdCounter(0x01u);
      for (auto& weakAudioTrackFormat :
           audioStreamFormat->getAudioTrackFormatReferences()) {
        auto audioTrackFormat = weakAudioTrackFormat.lock();
        if (!audioTrackFormat) {
          continue;
        }
        auto audioTrackFormatId =
            audioTrackFormat->template get<AudioTrackFormatId>();
        if (!isCommonDefinitionsId(audioTrackFormatId)) {
          auto trackFormatIdValue = AudioTrackFormatIdValue(
              audioStreamFormatId.template get<AudioStreamFormatIdValue>()
                  .get());
          audioTrackFormatId.set(trackFormatIdValue);
          audioTrackFormatId.set(
              audioStreamFormatId.template get<TypeDescriptor>());
          audioTrackFormatId.set(audioTrackFormatIdCounter);
          audioTrackFormat->set(audioTrackFormatId);
          ++audioTrackFormatIdCounter;
        }
      }
      if (auto audioChannelFormat =
              audioStreamFormat->template getReference<AudioChannelFormat>()) {
        auto audioChannelFormatId =
            audioChannelFormat->template get<AudioChannelFormatId>();
        if (!isCommonDefinitionsId(audioChannelFormatId)) {
          auto channelFormatIdValue = AudioChannelFormatIdValue(
              audioStreamFormatId.template get<AudioStreamFormatIdValue>()
                  .get());
          audioChannelFormatId.set(channelFormatIdValue);
          audioChannelFormatId.set(
              audioStreamFormatId.template get<TypeDescriptor>());
          audioChannelFormat->set(audioChannelFormatId);
          reassignAudioBlockFormatIds(audioChannelFormat);
        }
      }
      ++audioStreamFormatIdValue;
    }
  }

  template <typename It>
  void reassignAudioTrackUidIds(It begin, It end) {
    undefineIds(begin, end);
    auto audioTrackUidIdValue = AudioTrackUidIdValue(0x00000001u);
    for (auto it = begin; it != end; ++it) {
      auto audioTrackUid = *it;
      auto audioTrackUidId = audioTrackUid->template get<AudioTrackUidId>();
      audioTrackUidId.set(audioTrackUidIdValue);
      audioTrackUid->set(audioTrackUidId);
      ++audioTrackUidIdValue;
    }
  }

  void reassignAudioBlockFormatIds(
      std::shared_ptr<AudioChannelFormat> audioChannelFormat) {
    auto typeDefinition = audioChannelFormat->get<TypeDescriptor>();
    auto audioChannelFormatIdValue =
        audioChannelFormat->get<AudioChannelFormatId>()
            .get<AudioChannelFormatIdValue>();
    auto audioBlockFormatIdValue =
        AudioBlockFormatIdValue(audioChannelFormatIdValue.get());
    auto audioBlockFormatIdCounter = AudioBlockFormatIdCounter(0x00000001u);
    if (typeDefinition == TypeDefinition::DIRECT_SPEAKERS) {
      auto audioBlockFormats =
          audioChannelFormat->getElements<AudioBlockFormatDirectSpeakers>();
      for (auto& audioBlockFormat : audioBlockFormats) {
        auto audioBlockFormatId = audioBlockFormat.get<AudioBlockFormatId>();
        audioBlockFormatId.set(typeDefinition);
        audioBlockFormatId.set(audioBlockFormatIdValue);
        audioBlockFormatId.set(audioBlockFormatIdCounter);
        audioBlockFormat.set(audioBlockFormatId);
        ++audioBlockFormatIdCounter;
      }
    } else if (typeDefinition == TypeDefinition::MATRIX) {
      auto audioBlockFormats =
          audioChannelFormat->getElements<AudioBlockFormatMatrix>();
      for (auto& audioBlockFormat : audioBlockFormats) {
        auto audioBlockFormatId = audioBlockFormat.get<AudioBlockFormatId>();
        audioBlockFormatId.set(typeDefinition);
        audioBlockFormatId.set(audioBlockFormatIdValue);
        audioBlockFormatId.set(audioBlockFormatIdCounter);
        audioBlockFormat.set(audioBlockFormatId);
        ++audioBlockFormatIdCounter;
      }
    } else if (typeDefinition == TypeDefinition::OBJECTS) {
      auto audioBlockFormats =
          audioChannelFormat->getElements<AudioBlockFormatObjects>();
      for (auto& audioBlockFormat : audioBlockFormats) {
        auto audioBlockFormatId = audioBlockFormat.get<AudioBlockFormatId>();
        audioBlockFormatId.set(typeDefinition);
        audioBlockFormatId.set(audioBlockFormatIdValue);
        audioBlockFormatId.set(audioBlockFormatIdCounter);
        audioBlockFormat.set(audioBlockFormatId);
        ++audioBlockFormatIdCounter;
      }
    } else if (typeDefinition == TypeDefinition::HOA) {
      auto audioBlockFormats =
          audioChannelFormat->getElements<AudioBlockFormatHoa>();
      for (auto& audioBlockFormat : audioBlockFormats) {
        auto audioBlockFormatId = audioBlockFormat.get<AudioBlockFormatId>();
        audioBlockFormatId.set(typeDefinition);
        audioBlockFormatId.set(audioBlockFormatIdValue);
        audioBlockFormatId.set(audioBlockFormatIdCounter);
        audioBlockFormat.set(audioBlockFormatId);
        ++audioBlockFormatIdCounter;
      }
    } else if (typeDefinition == TypeDefinition::BINAURAL) {
      auto audioBlockFormats =
          audioChannelFormat->getElements<AudioBlockFormatBinaural>();
      for (auto& audioBlockFormat : audioBlockFormats) {
        auto audioBlockFormatId = audioBlockFormat.get<AudioBlockFormatId>();
        audioBlockFormatId.set(typeDefinition);
        audioBlockFormatId.set(audioBlockFormatIdValue);
        audioBlockFormatId.set(audioBlockFormatIdCounter);
        audioBlockFormat.set(audioBlockFormatId);
        ++audioBlockFormatIdCounter;
      }
    }
  }

  template <typename DocumentType>
  void reassignAllIds(DocumentType document) {
    reassignAudioProgrammeIds(
        document->template getElements<AudioProgramme>().begin(),
        document->template getElements<AudioProgramme>().end());
    reassignAudioContentIds(
        document->template getElements<AudioContent>().begin(),
        document->template getElements<AudioContent>().end());
    reassignAudioObjectIds(
        document->template getElements<AudioObject>().begin(),
        document->template getElements<AudioObject>().end());
    reassignAudioPackFormatIds(
        document->template getElements<AudioPackFormat>().begin(),
        document->template getElements<AudioPackFormat>().end());
    /**
     * Initialize all audioTrackFormatIds and audioChanneldFormatIds to zero.
     * The reason behind this is, that the reassignAudioStreamFormatIds
     * algorithm only gives new Ids to audioTrackFormats and
     * audioChannelFormats which are referenced by a audioStreamFormat. This
     * should be the right way to do it. Unreferenced elements will get an ID
     * with value 0 and are thereby marked as elements which should be
     * ignored.
     */
    undefineIds(document->template getElements<AudioTrackFormat>().begin(),
                document->template getElements<AudioTrackFormat>().end());
    undefineIds(document->template getElements<AudioChannelFormat>().begin(),
                document->template getElements<AudioChannelFormat>().end());
    reassignAudioStreamFormatIds(
        document->template getElements<AudioStreamFormat>().begin(),
        document->template getElements<AudioStreamFormat>().end());
    reassignAudioTrackUidIds(
        document->template getElements<AudioTrackUid>().begin(),
        document->template getElements<AudioTrackUid>().end());
  }

  void reassignIds(std::shared_ptr<Document> document) {
    reassignAllIds(document);
  }

  void reassignIds(std::shared_ptr<Frame> frame) { reassignAllIds(frame); }

}  // namespace adm
