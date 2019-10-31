#include "adm/private/rapidxml_formatter.hpp"
#include "adm/private/rapidxml_wrapper.hpp"

namespace adm {
  namespace xml {

    namespace detail {

      std::string toString(const std::string &string) { return string; }
      std::string toString(const std::chrono::nanoseconds &time) {
        return formatTimecode(time);
      }
      std::string toString(const AudioProgrammeId &id) { return formatId(id); }
      std::string toString(const AudioContentId &id) { return formatId(id); }
      std::string toString(const AudioObjectId &id) { return formatId(id); }
      std::string toString(const AudioPackFormatId &id) { return formatId(id); }
      std::string toString(const AudioChannelFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioBlockFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioStreamFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioTrackFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioTrackUidId &id) { return formatId(id); }

      struct MultiElementAttributeFormatter {
        MultiElementAttributeFormatter(const std::string &a,
                                       const std::string &v)
            : attribute(a), value(v) {}
        template <typename Element>
        void operator()(XmlNode &node, Element element) {
          node.addAttribute(attribute, value);
          node.setValue(element.get());
        }
        std::string attribute;
        std::string value;
      };

      MultiElementAttributeFormatter formatMultiElementAttribute(
          const std::string &attribute, const std::string &value) {
        return MultiElementAttributeFormatter(attribute, value);
      }

    }  // namespace detail

    void formatAudioProgramme(
        XmlNode &node, const std::shared_ptr<const AudioProgramme> programme) {
      // clang-format off
      node.addAttribute<AudioProgrammeId>(programme, "audioProgrammeID");
      node.addOptionalAttribute<AudioProgrammeName>(programme, "audioProgrammeName");
      node.addOptionalAttribute<AudioProgrammeLanguage>(programme, "audioProgrammeLanguage");
      node.addOptionalAttribute<Start>(programme, "start");
      node.addOptionalAttribute<End>(programme, "end");
      node.addOptionalAttribute<MaxDuckingDepth>(programme, "maxDuckingDepth");
      node.addReferences<AudioContent, AudioContentId>(programme, "audioContentIDRef");
      node.addOptionalElement<LoudnessMetadata>(programme, "loudnessMetadata", &formatLoudnessMetadata);
      // clang-format on
    }

    void formatLoudnessMetadata(XmlNode &node,
                                const LoudnessMetadata loudnessMetadata) {
      node.addOptionalAttribute<LoudnessMethod>(&loudnessMetadata,
                                                "loudnessMethod");
      node.addOptionalAttribute<LoudnessRecType>(&loudnessMetadata,
                                                 "loudnessRecType");
      node.addOptionalAttribute<LoudnessCorrectionType>(
          &loudnessMetadata, "loudnessCorrectionType");
      node.addOptionalElement<IntegratedLoudness>(&loudnessMetadata,
                                                  "integratedLoudness");
      node.addOptionalElement<LoudnessRange>(&loudnessMetadata,
                                             "loudnessRange");
      node.addOptionalElement<MaxTruePeak>(&loudnessMetadata, "maxTruePeak");
      node.addOptionalElement<MaxMomentary>(&loudnessMetadata, "maxMomentary");
      node.addOptionalElement<MaxShortTerm>(&loudnessMetadata, "maxShortTerm");
      node.addOptionalElement<DialogueLoudness>(&loudnessMetadata,
                                                "dialogueLoudness");
    }

    void formatAudioContent(XmlNode &node,
                            std::shared_ptr<const AudioContent> content) {
      // clang-format off
      node.addAttribute<AudioContentId>(content, "audioContentID");
      node.addOptionalAttribute<AudioContentName>(content, "audioContentName");
      node.addOptionalAttribute<AudioContentLanguage>(content, "audioContentLanguage");
      node.addReferences<AudioObject, AudioObjectId>(content, "audioObjectIDRef");
      node.addOptionalElement<LoudnessMetadata>(content, "loudnessMetadata", &formatLoudnessMetadata);
      node.addOptionalElement<NonDialogueContentKind>(content, "dialogue", &formatNonDialogueContentKind);
      node.addOptionalElement<DialogueContentKind>(content, "dialogue", &formatDialogueContentKind);
      node.addOptionalElement<MixedContentKind>(content, "dialogue", &formatMixedContentKind);
      // clang-format on
    }

