#include "adm/combiner.hpp"
#include "adm/private/copy.hpp"

namespace adm {

  struct ElementIdMapping {
    // clang-format off
    std::map<std::shared_ptr<AudioProgramme>, std::vector<AudioContentId>> programmeContentRefs;
    std::map<std::shared_ptr<AudioContent>, std::vector<AudioObjectId>> contentObjectRefs;
    std::map<std::shared_ptr<AudioObject>, std::vector<AudioObjectId>> objectObjectRefs;
    std::map<std::shared_ptr<AudioObject>, std::vector<AudioPackFormatId>> objectPackFormatRefs;
    std::map<std::shared_ptr<AudioObject>, std::vector<AudioTrackUidId>> objectTrackUidRefs;
    std::map<std::shared_ptr<AudioTrackUid>, AudioTrackFormatId> trackUidTrackFormatRef;
    std::map<std::shared_ptr<AudioTrackUid>, AudioPackFormatId> trackUidPackFormatRef;
    std::map<std::shared_ptr<AudioPackFormat>, std::vector<AudioChannelFormatId>> packFormatChannelFormatRefs;
    std::map<std::shared_ptr<AudioPackFormat>, std::vector<AudioPackFormatId>> packFormatPackFormatRefs;
    std::map<std::shared_ptr<AudioTrackFormat>, AudioStreamFormatId> trackFormatStreamFormatRef;
    std::map<std::shared_ptr<AudioStreamFormat>, AudioChannelFormatId> streamFormatChannelFormatRef;
    std::map<std::shared_ptr<AudioStreamFormat>, AudioPackFormatId> streamFormatPackFormatRef;
    std::map<std::shared_ptr<AudioStreamFormat>, std::vector<AudioTrackFormatId>> streamFormatTrackFormatRefs;
    // clang-format on
  };

  Combiner::Combiner() : document_(Document::create()), firstFrame_(true) {}

  void Combiner::push(std::shared_ptr<const Frame> frame) {
    updateDocument(frame);
    updateTransportTrackFormats(frame);
  }

