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

    XmlParser::XmlParser(const std::string& filename, ParserOptions options,
                         std::shared_ptr<Document> destDocument)
        : xmlFile_(filename.c_str()),
          options_(options),
          document_(destDocument) {}

    XmlParser::XmlParser(std::istream& stream, ParserOptions options,
                         std::shared_ptr<Document> destDocument)
        : xmlFile_(stream), options_(options), document_(destDocument) {}

    std::shared_ptr<Document> XmlParser::parse() {
      rapidxml::xml_document<> xmlDocument;
      xmlDocument.parse<0>(xmlFile_.data());
      NodePtr root = nullptr;
      if (isSet(options_, ParserOptions::recursive_node_search)) {
        root =
            findAudioFormatExtendedNodeFullRecursive(xmlDocument.first_node());
      } else {
        root = findAudioFormatExtendedNodeEbuCore(xmlDocument.first_node());
      }
      if (root) {
        // add ADM elements to ADM document
        for (NodePtr node = root->first_node(); node;
             node = node->next_sibling()) {
          if (std::string(node->name()) == "audioProgramme") {
            document_->add(parseAudioProgramme(node));
          } else if (std::string(node->name()) == "audioContent") {
            document_->add(parseAudioContent(node));
          } else if (std::string(node->name()) == "audioObject") {
            document_->add(parseAudioObject(node));
          } else if (std::string(node->name()) == "audioTrackUID") {
            document_->add(parseAudioTrackUid(node));
          } else if (std::string(node->name()) == "audioPackFormat") {
            document_->add(parseAudioPackFormat(node));
          } else if (std::string(node->name()) == "audioChannelFormat") {
            document_->add(parseAudioChannelFormat(node));
          } else if (std::string(node->name()) == "audioStreamFormat") {
            document_->add(parseAudioStreamFormat(node));
          } else if (std::string(node->name()) == "audioTrackFormat") {
            document_->add(parseAudioTrackFormat(node));
          }
        }
        resolveReferences(programmeContentRefs_);
        resolveReferences(contentObjectRefs_);
        resolveReferences(objectObjectRefs_);
        resolveReferences(objectPackFormatRefs_);
        resolveReferences(objectTrackUidRefs_);
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
        throw std::runtime_error("audioFormatExtended node not found");
      }
      return document_;
    }  // namespace xml

    /**
     * @brief Find the top level element 'audioFormatExtended'
     *
     * This function recursively tries to find the audioFormatExtended node.
     * It walks down the XML always checking the names of the nodes. It returns
     * a nullptr if no audioFormatExtended node could be found.
     *
     * @note: Only the first audioFormatExtended node will be found!
     */
    NodePtr findAudioFormatExtendedNodeEbuCore(NodePtr node) {
      if (std::string(node->name()) != "ebuCoreMain") {
        return nullptr;
      }
      auto coreMetadataNodes = detail::findElements(node, "coreMetadata");
      if (coreMetadataNodes.size() != 1) {
        return nullptr;
      }
      auto formatNodes =
          detail::findElements(coreMetadataNodes.at(0), "format");
      if (formatNodes.size() != 1) {
        return nullptr;
      }
      auto audioFormatExtendedNodes =
          detail::findElements(formatNodes.at(0), "audioFormatExtended");
      if (audioFormatExtendedNodes.size() != 1) {
        return nullptr;
      }
      return audioFormatExtendedNodes.at(0);
    }

    /**
     * @brief Find the top level element 'audioFormatExtended'
     *
     * This function recursively tries to find the audioFormatExtended node.
     * It walks down the XML always checking the names of the nodes. It returns
     * a nullptr if no audioFormatExtended node could be found.
     *
     * @note: Only the first audioFormatExtended node will be found!
     */
    NodePtr findAudioFormatExtendedNodeFullRecursive(NodePtr node) {
      if (std::string(node->name()) == "audioFormatExtended") {
        return node;
      }
      for (NodePtr childnode = node->first_node(); childnode;
           childnode = childnode->next_sibling()) {
        auto rtnNode = findAudioFormatExtendedNodeFullRecursive(childnode);
        if (rtnNode) {
          return rtnNode;
        }
      }
      return nullptr;
    }

    std::shared_ptr<AudioProgramme> XmlParser::parseAudioProgramme(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioProgrammeName>(node, "audioProgrammeName");
      AudioProgrammeId id = parseAttribute<AudioProgrammeId>(node, "audioProgrammeID", &parseAudioProgrammeId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioProgramme = AudioProgramme::create(name, id);

      setOptionalAttribute<AudioProgrammeLanguage>(node, "audioProgrammeLanguage", audioProgramme);
      setOptionalAttribute<Start>(node, "start", audioProgramme, &parseTimecode);
      setOptionalAttribute<End>(node, "end", audioProgramme, &parseTimecode);
      setOptionalAttribute<MaxDuckingDepth>(node, "maxDuckingDepth", audioProgramme);

      setOptionalMultiElement<LoudnessMetadatas>(node, "loudnessMetadata", audioProgramme, &parseLoudnessMetadatas);
      setOptionalElement<AudioProgrammeReferenceScreen>(node, "audioProgrammeReferenceScreen", audioProgramme, &parseAudioProgrammeReferenceScreen);

      addOptionalReferences<AudioContentId>(node, "audioContentIDRef", audioProgramme, programmeContentRefs_, &parseAudioContentId);

      addOptionalElements<Label>(node, "audioProgrammeLabel", audioProgramme, &parseLabel);
      // clang-format on
      return audioProgramme;
    }

    std::shared_ptr<AudioContent> XmlParser::parseAudioContent(NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioContentName>(node, "audioContentName");
      auto id = parseAttribute<AudioContentId>(node, "audioContentID", &parseAudioContentId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioContent = AudioContent::create(name, id);

      setOptionalAttribute<AudioContentLanguage>(node, "audioContentLanguage", audioContent);

      setOptionalMultiElement<LoudnessMetadatas>(node, "loudnessMetadata", audioContent, &parseLoudnessMetadatas);
      setOptionalElement<ContentKind>(node, "dialogue", audioContent, &parseContentKind);

      addOptionalReferences<AudioObjectId>(node, "audioObjectIDRef", audioContent, contentObjectRefs_, &parseAudioObjectId);

      addOptionalElements<Label>(node, "audioContentLabel", audioContent, &parseLabel);
      // clang-format on
      return audioContent;
    }

    std::shared_ptr<AudioObject> XmlParser::parseAudioObject(NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioObjectName>(node, "audioObjectName");
      auto id = parseAttribute<AudioObjectId>(node, "audioObjectID", &parseAudioObjectId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioObject = AudioObject::create(name, id);

      setOptionalAttribute<Start>(node, "start", audioObject, &parseTimecode);
      setOptionalAttribute<Duration>(node, "duration", audioObject, &parseTimecode);
      setOptionalAttribute<DialogueId>(node, "dialogue", audioObject);
      setOptionalAttribute<Importance>(node, "importance", audioObject);
      setOptionalAttribute<Interact>(node, "interact", audioObject);
      setOptionalAttribute<DisableDucking>(node, "disableDucking", audioObject);

      addOptionalReferences<AudioObjectId>(node, "audioObjectIDRef", audioObject, objectObjectRefs_, &parseAudioObjectId);
      addOptionalReferences<AudioPackFormatId>(node, "audioPackFormatIDRef", audioObject, objectPackFormatRefs_, &parseAudioPackFormatId);
      addOptionalReferences<AudioTrackUidId>(node, "audioTrackUIDRef", audioObject, objectTrackUidRefs_, &parseAudioTrackUidId);
      setOptionalElement<AudioObjectInteraction>(node, "audioObjectInteraction", audioObject, &parseAudioObjectInteraction);
      addOptionalElements<Label>(node, "audioObjectLabel", audioObject, &parseLabel);
      addOptionalElements<AudioComplementaryObjectGroupLabel>(node, "audioComplementaryObjectGroupLabel", audioObject, &parseLabel);

      // clang-format on
      return audioObject;
    }

    AudioObjectInteraction parseAudioObjectInteraction(NodePtr node) {
      // clang-format off
      auto onOffInteract = parseAttribute<OnOffInteract>(node, "onOffInteract");
      AudioObjectInteraction objectInteraction(onOffInteract);
      setOptionalAttribute<GainInteract>(node, "gainInteract", objectInteraction);
      setOptionalAttribute<PositionInteract>(node, "positionInteract", objectInteraction);
      setOptionalMultiElement<GainInteractionRange>(node, "gainInteractionRange", objectInteraction, &parseGainInteractionRange);
      setOptionalMultiElement<PositionInteractionRange>(node, "positionInteractionRange", objectInteraction, &parsePositionInteractionRange);
      // clang-format on
      return objectInteraction;
    }

    GainInteractionRange parseGainInteractionRange(std::vector<NodePtr> nodes) {
      GainInteractionRange gainInteraction;
      for (auto& element : nodes) {
        auto bound =
            parseAttribute<GainInteractionBoundValue>(element, "bound");
        if (bound.get() == "min") {
          gainInteraction.set(GainInteractionMin(parseGain(element)));
        } else if (bound.get() == "max") {
          gainInteraction.set(GainInteractionMax(parseGain(element)));
        }
      }
      return gainInteraction;
    }

    PositionInteractionRange parsePositionInteractionRange(
        std::vector<NodePtr> nodes) {
      PositionInteractionRange positionInteraction;
      for (auto& element : nodes) {
        auto bound =
            parseAttribute<PositionInteractionBoundValue>(element, "bound");
        auto coordinate =
            parseAttribute<CoordinateInteractionValue>(element, "coordinate");
        if (coordinate.get() == "azimuth" && bound.get() == "min") {
          setValue<AzimuthInteractionMin>(element, positionInteraction);
        } else if (coordinate.get() == "azimuth" && bound.get() == "max") {
          setValue<AzimuthInteractionMax>(element, positionInteraction);
        } else if (coordinate.get() == "elevation" && bound.get() == "min") {
          setValue<ElevationInteractionMin>(element, positionInteraction);
        } else if (coordinate.get() == "elevation" && bound.get() == "max") {
          setValue<ElevationInteractionMax>(element, positionInteraction);
        } else if (coordinate.get() == "distance" && bound.get() == "min") {
          setValue<DistanceInteractionMin>(element, positionInteraction);
        } else if (coordinate.get() == "distance" && bound.get() == "max") {
          setValue<DistanceInteractionMax>(element, positionInteraction);
        } else if (coordinate.get() == "X" && bound.get() == "min") {
          setValue<XInteractionMin>(element, positionInteraction);
        } else if (coordinate.get() == "X" && bound.get() == "max") {
          setValue<XInteractionMax>(element, positionInteraction);
        } else if (coordinate.get() == "Y" && bound.get() == "min") {
          setValue<YInteractionMin>(element, positionInteraction);
        } else if (coordinate.get() == "Y" && bound.get() == "max") {
          setValue<YInteractionMax>(element, positionInteraction);
        } else if (coordinate.get() == "Z" && bound.get() == "min") {
          setValue<ZInteractionMin>(element, positionInteraction);
        } else if (coordinate.get() == "Z" && bound.get() == "max") {
          setValue<ZInteractionMax>(element, positionInteraction);
        }
      }
      return positionInteraction;
    }

    std::shared_ptr<AudioPackFormat> XmlParser::parseAudioPackFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioPackFormatName>(node, "audioPackFormatName");
      auto id = parseAttribute<AudioPackFormatId>(node, "audioPackFormatID", &parseAudioPackFormatId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto typeDescriptor = id.get<TypeDescriptor>();

      auto typeLabel = parseOptionalAttribute<TypeDescriptor>(node, "typeLabel", &parseTypeLabel);
      auto typeDefinition = parseOptionalAttribute<TypeDescriptor>(node, "typeDefinition", &parseTypeDefinition);
      checkChannelType(id, typeLabel, typeDefinition);

      if(typeDescriptor == adm::TypeDefinition::HOA){
          auto audioPackFormat = AudioPackFormatHoa::create(name, id);
          setCommonProperties(audioPackFormat, node);
          setOptionalAttribute<Normalization>(node, "normalization", audioPackFormat);
          setOptionalAttribute<ScreenRef>(node, "screenRef", audioPackFormat);
          setOptionalAttribute<NfcRefDist>(node, "nfcRefDist", audioPackFormat);
          return audioPackFormat;
      } else {
          auto audioPackFormat = AudioPackFormat::create(name, typeDescriptor, id);
          setCommonProperties(audioPackFormat, node);
          return audioPackFormat;
      }
      // clang-format on
    }

    void XmlParser::setCommonProperties(
        std::shared_ptr<AudioPackFormat> audioPackFormat, NodePtr node) {
      // clang-format off
      setOptionalAttribute<Importance>(node, "importance", audioPackFormat);
      setOptionalAttribute<AbsoluteDistance>(node, "absoluteDistance", audioPackFormat);
      addOptionalReferences<AudioChannelFormatId>(node, "audioChannelFormatIDRef", audioPackFormat, packFormatChannelFormatRefs_, &parseAudioChannelFormatId);
      addOptionalReferences<AudioPackFormatId>(node, "audioPackFormatIDRef", audioPackFormat, packFormatPackFormatRefs_, &parseAudioPackFormatId);
      // clang-format on
    }

    std::shared_ptr<AudioChannelFormat> XmlParser::parseAudioChannelFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioChannelFormatName>(node, "audioChannelFormatName");
      auto id = parseAttribute<AudioChannelFormatId>(node, "audioChannelFormatID", &parseAudioChannelFormatId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioChannelFormat = AudioChannelFormat::create(name, id.get<TypeDescriptor>(), id);

      auto typeLabel = parseOptionalAttribute<TypeDescriptor>(node, "typeLabel", &parseTypeLabel);
      auto typeDefinition = parseOptionalAttribute<TypeDescriptor>(node, "typeDefinition", &parseTypeDefinition);
      checkChannelType(id, typeLabel, typeDefinition);

      setOptionalMultiElement<Frequency>(node, "frequency", audioChannelFormat, &parseFrequency);
      // clang-format on

      auto elements = detail::findElements(node, "audioBlockFormat");

      if (audioChannelFormat->get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS) {
        for (auto& element : elements) {
          audioChannelFormat->add(parseAudioBlockFormatDirectSpeakers(element));
        }
      } else if (audioChannelFormat->get<TypeDescriptor>() ==
                 TypeDefinition::MATRIX) {
        // for (auto& element : elements) {
        //    audioChannelFormat->add(parseAudioBlockFormatMatrix(element));
        // }
      } else if (audioChannelFormat->get<TypeDescriptor>() ==
                 TypeDefinition::OBJECTS) {
        for (auto& element : elements) {
          audioChannelFormat->add(parseAudioBlockFormatObjects(element));
        }
      } else if (audioChannelFormat->get<TypeDescriptor>() ==
                 TypeDefinition::HOA) {
        for (auto& element : elements) {
          audioChannelFormat->add(parseAudioBlockFormatHoa(element));
        }
      } else if (audioChannelFormat->get<TypeDescriptor>() ==
                 TypeDefinition::BINAURAL) {
        // for (auto& element : elements) {
        //    audioChannelFormat->add(parseAudioBlockFormatBinaural(element));
        // }
      }
      return audioChannelFormat;
    }

    std::shared_ptr<AudioStreamFormat> XmlParser::parseAudioStreamFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioStreamFormatName>(node, "audioStreamFormatName");
      auto id = parseAttribute<AudioStreamFormatId>(node, "audioStreamFormatID", &parseAudioStreamFormatId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }

      auto formatLabel = parseOptionalAttribute<FormatDescriptor>(node, "formatLabel", &parseFormatLabel);
      auto formatDefinition = parseOptionalAttribute<FormatDescriptor>(node, "formatDefinition", &parseFormatDefinition);
      auto format = checkFormat(formatLabel, formatDefinition);
      auto audioStreamFormat = AudioStreamFormat::create(name, format, id);

      setOptionalReference<AudioChannelFormatId>(node, "audioChannelFormatIDRef", audioStreamFormat, streamFormatChannelFormatRef_, &parseAudioChannelFormatId);
      setOptionalReference<AudioPackFormatId>(node, "audioPackFormatIDRef", audioStreamFormat, streamFormatPackFormatRef_, &parseAudioPackFormatId);
      addOptionalReferences<AudioTrackFormatId>(node, "audioTrackFormatIDRef", audioStreamFormat, streamFormatTrackFormatRefs_, &parseAudioTrackFormatId);
      // clang-format on
      return audioStreamFormat;
    }

    std::shared_ptr<AudioTrackFormat> XmlParser::parseAudioTrackFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioTrackFormatName>(node, "audioTrackFormatName");
      auto id = parseAttribute<AudioTrackFormatId>(node, "audioTrackFormatID", &parseAudioTrackFormatId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }

      auto formatLabel = parseOptionalAttribute<FormatDescriptor>(node, "formatLabel", &parseFormatLabel);
      auto formatDefinition = parseOptionalAttribute<FormatDescriptor>(node, "formatDefinition", &parseFormatDefinition);
      auto format = checkFormat(formatLabel, formatDefinition);

      auto audioTrackFormat = AudioTrackFormat::create(name, format, id);

      setOptionalReference<AudioStreamFormatId>(node, "audioStreamFormatIDRef", audioTrackFormat, trackFormatStreamFormatRef_, &parseAudioStreamFormatId);
      // clang-format on
      return audioTrackFormat;
    }

    std::shared_ptr<AudioTrackUid> XmlParser::parseAudioTrackUid(NodePtr node) {
      // clang-format off
      auto id = parseAttribute<AudioTrackUidId>(node, "UID", &parseAudioTrackUidId);
      if(document_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioTrackUid = AudioTrackUid::create(id);

      setOptionalAttribute<SampleRate>(node, "sampleRate", audioTrackUid);
      setOptionalAttribute<BitDepth>(node, "bitDepth", audioTrackUid);

      setOptionalReference<AudioChannelFormatId>(node, "audioChannelFormatIDRef", audioTrackUid, trackUidChannelFormatRef_, &parseAudioChannelFormatId);
      setOptionalReference<AudioTrackFormatId>(node, "audioTrackFormatIDRef", audioTrackUid, trackUidTrackFormatRef_, &parseAudioTrackFormatId);
      setOptionalReference<AudioPackFormatId>(node, "audioPackFormatIDRef", audioTrackUid, trackUidPackFormatRef_, &parseAudioPackFormatId);
      // clang-format on
      return audioTrackUid;
    }

    AudioBlockFormatDirectSpeakers parseAudioBlockFormatDirectSpeakers(
        NodePtr node) {
      AudioBlockFormatDirectSpeakers audioBlockFormat;
      // clang-format off
      setOptionalAttribute<AudioBlockFormatId>(node, "audioBlockFormatID", audioBlockFormat, &parseAudioBlockFormatId);
      setOptionalAttribute<Rtime>(node, "rtime", audioBlockFormat, &parseTimecode);
      setOptionalAttribute<Duration>(node, "duration", audioBlockFormat, &parseTimecode);
      setMultiElement<SpeakerPosition>(node, "position", audioBlockFormat, &parseSpeakerPosition);
      addOptionalElements<SpeakerLabel>(node, "speakerLabel", audioBlockFormat, &parseSpeakerLabel);
      setOptionalElement<HeadphoneVirtualise>(node, "headphoneVirtualise", audioBlockFormat, &parseHeadphoneVirtualise);
      // clang-format on
      return audioBlockFormat;
    }

    SpeakerPosition parseSpeakerPosition(std::vector<NodePtr> nodes) {
      std::vector<std::pair<NodePtr, CartesianCoordinateValue>>
          cartesianCoordinates;
      std::vector<std::pair<NodePtr, SphericalCoordinateValue>>
          sphericalCoordinates;
      for (auto const& element : nodes) {
        auto coordinate = element->first_attribute("coordinate");
        if (coordinate) {
          auto axis = coordinate->value();
          if (axis == std::string("X") || axis == std::string("Y") ||
              axis == std::string("Z")) {
            cartesianCoordinates.emplace_back(element, axis);
          } else if (axis == std::string{"azimuth"} ||
                     axis == std::string{"elevation"} ||
                     axis == std::string{"distance"}) {
            sphericalCoordinates.emplace_back(element, axis);
          } else {
            throw error::XmlParsingError(
                "Speaker position has invalid coordinate attribute",
                getDocumentLine(coordinate));
          };
        } else {
          throw error::XmlParsingError(
              "SpeakerPosition is missing coordinate attribute",
              getDocumentLine(element));
        }
      }

      if (cartesianCoordinates.empty() && sphericalCoordinates.empty()) {
        throw error::XmlParsingError(
            "SpeakerPosition has neither cartesian nor spherical coordinates");
      }

      if (!cartesianCoordinates.empty() && !sphericalCoordinates.empty()) {
        throw error::XmlParsingError(
            "SpeakerPosition has both cartesian and spherical coordinates");
      }

      if (!cartesianCoordinates.empty()) {
        return parseCartesianSpeakerPosition(cartesianCoordinates);
      } else {
        return parseSphericalSpeakerPosition(sphericalCoordinates);
      }
    }

    CartesianSpeakerPosition parseCartesianSpeakerPosition(
        const std::vector<
            std::pair<adm::xml::NodePtr, adm::CartesianCoordinateValue>>&
            cartesianCoordinates) {
      adm::CartesianSpeakerPosition speakerPosition;
      adm::ScreenEdgeLock screenEdgeLock;
      for (auto& coord : cartesianCoordinates) {
        auto element = coord.first;
        auto axe = coord.second;
        boost::optional<adm::BoundValue> bound;
        try {
          bound = adm::xml::parseOptionalAttribute<adm::BoundValue>(element,
                                                                    "bound");
        } catch (InvalidStringError const& e) {
          throw error::XmlParsingError(e.what(), getDocumentLine(element));
        }
        std::string boundValue;

        if (axe == "X") {
          if (bound == boost::none) {
            adm::xml::setValue<adm::X>(element, speakerPosition);
            adm::xml::setOptionalAttribute<adm::HorizontalEdge>(
                element, "screenEdgeLock", screenEdgeLock);
          } else if (bound.get() == "min") {
            adm::xml::setValue<adm::XMin>(element, speakerPosition);
          } else if (bound.get() == "max") {
            adm::xml::setValue<adm::XMax>(element, speakerPosition);
          }
        } else if (axe == "Y") {
          if (bound == boost::none) {
            adm::xml::setValue<adm::Y>(element, speakerPosition);
            adm::xml::setOptionalAttribute<adm::VerticalEdge>(
                element, "screenEdgeLock", screenEdgeLock);
          } else if (bound.get() == "min") {
            adm::xml::setValue<adm::YMin>(element, speakerPosition);
          } else if (bound.get() == "max") {
            adm::xml::setValue<adm::YMax>(element, speakerPosition);
          }
        } else if (axe == "Z") {
          if (bound == boost::none) {
            adm::xml::setValue<adm::Z>(element, speakerPosition);
          } else if (bound.get() == "min") {
            adm::xml::setValue<adm::ZMin>(element, speakerPosition);
          } else if (bound.get() == "max") {
            adm::xml::setValue<adm::ZMax>(element, speakerPosition);
          }
        }
      }

      speakerPosition.set(screenEdgeLock);
      return speakerPosition;
    }

    SphericalSpeakerPosition parseSphericalSpeakerPosition(
        std::vector<std::pair<NodePtr, SphericalCoordinateValue>> const&
            sphericalCoordinates) {
      SphericalSpeakerPosition speakerPosition;
      ScreenEdgeLock screenEdgeLock;
      for (auto& coordinate : sphericalCoordinates) {
        auto element = coordinate.first;
        auto axe = coordinate.second;
        auto bound = parseOptionalAttribute<BoundValue>(element, "bound");
        if (axe == "azimuth") {
          if (bound == boost::none) {
            setValue<Azimuth>(element, speakerPosition);
            setOptionalAttribute<HorizontalEdge>(element, "screenEdgeLock",
                                                 screenEdgeLock);
          } else if (bound.get() == "min") {
            setValue<AzimuthMin>(element, speakerPosition);
          } else if (bound.get() == "max") {
            setValue<AzimuthMax>(element, speakerPosition);
          }
        } else if (axe == "elevation") {
          if (bound == boost::none) {
            setValue<Elevation>(element, speakerPosition);
            setOptionalAttribute<VerticalEdge>(element, "screenEdgeLock",
                                               screenEdgeLock);
          } else if (bound.get() == "min") {
            setValue<ElevationMin>(element, speakerPosition);
          } else if (bound.get() == "max") {
            setValue<ElevationMax>(element, speakerPosition);
          }
        } else if (axe == "distance") {
          if (bound == boost::none) {
            setValue<Distance>(element, speakerPosition);
          } else if (bound.get() == "min") {
            setValue<DistanceMin>(element, speakerPosition);
          } else if (bound.get() == "max") {
            setValue<DistanceMax>(element, speakerPosition);
          }
        }
      }
      speakerPosition.set(screenEdgeLock);
      return speakerPosition;
    }

    SpeakerLabel parseSpeakerLabel(NodePtr node) {
      return SpeakerLabel(node->value());
    }

    HeadphoneVirtualise parseHeadphoneVirtualise(NodePtr node) {
      HeadphoneVirtualise headphoneVirtualise = HeadphoneVirtualise();
      setOptionalAttribute<Bypass>(node, "bypass", headphoneVirtualise);
      setOptionalAttribute<DirectToReverberantRatio>(node, "DRR",
                                                     headphoneVirtualise);
      return headphoneVirtualise;
    }

    AudioBlockFormatObjects parseAudioBlockFormatObjects(NodePtr node) {
      AudioBlockFormatObjects audioBlockFormat{SphericalPosition()};
      // clang-format off
      setOptionalAttribute<AudioBlockFormatId>(node, "audioBlockFormatID", audioBlockFormat, &parseAudioBlockFormatId);
      setOptionalAttribute<Rtime>(node, "rtime", audioBlockFormat, &parseTimecode);
      setOptionalAttribute<Duration>(node, "duration", audioBlockFormat, &parseTimecode);

      setOptionalElement<Cartesian>(node, "cartesian", audioBlockFormat);
      auto cartesianGuess = guessCartesianFlag(node);
      if(audioBlockFormat.get<Cartesian>() != cartesianGuess) {
        audioBlockFormat.set(cartesianGuess);
      }
      if(audioBlockFormat.get<Cartesian>() == false) {
        setMultiElement<SphericalPosition>(node, "position", audioBlockFormat, &parseSphericalPosition);
      } else {
        setMultiElement<CartesianPosition>(node, "position", audioBlockFormat, &parseCartesianPosition);
      }
      setOptionalElement<Width>(node, "width", audioBlockFormat);
      setOptionalElement<Height>(node, "height", audioBlockFormat);
      setOptionalElement<Depth>(node, "depth", audioBlockFormat);
      setOptionalElement<Gain>(node, "gain", audioBlockFormat, &parseGain);
      setOptionalElement<Diffuse>(node, "diffuse", audioBlockFormat);
      setOptionalElement<ChannelLock>(node, "channelLock", audioBlockFormat, &parseChannelLock);
      setOptionalElement<ObjectDivergence>(node, "objectDivergence", audioBlockFormat, &parseObjectDivergence);
      setOptionalElement<JumpPosition>(node, "jumpPosition", audioBlockFormat, &parseJumpPosition);
      setOptionalElement<ScreenRef>(node, "screenRef", audioBlockFormat);
      setOptionalElement<Importance>(node, "importance", audioBlockFormat);
      setOptionalElement<HeadphoneVirtualise>(node, "headphoneVirtualise", audioBlockFormat, &parseHeadphoneVirtualise);
      // clang-format on
      return audioBlockFormat;
    }

    Gain parseGain(NodePtr node) {
      auto unitAttr = node->first_attribute("gainUnit");
      double value = std::stod(node->value());
      if (unitAttr) {
        std::string unitAttrStr{unitAttr->value()};
        if (unitAttrStr == "linear")
          return Gain::fromLinear(value);
        else if (unitAttrStr == "dB")
          return Gain::fromDb(value);
        else
          throw error::XmlParsingUnexpectedAttrError("gainUnit", unitAttrStr,
                                                     getDocumentLine(unitAttr));
      } else
        return Gain::fromLinear(value);
    }

    Label parseLabel(NodePtr node) {
      Label label;
      setValue<LabelValue>(node, label);
      setOptionalAttribute<LabelLanguage>(node, "language", label);
      return label;
    }

    ChannelLock parseChannelLock(NodePtr node) {
      ChannelLock channelLock;
      setValue<ChannelLockFlag>(node, channelLock);
      setOptionalAttribute<MaxDistance>(node, "maxDistance", channelLock);
      return channelLock;
    }

    ObjectDivergence parseObjectDivergence(NodePtr node) {
      ObjectDivergence objectDivergence;
      setValue<Divergence>(node, objectDivergence);
      setOptionalAttribute<AzimuthRange>(node, "azimuthRange",
                                         objectDivergence);
      setOptionalAttribute<PositionRange>(node, "positionRange",
                                          objectDivergence);
      return objectDivergence;
    }

    Frequency parseFrequency(std::vector<NodePtr> nodes) {
      Frequency frequency;
      for (auto& element : nodes) {
        auto type = parseAttribute<FrequencyType>(element, "typeDefinition");
        if (type == "lowPass") {
          setValue<LowPass>(element, frequency);
        } else if (type == "highpass") {
          setValue<HighPass>(element, frequency);
        }
      }
      return frequency;
    }

    Cartesian guessCartesianFlag(NodePtr node) {
      auto element = detail::findElement(node, "position");
      if (element) {
        auto coordinate = element->first_attribute("coordinate");
        if (coordinate) {
          auto coordinateStr = std::string(coordinate->value());
          if (coordinateStr == "X" || coordinateStr == "Y" ||
              coordinateStr == "Z")
            return Cartesian(true);
        }
      }
      return Cartesian(false);
    }

    SphericalPosition parseSphericalPosition(std::vector<NodePtr> nodes) {
      SphericalPosition position;
      ScreenEdgeLock screenEdgeLock;
      for (auto& element : nodes) {
        auto axe =
            parseAttribute<SphericalCoordinateValue>(element, "coordinate");
        if (axe == "azimuth") {
          setValue<Azimuth>(element, position);
          setOptionalAttribute<HorizontalEdge>(element, "screenEdgeLock",
                                               screenEdgeLock);
        } else if (axe == "elevation") {
          setValue<Elevation>(element, position);
          setOptionalAttribute<VerticalEdge>(element, "screenEdgeLock",
                                             screenEdgeLock);
        } else if (axe == "distance") {
          setValue<Distance>(element, position);
        }
      }
      position.set(screenEdgeLock);
      return position;
    }

    CartesianPosition parseCartesianPosition(std::vector<NodePtr> nodes) {
      CartesianPosition position;
      for (auto& element : nodes) {
        auto axe =
            parseAttribute<CartesianCoordinateValue>(element, "coordinate");
        if (axe == "X") {
          setValue<X>(element, position);
        } else if (axe == "Y") {
          setValue<Y>(element, position);
        } else if (axe == "Z") {
          setValue<Z>(element, position);
        }
      }
      return position;
    }

    JumpPosition parseJumpPosition(NodePtr node) {
      JumpPosition jumpPosition;
      setValue<JumpPositionFlag>(node, jumpPosition);
      setOptionalAttribute<InterpolationLength>(
          node, "interpolationLength", jumpPosition, &parseInterpolationLength);
      return jumpPosition;
    }

    LoudnessMetadata parseLoudnessMetadata(NodePtr node) {
      LoudnessMetadata loudnessMetadata;
      setOptionalAttribute<LoudnessMethod>(node, "loudnessMethod",
                                           loudnessMetadata);
      setOptionalAttribute<LoudnessRecType>(node, "loudnessRecType",
                                            loudnessMetadata);
      setOptionalAttribute<LoudnessCorrectionType>(
          node, "loudnessCorrectionType", loudnessMetadata);
      setOptionalElement<IntegratedLoudness>(node, "integratedLoudness",
                                             loudnessMetadata);
      setOptionalElement<LoudnessRange>(node, "loudnessRange",
                                        loudnessMetadata);
      setOptionalElement<MaxTruePeak>(node, "maxTruePeak", loudnessMetadata);
      setOptionalElement<MaxMomentary>(node, "maxMomentary", loudnessMetadata);
      setOptionalElement<MaxShortTerm>(node, "maxShortTerm", loudnessMetadata);
      setOptionalElement<DialogueLoudness>(node, "dialogueLoudness",
                                           loudnessMetadata);
      return loudnessMetadata;
    }

    LoudnessMetadatas parseLoudnessMetadatas(std::vector<NodePtr> const& nodes) {
      LoudnessMetadatas loudnessMetatatas;
      for (auto& element : nodes) {
        auto loudnessMetadata = parseLoudnessMetadata(element);
        loudnessMetatatas.push_back(loudnessMetadata);
      }
      return loudnessMetatatas;
    }

    DialogueId parseDialogueId(NodePtr node) {
      return DialogueId(std::stoi(node->value()));
    }

    ContentKind parseContentKind(NodePtr node) {
      auto dialogueId = parseDialogueId(node);
      if (dialogueId == Dialogue::NON_DIALOGUE) {
        return ContentKind(parseAttribute<NonDialogueContentKind>(
            node, "nonDialogueContentKind"));
      } else if (dialogueId == Dialogue::DIALOGUE) {
        return ContentKind(
            parseAttribute<DialogueContentKind>(node, "dialogueContentKind"));
      } else if (dialogueId == Dialogue::MIXED) {
        return ContentKind(
            parseAttribute<MixedContentKind>(node, "mixedContentKind"));
      } else {
        throw std::runtime_error("unknown dialogue id");
      }
    }

    AudioProgrammeReferenceScreen parseAudioProgrammeReferenceScreen(
        NodePtr /* node */) {
      return AudioProgrammeReferenceScreen();
    }

    /* Below here Hoa */

    AudioBlockFormatHoa parseAudioBlockFormatHoa(NodePtr node) {
      AudioBlockFormatHoa audioBlockFormat{Order(), Degree()};
      // clang-format off
      setOptionalAttribute<AudioBlockFormatId>(node, "audioBlockFormatID", audioBlockFormat, &parseAudioBlockFormatId);
      setOptionalAttribute<Rtime>(node, "rtime", audioBlockFormat, &parseTimecode);
      setOptionalAttribute<Duration>(node, "duration", audioBlockFormat, &parseTimecode);
      setOptionalElement<Order>(node, "order", audioBlockFormat);
      setOptionalElement<Degree>(node, "degree", audioBlockFormat);
      setOptionalElement<NfcRefDist>(node, "nfcRefDist", audioBlockFormat);
      setOptionalElement<ScreenRef>(node, "screenRef", audioBlockFormat);
      setOptionalElement<Normalization>(node, "normalization", audioBlockFormat);
      setOptionalElement<Equation>(node, "equation", audioBlockFormat);
      setOptionalElement<HeadphoneVirtualise>(node, "headphoneVirtualise", audioBlockFormat, &parseHeadphoneVirtualise);
      // clang-format on
      return audioBlockFormat;
    }

  }  // namespace xml
}  // namespace adm