    void formatNonDialogueContentKind(
        XmlNode &node, const NonDialogueContentKind contentKind) {
      // clang-format off
      node.addAttribute("nonDialogueContentKind", std::to_string(contentKind.get()));
      node.setValue(std::string("0"));
      // clang-format on
    }

    void formatDialogueContentKind(XmlNode &node,
                                   const DialogueContentKind contentKind) {
      // clang-format off
      node.addAttribute("dialogueContentKind", std::to_string(contentKind.get()));
      node.setValue(std::string("1"));
      // clang-format on
    }

    void formatMixedContentKind(XmlNode &node,
                                const MixedContentKind contentKind) {
      // clang-format off
      node.addAttribute("mixedContentKind", std::to_string(contentKind.get()));
      node.setValue(std::string("2"));
      // clang-format on
    }

    void formatAudioObject(XmlNode &node,
                           std::shared_ptr<const AudioObject> object) {
      // clang-format off
      node.addAttribute<AudioObjectId>(object, "audioObjectID");
      node.addOptionalAttribute<AudioObjectName>(object, "audioObjectName");
      node.addOptionalAttribute<Start>(object, "start");
      node.addOptionalAttribute<Duration>(object, "duration");
      node.addOptionalAttribute<Importance>(object, "importance");
      node.addOptionalAttribute<Interact>(object, "interact");
      node.addOptionalAttribute<DisableDucking>(object, "disableDucking");
      node.addOptionalElement<AudioObjectInteraction>(object, "audioObjectInteraction", &formatAudioObjectInteraction);
      node.addReferences<AudioPackFormat, AudioPackFormatId>(object, "audioPackFormatIDRef");
      node.addReferences<AudioObject, AudioObjectId>(object, "audioObjectIDRef");
      for (auto &element : object->getComplementaryObjects()) {
        node.addElement<AudioObjectId>(element, "audioComplementaryObjectIDRef");
      }
      node.addReferences<AudioTrackUid, AudioTrackUidId>(object, "audioTrackUIDRef");
      // clang-format on
    }

    void formatAudioObjectInteraction(
        XmlNode &node, const AudioObjectInteraction &objectInteraction) {
      // clang-format off
      node.addAttribute<OnOffInteract>(&objectInteraction, "onOffInteract");
      node.addOptionalAttribute<GainInteract>(&objectInteraction, "gainInteract");
      node.addOptionalAttribute<PositionInteract>(&objectInteraction, "positionInteract");
      node.addOptionalMultiElement<GainInteractionRange>(&objectInteraction, "gainInteractionRange", &formatGainInteractionRange);
      node.addOptionalMultiElement<PositionInteractionRange>(&objectInteraction, "positionInteractionRange", &formatPositionInteractionRange);
      // clang-format on
    }

    void formatGainInteractionRange(
        XmlNode &parentNode, const std::string &name,
        const GainInteractionRange &gainInteraction) {
      if (gainInteraction.has<GainInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("bound", "min");
        minNode.setValue(gainInteraction.get<GainInteractionMin>());
      }
      if (gainInteraction.has<GainInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(gainInteraction.get<GainInteractionMax>());
      }
    }