  void Combiner::updateDocument(std::shared_ptr<const Frame> frame) {
    // copy new elements
    ElementIdMapping mapping;

    for (auto element : frame->getElements<AudioProgramme>()) {
      if (!document_->lookup(element->get<AudioProgrammeId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        for (auto content : element->getReferences<AudioContent>()) {
          mapping.programmeContentRefs[copiedElement].push_back(
              content->get<AudioContentId>());
        }
      }
    }
    for (auto element : frame->getElements<AudioContent>()) {
      if (!document_->lookup(element->get<AudioContentId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        for (auto object : element->getReferences<AudioObject>()) {
          mapping.contentObjectRefs[copiedElement].push_back(
              object->get<AudioObjectId>());
        }
      }
    }
    for (auto element : frame->getElements<AudioObject>()) {
      if (!document_->lookup(element->get<AudioObjectId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        for (auto object : element->getReferences<AudioObject>()) {
          mapping.objectObjectRefs[copiedElement].push_back(
              object->get<AudioObjectId>());
        }
        for (auto packFormat : element->getReferences<AudioPackFormat>()) {
          mapping.objectPackFormatRefs[copiedElement].push_back(
              packFormat->get<AudioPackFormatId>());
        }
        for (auto trackUid : element->getReferences<AudioTrackUid>()) {
          mapping.objectTrackUidRefs[copiedElement].push_back(
              trackUid->get<AudioTrackUidId>());
        }
      }
    }
    for (auto element : frame->getElements<AudioPackFormat>()) {
      if (!document_->lookup(element->get<AudioPackFormatId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        for (auto content : element->getReferences<AudioChannelFormat>()) {
          mapping.packFormatChannelFormatRefs[copiedElement].push_back(
              content->get<AudioChannelFormatId>());
        }
        for (auto content : element->getReferences<AudioPackFormat>()) {
          mapping.packFormatPackFormatRefs[copiedElement].push_back(
              content->get<AudioPackFormatId>());
        }
      }
    }
    for (auto channelFormatFrame : frame->getElements<AudioChannelFormat>()) {
      auto channelFormatId = channelFormatFrame->get<AudioChannelFormatId>();
      auto channelFormatDoc = document_->lookup(channelFormatId);
      if (!channelFormatDoc) {
        channelFormatDoc = channelFormatFrame->copy();
        channelFormatDoc->clearAudioBlockFormats();
        document_->add(channelFormatDoc);
        lastBlockFormatIds_[channelFormatId] = AudioBlockFormatId();
      }
      addBlockFormats<AudioBlockFormatDirectSpeakers>(channelFormatFrame,
                                                      channelFormatDoc);
      addBlockFormats<AudioBlockFormatMatrix>(channelFormatFrame,
                                              channelFormatDoc);
      addBlockFormats<AudioBlockFormatObjects>(channelFormatFrame,
                                               channelFormatDoc);
      addBlockFormats<AudioBlockFormatHoa>(channelFormatFrame,
                                           channelFormatDoc);
      addBlockFormats<AudioBlockFormatBinaural>(channelFormatFrame,
                                                channelFormatDoc);
    }
    for (auto element : frame->getElements<AudioStreamFormat>()) {
      if (!document_->lookup(element->get<AudioStreamFormatId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        if (auto channelFormat = element->getReference<AudioChannelFormat>()) {
          mapping.streamFormatChannelFormatRef[copiedElement] =
              channelFormat->get<AudioChannelFormatId>();
        }
        if (auto packFormat = element->getReference<AudioPackFormat>()) {
          mapping.streamFormatPackFormatRef[copiedElement] =
              packFormat->get<AudioPackFormatId>();
        }
        for (auto trackFormat : element->getAudioTrackFormatReferences()) {
          mapping.streamFormatTrackFormatRefs[copiedElement].push_back(
              trackFormat.lock()->get<AudioTrackFormatId>());
        }
      }
    }
    for (auto element : frame->getElements<AudioTrackFormat>()) {
      if (!document_->lookup(element->get<AudioTrackFormatId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        if (auto streamFormat = element->getReference<AudioStreamFormat>()) {
          mapping.trackFormatStreamFormatRef[copiedElement] =
              streamFormat->get<AudioStreamFormatId>();
        }
      }
    }
    for (auto element : frame->getElements<AudioTrackUid>()) {
      if (!document_->lookup(element->get<AudioTrackUidId>())) {
        auto copiedElement = element->copy();
        document_->add(copiedElement);
        if (auto trackFormat = element->getReference<AudioTrackFormat>()) {
          mapping.trackUidTrackFormatRef[copiedElement] =
              trackFormat->get<AudioTrackFormatId>();
        }
        if (auto packFormat = element->getReference<AudioPackFormat>()) {
          mapping.trackUidPackFormatRef[copiedElement] =
              packFormat->get<AudioPackFormatId>();
        }
      }
    }
    // resolve new elements
    resolveReferences(mapping.programmeContentRefs);
    resolveReferences(mapping.contentObjectRefs);
    resolveReferences(mapping.objectObjectRefs);
    resolveReferences(mapping.objectPackFormatRefs);
    resolveReferences(mapping.objectTrackUidRefs);
    resolveReference(mapping.trackUidTrackFormatRef);
    resolveReference(mapping.trackUidPackFormatRef);
    resolveReferences(mapping.packFormatChannelFormatRefs);
    resolveReferences(mapping.packFormatPackFormatRefs);
    resolveReference(mapping.trackFormatStreamFormatRef);
    resolveReference(mapping.streamFormatChannelFormatRef);
    resolveReference(mapping.streamFormatPackFormatRef);
    resolveReferences(mapping.streamFormatTrackFormatRefs);
  }

  void Combiner::updateTransportTrackFormats(
      std::shared_ptr<const Frame> frame) {
    auto transportsFrame = frame->frameHeader().transportTrackFormats();
    if (transportsFrame.size() > 1) {
      throw std::runtime_error(
          "Combiner: too many transportTrackFormats (n > 1)");
    }
    if (transportsFrame.size() == 1) {
      auto transportTrackFormatFrame = *(transportsFrame.begin());
      if (firstFrame_ == true) {
        transportTrackFormat_ = transportTrackFormatFrame;
        firstFrame_ = false;
      } else {
        if (transportTrackFormat_.get<TransportId>() ==
            transportTrackFormatFrame.get<TransportId>()) {
          for (auto audioTrack : transportTrackFormatFrame.audioTracks()) {
            cumulateAudioTrack(audioTrack);
          }
        } else {
          throw std::runtime_error("transportId changed");
        }
      }
    }
  }

  /**
   * Comulates audioTracks and audioTrackUids
   *
   * Adds the audioTrack to the transportTrackFormat if no audioTrack with
   * the same id already exists. If it exists the AudioTrackUidIds, which
   * are not already in the list of AudioTrackUidIds, will be added.
   */
  void Combiner::cumulateAudioTrack(AudioTrack audioTrack) {
    auto audioTracks = transportTrackFormat_.audioTracks();
    auto foundAudioTrack =
        std::find_if(audioTracks.begin(), audioTracks.end(),
                     ParameterEqualTo<AudioTrack, TrackId>(audioTrack));
    if (foundAudioTrack == audioTracks.end()) {
      transportTrackFormat_.add(audioTrack);
    } else {
      auto audioTrackUidIds = (*foundAudioTrack).audioTrackUidIds();
      for (auto audioTrackUidId : audioTrack.audioTrackUidIds()) {
        auto foundAudioTrackUid = std::find(
            audioTrackUidIds.begin(), audioTrackUidIds.end(), audioTrackUidId);
        if (foundAudioTrackUid == audioTrackUidIds.end()) {
          (*foundAudioTrack).add(audioTrackUidId);
        }
      }
    }
  }

  std::shared_ptr<Document> Combiner::getDocument() { return document_; }

  TransportTrackFormat Combiner::getTransportTrackFormat() {
    return transportTrackFormat_;
  }

}  // namespace adm
