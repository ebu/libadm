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

  uint16_t getAvailableIdValue(std::shared_ptr<adm::Document> doc,
                               const adm::TypeDescriptor& td) {
    // We want to find a matching set of available ATF, ASF and ACF ID's
    AudioChannelFormatId acfId;
    AudioStreamFormatId asfId;
    AudioTrackFormatId atfId;

    acfId.set(td);
    asfId.set(td);
    atfId.set(td);
    atfId.set(AudioTrackFormatIdCounter{0x01u});

    for (uint16_t val = 0x1001u; val <= 0xFFFFu; val++) {
      acfId.set(AudioChannelFormatIdValue{val});
      asfId.set(AudioStreamFormatIdValue{val});
      atfId.set(AudioTrackFormatIdValue{val});
      if (!doc->lookup(acfId) && !doc->lookup(asfId) && !doc->lookup(atfId)) {
        return val;
      }
    }
    throw std::runtime_error("No more ID's available");
  }

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
    for (auto it = begin; it != end; ++it) {
      auto audioStreamFormat = *it;
      auto doc = audioStreamFormat->getParent().lock();
      if (!doc) {
        // If ASF doesn't belong to a doc, don't process this tree
        continue;
      }
      auto audioChannelFormat =
          audioStreamFormat->template getReference<AudioChannelFormat>();
      if (!audioChannelFormat) {
        // If no ACF, don't process this tree - it's redundant
        // This is similar to previous logic, since the TD component of ID's
        // was pulled from the ACF, so no ACF left an undef TD type in
        // ASF ID and all associated ATF ID's
        continue;
      }
      auto td = audioChannelFormat->template get<TypeDescriptor>();
      auto idValue = getAvailableIdValue(doc, td);

      // AudioStreamFormat
      auto audioStreamFormatId =
          audioStreamFormat->template get<AudioStreamFormatId>();
      if (!isCommonDefinitionsId(audioStreamFormatId)) {
        audioStreamFormatId.set(AudioStreamFormatIdValue{idValue});
        audioStreamFormat->set(audioStreamFormatId);
      }

      // AudioChannelFormat
      auto audioChannelFormatId =
          audioChannelFormat->template get<AudioChannelFormatId>();
      if (!isCommonDefinitionsId(audioChannelFormatId)) {
        audioChannelFormatId.set(AudioChannelFormatIdValue{idValue});
        audioChannelFormat->set(audioChannelFormatId);
        reassignAudioBlockFormatIds(audioChannelFormat);
      }

      // AudioTrackFormats
      AudioTrackFormatIdCounter audioTrackFormatIdCounter{0x01u};
      for (auto& weakAudioTrackFormat :
           audioStreamFormat->getAudioTrackFormatReferences()) {
        auto audioTrackFormat = weakAudioTrackFormat.lock();
        if (!audioTrackFormat) {
          continue;
        }
        auto audioTrackFormatId =
            audioTrackFormat->template get<AudioTrackFormatId>();
        if (!isCommonDefinitionsId(audioTrackFormatId)) {
          audioTrackFormatId.set(AudioTrackFormatIdValue{idValue});
          audioTrackFormatId.set(audioTrackFormatIdCounter);
          audioTrackFormat->set(audioTrackFormatId);
          audioTrackFormatIdCounter++;
        }
      }
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
      auto audioChannelFormat =
          audioTrackUid->getReference<adm::AudioChannelFormat>();
      if (audioChannelFormat) {
        auto audioChannelFormatId =
            audioChannelFormat->template get<AudioChannelFormatId>();
        if (!isCommonDefinitionsId(audioChannelFormatId)) {
          auto channelFormatType = audioChannelFormat->get<TypeDescriptor>();
          auto doc = audioChannelFormat->getParent().lock();
          if (doc) {
            auto idValue = getAvailableIdValue(doc, channelFormatType);
            audioChannelFormatId.set(channelFormatType);
            audioChannelFormatId.set(AudioChannelFormatIdValue{idValue});
            audioChannelFormat->set(audioChannelFormatId);
            reassignAudioBlockFormatIds(audioChannelFormat);
          }
        }
      }
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
     * algorithm only gives new IDs to audioTrackFormats and
     * audioChannelFormats which are referenced by an audioStreamFormat. 
     * Additionally, audioStreamFormats are only given a valid ID if they 
     * reference an audioChannelFormat. This should be the right way to do it 
     * for 2076-0/1 structures. Unreferenced elements will get an ID with 
     * value 0 and are thereby marked as elements which should be ignored.
     * For 2076-2 structures, reassignAudioTrackUidIds will apply a unique ID
     * to audioChannelFormats referenced directly from audioTrackUids.
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

}  // namespace adm