    void formatPositionInteractionRange(
        XmlNode &parentNode, const std::string &name,
        const PositionInteractionRange &positionInteraction) {
      if (positionInteraction.has<AzimuthInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "azimuth");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<AzimuthInteractionMin>());
      }
      if (positionInteraction.has<ElevationInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "elevation");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<ElevationInteractionMin>());
      }
      if (positionInteraction.has<DistanceInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "distance");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<DistanceInteractionMin>());
      }
      if (positionInteraction.has<AzimuthInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "azimuth");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<AzimuthInteractionMax>());
      }
      if (positionInteraction.has<ElevationInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "elevation");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<ElevationInteractionMax>());
      }
      if (positionInteraction.has<DistanceInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "distance");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<DistanceInteractionMax>());
      }
      if (positionInteraction.has<XInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "X");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<XInteractionMin>());
      }
      if (positionInteraction.has<YInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "Y");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<YInteractionMin>());
      }
      if (positionInteraction.has<ZInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "Z");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<ZInteractionMin>());
      }
      if (positionInteraction.has<XInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "X");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<XInteractionMax>());
      }
      if (positionInteraction.has<YInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "Y");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<YInteractionMax>());
      }
      if (positionInteraction.has<ZInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "Z");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<ZInteractionMax>());
      }
    }

    void formatAudioPackFormat(
        XmlNode &node, std::shared_ptr<const AudioPackFormat> packFormat) {
      // clang-format off
      node.addAttribute<AudioPackFormatId>(packFormat, "audioPackFormatID");
      node.addOptionalAttribute<AudioPackFormatName>(packFormat, "audioPackFormatName");
      node.addOptionalAttribute<TypeDescriptor>(packFormat, "typeLabel", &formatTypeLabel);
      node.addOptionalAttribute<TypeDescriptor>(packFormat, "typeDefinition", &formatTypeDefinition);
      node.addOptionalAttribute<Importance>(packFormat, "importance");
      node.addReferences<AudioChannelFormat, AudioChannelFormatId>(packFormat, "audioChannelFormatIDRef");
      node.addReferences<AudioPackFormat, AudioPackFormatId>(packFormat, "audioPackFormatIDRef");
      // clang-format on
    }

    void formatAudioChannelFormat(
        XmlNode &node,
        std::shared_ptr<const AudioChannelFormat> channelFormat) {
      // clang-format off
      node.addAttribute<AudioChannelFormatId>(channelFormat, "audioChannelFormatID");
      node.addOptionalAttribute<AudioChannelFormatName>(channelFormat, "audioChannelFormatName");
      node.addOptionalAttribute<TypeDescriptor>(channelFormat, "typeLabel", &formatTypeLabel);
      node.addOptionalAttribute<TypeDescriptor>(channelFormat, "typeDefinition", &formatTypeDefinition);
      node.addOptionalMultiElement<Frequency>(channelFormat, "frequency", &formatFrequency);

      auto channelType = channelFormat->get<TypeDescriptor>();
      if (channelType == TypeDefinition::DIRECT_SPEAKERS) {
        node.addElements<AudioBlockFormatDirectSpeakers>(channelFormat, "audioBlockFormat", &formatBlockFormatDirectSpeakers);
      } else if (channelType == TypeDefinition::MATRIX) {
        node.addElements<AudioBlockFormatMatrix>(channelFormat, "audioBlockFormat", &formatBlockFormatMatrix);
      } else if (channelType == TypeDefinition::OBJECTS) {
        node.addElements<AudioBlockFormatObjects>(channelFormat, "audioBlockFormat", &formatBlockFormatObjects);
      } else if (channelType == TypeDefinition::HOA) {
        node.addElements<AudioBlockFormatHoa>(channelFormat, "audioBlockFormat", &formatBlockFormatHoa);
      } else if (channelType == TypeDefinition::BINAURAL) {
        node.addElements<AudioBlockFormatBinaural>(channelFormat, "audioBlockFormat", &formatBlockFormatBinaural);
      }
      // clang-format on
    }

    void formatBlockFormatDirectSpeakers(
        XmlNode &node, const AudioBlockFormatDirectSpeakers &audioBlock) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      node.addOptionalAttribute<Rtime>(&audioBlock, "rtime");
      node.addOptionalAttribute<Duration>(&audioBlock, "duration");
      node.addMultiElement<SpeakerLabels>(&audioBlock, "speakerLabel", &formatSpeakerLabels);
      node.addMultiElement<SpeakerPosition>(&audioBlock, "position", &formatSpeakerPosition);
      // clang-format on
    }

    void formatSpeakerLabels(XmlNode &parentNode, const std::string &name,
                             const SpeakerLabels &speakerLabels) {
      for (const auto &speakerLabel : speakerLabels) {
        auto speakerLabelNode = parentNode.addNode(name);
        speakerLabelNode.setValue(speakerLabel);
      }
    }

    void formatSpeakerPosition(XmlNode &parentNode, const std::string &name,
                               const SpeakerPosition &position) {
      auto azimuthNode = parentNode.addNode(name);
      azimuthNode.addAttribute("coordinate", "azimuth");
      azimuthNode.addOptionalAttribute<AzimuthMin>(&position, "min");
      azimuthNode.addOptionalAttribute<AzimuthMax>(&position, "max");
      if (position.has<ScreenEdgeLock>()) {
        auto screenEdgeLock = position.get<ScreenEdgeLock>();
        azimuthNode.addOptionalAttribute<HorizontalEdge>(&screenEdgeLock,
                                                         "screenEdgeLock");
      }
      azimuthNode.setValue(position.get<Azimuth>());

      auto elevationNode = parentNode.addNode(name);
      elevationNode.addAttribute("coordinate", "elevation");
      elevationNode.addOptionalAttribute<ElevationMin>(&position, "min");
      elevationNode.addOptionalAttribute<ElevationMax>(&position, "max");
      if (position.has<ScreenEdgeLock>()) {
        auto screenEdgeLock = position.get<ScreenEdgeLock>();
        elevationNode.addOptionalAttribute<VerticalEdge>(&screenEdgeLock,
                                                         "screenEdgeLock");
      }
      elevationNode.setValue(position.get<Elevation>());
      if (!position.isDefault<Distance>()) {
        auto distanceNode = parentNode.addNode(name);
        distanceNode.addAttribute("coordinate", "distance");
        distanceNode.addOptionalAttribute<DistanceMin>(&position, "min");
        distanceNode.addOptionalAttribute<DistanceMax>(&position, "max");
        distanceNode.setValue(position.get<Distance>());
      }
    }

    void formatFrequency(XmlNode &parentNode, const std::string &name,
                         const Frequency &frequency) {
      if (frequency.has<LowPass>()) {
        auto lowPassNode = parentNode.addNode(name);
        lowPassNode.addAttribute("typeDefinition", "lowPass");
        lowPassNode.setValue(frequency.get<LowPass>());
      }
      if (frequency.has<HighPass>()) {
        auto highPassNode = parentNode.addNode(name);
        highPassNode.addAttribute("typeDefinition", "highPass");
        highPassNode.setValue(frequency.get<HighPass>());
      }
    }

    void formatBlockFormatMatrix(XmlNode &node,
                                 const AudioBlockFormatMatrix &audioBlock) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      node.addOptionalAttribute<Rtime>(&audioBlock, "rtime");
      node.addOptionalAttribute<Duration>(&audioBlock, "duration");
      // TODO: add missing matrix attributes and elements
      // clang-format on
    }

    void formatBlockFormatObjects(XmlNode &node,
                                  const AudioBlockFormatObjects &audioBlock) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      node.addOptionalAttribute<Rtime>(&audioBlock, "rtime");
      node.addOptionalAttribute<Duration>(&audioBlock, "duration");
      node.addMultiElement<Position>(&audioBlock, "position", &formatPosition);
      node.addOptionalElement<Width>(&audioBlock, "width");
      node.addOptionalElement<Height>(&audioBlock, "height");
      node.addOptionalElement<Depth>(&audioBlock, "depth");
      node.addOptionalElement<Cartesian>(&audioBlock, "cartesian");
      node.addOptionalElement<Gain>(&audioBlock, "gain");
      node.addOptionalElement<Diffuse>(&audioBlock, "diffuse");
      node.addOptionalElement<ChannelLock>(&audioBlock, "channelLock", &formatChannelLock);
      node.addOptionalElement<ObjectDivergence>(&audioBlock, "objectDivergence", &formatObjectDivergence);
      node.addOptionalElement<JumpPosition>(&audioBlock, "jumpPosition", &formatJumpPosition);
      node.addOptionalElement<ScreenRef>(&audioBlock, "screenRef");
      // clang-format on
      // TODO: add zoneExclusion
      node.addOptionalElement<Importance>(&audioBlock, "importance");
    }

    void formatPosition(XmlNode &parentNode, const std::string &name,
                        const Position &position) {
      if (isSpherical(position)) {
        auto sphericalPosition = boost::get<SphericalPosition>(position);
        parentNode.addElement(
            sphericalPosition.get<Azimuth>(), name,
            detail::formatMultiElementAttribute("coordinate", "azimuth"));
        parentNode.addElement(
            sphericalPosition.get<Elevation>(), name,
            detail::formatMultiElementAttribute("coordinate", "elevation"));
        parentNode.addOptionalElement<Distance>(
            &sphericalPosition, name,
            detail::formatMultiElementAttribute("coordinate", "distance"));
      } else {
        auto cartesianPosition = boost::get<CartesianPosition>(position);
        parentNode.addElement(
            cartesianPosition.get<X>(), name,
            detail::formatMultiElementAttribute("coordinate", "X"));
        parentNode.addElement(
            cartesianPosition.get<Y>(), name,
            detail::formatMultiElementAttribute("coordinate", "Y"));
        parentNode.addOptionalElement<Z>(
            &cartesianPosition, name,
            detail::formatMultiElementAttribute("coordinate", "Z"));
      }
    }

    void formatChannelLock(XmlNode &node, const ChannelLock &channelLock) {
      node.addOptionalAttribute<MaxDistance>(&channelLock, "maxDistance");
      node.setValue(channelLock.get<ChannelLockFlag>());
    }

    void formatObjectDivergence(XmlNode &node,
                                const ObjectDivergence &objectDivergence) {
      // clang-format off
      node.addOptionalAttribute<AzimuthRange>(&objectDivergence, "azimuthRange");
      node.addOptionalAttribute<PositionRange>(&objectDivergence, "positionRange");
      node.setValue(objectDivergence.get<Divergence>());
      // clang-format on
    }

    void formatJumpPosition(XmlNode &node, const JumpPosition &jumpPosition) {
      // clang-format off
      node.addOptionalAttribute<InterpolationLength>(&jumpPosition, "interpolationLength", &formatInterpolationLength);
      node.setValue(jumpPosition.get<JumpPositionFlag>());
      // clang-format on
    }

    void formatBlockFormatHoa(XmlNode &node,
                              const AudioBlockFormatHoa &audioBlock) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      node.addOptionalAttribute<Rtime>(&audioBlock, "rtime");
      node.addOptionalAttribute<Duration>(&audioBlock, "duration");
      node.addOptionalElement<Order>(&audioBlock, "order");
      node.addOptionalElement<Degree>(&audioBlock, "degree");
      node.addOptionalElement<NfcRefDist>(&audioBlock, "nfcRefDist");
      node.addOptionalElement<ScreenRef>(&audioBlock, "screenRef");
      node.addOptionalElement<Normalization>(&audioBlock, "normalizaiton");
      // TODO: add Equation
      // clang-format on
    }

    void formatBlockFormatBinaural(XmlNode &node,
                                   const AudioBlockFormatBinaural &audioBlock) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      node.addOptionalAttribute<Rtime>(&audioBlock, "rtime");
      node.addOptionalAttribute<Duration>(&audioBlock, "duration");
      // TODO: add missing binaural attributes and elements
      // clang-format on
    }

    void formatAudioStreamFormat(
        XmlNode &node, std::shared_ptr<const AudioStreamFormat> streamFormat) {
      // clang-format off
      node.addAttribute<AudioStreamFormatId>(streamFormat, "audioStreamFormatID");
      node.addOptionalAttribute<AudioStreamFormatName>(streamFormat, "audioStreamFormatName");
      node.addOptionalAttribute<FormatDescriptor>(streamFormat, "formatLabel", &formatFormatLabel);
      node.addOptionalAttribute<FormatDescriptor>(streamFormat, "formatDefinition", &formatFormatDefinition);

      node.addOptionalReference<AudioChannelFormat, AudioChannelFormatId>(streamFormat, "audioChannelFormatIDRef");
      node.addOptionalReference<AudioPackFormat, AudioPackFormatId>(streamFormat, "audioPackFormatIDRef");
      node.addAudioTrackFormatReferences(streamFormat, "audioTrackFormatIDRef");
      // clang-format on
    }

    void formatAudioTrackFormat(
        XmlNode &node, std::shared_ptr<const AudioTrackFormat> trackFormat) {
      // clang-format off
      node.addAttribute<AudioTrackFormatId>(trackFormat, "audioTrackFormatID");
      node.addOptionalAttribute<AudioTrackFormatName>(trackFormat, "audioTrackFormatName");
      node.addOptionalAttribute<FormatDescriptor>(trackFormat, "formatLabel", &formatFormatLabel);
      node.addOptionalAttribute<FormatDescriptor>(trackFormat, "formatDefinition", &formatFormatDefinition);
      node.addOptionalReference<AudioStreamFormat, AudioStreamFormatId>(trackFormat, "audioStreamFormatIDRef");
      // clang-format on
    }

    void formatAudioTrackUid(XmlNode &node,
                             std::shared_ptr<const AudioTrackUid> trackUid) {
      node.addAttribute<AudioTrackUidId>(trackUid, "UID");
      node.addOptionalAttribute<SampleRate>(trackUid, "sampleRate");
      node.addOptionalAttribute<BitDepth>(trackUid, "bitDepth");
      node.addOptionalReference<AudioTrackFormat, AudioTrackFormatId>(
          trackUid, "audioTrackFormatIDRef");
      node.addOptionalReference<AudioPackFormat, AudioPackFormatId>(
          trackUid, "audioPackFormatIDRef");
    }

  }  // namespace xml
}  // namespace adm
