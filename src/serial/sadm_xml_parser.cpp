#include "adm/private/xml_parser.hpp"
#include "adm/private/xml_parser_helper.hpp"
#include "adm/serial/sadm_xml_parser.hpp"
#include "adm/common_definitions.hpp"
#include "adm/errors.hpp"
#include "adm/private/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
//#include "adm/private/xml_parser.hpp"
#include <iostream>

namespace adm {
  namespace xml {

    /// Check if a option/flag is set
    /**
     * Checks if the option @a flag is set within @a options.
     *
     * This is equivalent to an bitwise AND followed by a conversion to bool,
     * but should improve readability.
     */
    /*inline bool isSet(ParserOptions options, ParserOptions flag) {
      return static_cast<bool>(options & flag);
    }*/
    
    SadmXmlParser::SadmXmlParser(std::istream& stream, ParserOptions options,
                         std::shared_ptr<Frame> destFrame, std::shared_ptr<Frame> cdFrame)
        : xmlFile_(stream), options_(options), frame_(destFrame), cd_frame_(cdFrame) {}

    std::shared_ptr<Frame> SadmXmlParser::parse() {
      rapidxml::xml_document<> xmlDocument;
      xmlDocument.parse<0>(xmlFile_.data());
      auto root = findFrameNode(xmlDocument.first_node());
      if (root) {
        for (NodePtr node = root->first_node(); node;
             node = node->next_sibling()) {
          if (std::string(node->name()) == "frameHeader") {
            if (frame_ != nullptr) {   // Don't overwrite existing frame
              frame_->setFrameHeader(parseFrameHeader(node));
            } else {
              frame_ = Frame::create(parseFrameHeader(node));
            }
          } else {
            NodePtr afe = nullptr;
            //if (isSet(options_, ParserOptions::recursive_node_search)) {
            if (true) {
              afe = findAudioFormatExtendedNodeFullRecursive(node);
            } else {
              afe = findAudioFormatExtendedNodeEbuCore(node);
            }
            if (afe) {
              // add ADM elements to ADM document
              for (NodePtr anode = afe->first_node(); anode;
                   anode = anode->next_sibling()) {
                if (std::string(anode->name()) == "audioProgramme") {
                  frame_->add(parseAudioProgramme(anode));
                } else if (std::string(anode->name()) == "audioContent") {
                  frame_->add(parseAudioContent(anode));
                } else if (std::string(anode->name()) == "audioObject") {
                  frame_->add(parseAudioObject(anode));
                } else if (std::string(anode->name()) == "audioTrackUID") {
                  frame_->add(parseAudioTrackUid(anode));
                } else if (std::string(anode->name()) == "audioPackFormat") {
                  frame_->add(parseAudioPackFormat(anode));
                } else if (std::string(anode->name()) == "audioChannelFormat") {
                  frame_->add(parseAudioChannelFormat(anode));
                } else if (std::string(anode->name()) == "audioStreamFormat") {
                  frame_->add(parseAudioStreamFormat(anode));
                } else if (std::string(anode->name()) == "audioTrackFormat") {
                  frame_->add(parseAudioTrackFormat(anode));
                }
              }
                      
              resolveReferences(programmeContentRefs_);
              resolveReferences(contentObjectRefs_);
              resolveReferences(objectObjectRefs_);
              resolveReferences(objectPackFormatRefs_);
              resolveReferences(objectTrackUidRefs_);
              resolveReference(trackUidTrackFormatRef_);
              resolveReference(trackUidPackFormatRef_);
              resolveReference(trackUidChannelFormatRef_);
              resolveReferences(packFormatChannelFormatRefs_);
              resolveReferences(packFormatPackFormatRefs_);
              resolveReference(trackFormatStreamFormatRef_);
              resolveReference(streamFormatChannelFormatRef_);
              resolveReference(streamFormatPackFormatRef_);
              resolveReferences(streamFormatTrackFormatRefs_);
            }
          }
        }
      } else {
        throw std::runtime_error("frame node not found");
      }
      return frame_;
    }  // namespace xml

    
    /**
     * @brief Find the top level element 'frame'
     *
     * This function recursively tries to find the frame node.
     * It walks down the XML always checking the name of the first node. It
     * returns a nullptr if no frame node could be found.
     *
     * @note: Only the first frame node will be found!
     */
    NodePtr SadmXmlParser::findFrameNode(NodePtr root) {
      // ituADM is for common definitions reading
      if (std::string(root->name()) == "frame" || std::string(root->name()) == "ituADM") {
        return root;
      } else {
        if (root->first_node()) {
          return findFrameNode(root->first_node());
        } else {
          return nullptr;
        }
      }
    }

    FrameHeader SadmXmlParser::parseFrameHeader(NodePtr node) {
      // clang-format off
      auto element = detail::findElement(node, "frameFormat");
      FrameFormatId ff_id = parseAttribute<FrameFormatId>(element, "frameFormatID", &parseFrameFormatId);
      FrameStart ff_start = parseAttribute<FrameStart>(element, "start", &parseTimecode);
      FrameDuration ff_duration = parseAttribute<FrameDuration>(element, "duration", &parseTimecode);
      FrameType ff_type = parseAttribute<FrameType>(element, "type");
        
      FrameHeader frameHeader(ff_start, ff_duration, ff_type);
      FrameFormat frameFormat = frameHeader.frameFormat();
      frameFormat.set(ff_id);
      setOptionalAttribute<CountToFull>(element, "countToFull", frameFormat);
      setOptionalAttribute<FlowId>(element, "flowID", frameFormat);

      frameHeader.add(frameFormat);
  
      auto elements = detail::findElements(node, "transportTrackFormat");
      for (auto el : elements) {
        TransportTrackFormat transportTrackFormat;
        TransportId tr_id = parseAttribute<TransportId>(el, "transportID", &parseTransportId);
        transportTrackFormat.set(tr_id);
        auto at_elements = detail::findElements(el, "audioTrack");
        for (auto at_element : at_elements) {
          TrackId track_id = parseAttribute<TrackId>(at_element, "trackID");
          AudioTrack audioTrack(track_id);
          auto atu_elements = detail::findElements(at_element, "audioTrackUIDRef");
          for (auto atu_element : atu_elements) {
             audioTrack.add(parseAudioTrackUidId(atu_element->value()));
          }
          transportTrackFormat.add(audioTrack);
        }
        frameHeader.add(transportTrackFormat);
      }

      // clang-format on
      return frameHeader;
    }

    std::shared_ptr<AudioProgramme> SadmXmlParser::parseAudioProgramme(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioProgrammeName>(node, "audioProgrammeName");
      AudioProgrammeId id = parseAttribute<AudioProgrammeId>(node, "audioProgrammeID", &parseAudioProgrammeId);
      if(frame_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioProgramme = AudioProgramme::create(name, id);

      setOptionalAttribute<AudioProgrammeLanguage>(node, "audioProgrammeLanguage", audioProgramme);
      setOptionalAttribute<Start>(node, "start", audioProgramme, &parseTimecode);
      setOptionalAttribute<End>(node, "end", audioProgramme, &parseTimecode);
      setOptionalAttribute<MaxDuckingDepth>(node, "maxDuckingDepth", audioProgramme);

      setOptionalElement<LoudnessMetadata>(node, "loudnessMetadata", audioProgramme, &parseLoudnessMetadata);
      setOptionalElement<AudioProgrammeReferenceScreen>(node, "audioProgrammeReferenceScreen", audioProgramme, &parseAudioProgrammeReferenceScreen);

      addOptionalReferences<AudioContentId>(node, "audioContentIDRef", audioProgramme, programmeContentRefs_, &parseAudioContentId);
      // clang-format on
      return audioProgramme;
    }

    std::shared_ptr<AudioContent> SadmXmlParser::parseAudioContent(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioContentName>(node, "audioContentName");
      auto id = parseAttribute<AudioContentId>(node, "audioContentID", &parseAudioContentId);
      if(frame_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioContent = AudioContent::create(name, id);

      setOptionalAttribute<AudioContentLanguage>(node, "audioContentLanguage", audioContent);

      setOptionalElement<LoudnessMetadata>(node, "loudnessMetadata", audioContent, &parseLoudnessMetadata);
      setOptionalElement<ContentKind>(node, "dialogue", audioContent, &parseContentKind);

      addOptionalReferences<AudioObjectId>(node, "audioObjectIDRef", audioContent, contentObjectRefs_, &parseAudioObjectId);
      // clang-format on
      return audioContent;
    }

    std::shared_ptr<AudioObject> SadmXmlParser::parseAudioObject(NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioObjectName>(node, "audioObjectName");
      auto id = parseAttribute<AudioObjectId>(node, "audioObjectID", &parseAudioObjectId);
      if(frame_->lookup(id) != nullptr) {
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
      // clang-format on
      return audioObject;
    }

    
    std::shared_ptr<AudioPackFormat> SadmXmlParser::parseAudioPackFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioPackFormatName>(node, "audioPackFormatName");
      auto id = parseAttribute<AudioPackFormatId>(node, "audioPackFormatID", &parseAudioPackFormatId);
      if(frame_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto typeDescriptor = id.get<TypeDescriptor>();

      auto typeLabel = parseOptionalAttribute<TypeDescriptor>(node, "typeLabel", &parseTypeLabel);
      auto typeDefinition = parseOptionalAttribute<TypeDescriptor>(node, "typeDefinition", &parseTypeDefinition);
      checkChannelType(id, typeLabel, typeDefinition);
      
      if (typeDescriptor == adm::TypeDefinition::HOA) {
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

    void SadmXmlParser::setCommonProperties(
        std::shared_ptr<AudioPackFormat> audioPackFormat, NodePtr node) {
      // clang-format off
      setOptionalAttribute<Importance>(node, "importance", audioPackFormat);
      setOptionalAttribute<AbsoluteDistance>(node, "absoluteDistance", audioPackFormat);
      addOptionalReferences<AudioChannelFormatId>(node, "audioChannelFormatIDRef", audioPackFormat, packFormatChannelFormatRefs_, &parseAudioChannelFormatId);
      addOptionalReferences<AudioPackFormatId>(node, "audioPackFormatIDRef", audioPackFormat, packFormatPackFormatRefs_, &parseAudioPackFormatId);
      // clang-format on
    }

    std::shared_ptr<AudioChannelFormat> SadmXmlParser::parseAudioChannelFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioChannelFormatName>(node, "audioChannelFormatName");
      auto id = parseAttribute<AudioChannelFormatId>(node, "audioChannelFormatID", &parseAudioChannelFormatId);
      if(frame_->lookup(id) != nullptr) {
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
        // for (auto& element : elements) {
        //    audioChannelFormat->add(parseAudioBlockFormatHoa(element));
        // }
      } else if (audioChannelFormat->get<TypeDescriptor>() ==
                 TypeDefinition::BINAURAL) {
        // for (auto& element : elements) {
        //    audioChannelFormat->add(parseAudioBlockFormatBinaural(element));
        // }
      }
      return audioChannelFormat;
    }

    std::shared_ptr<AudioStreamFormat> SadmXmlParser::parseAudioStreamFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioStreamFormatName>(node, "audioStreamFormatName");
      auto id = parseAttribute<AudioStreamFormatId>(node, "audioStreamFormatID", &parseAudioStreamFormatId);
      if(frame_->lookup(id) != nullptr) {
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

    std::shared_ptr<AudioTrackFormat> SadmXmlParser::parseAudioTrackFormat(
        NodePtr node) {
      // clang-format off
      auto name = parseAttribute<AudioTrackFormatName>(node, "audioTrackFormatName");
      auto id = parseAttribute<AudioTrackFormatId>(node, "audioTrackFormatID", &parseAudioTrackFormatId);
      if(frame_->lookup(id) != nullptr) {
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

    std::shared_ptr<AudioTrackUid> SadmXmlParser::parseAudioTrackUid(
        NodePtr node) {
      // clang-format off
      auto id = parseAttribute<AudioTrackUidId>(node, "UID", &parseAudioTrackUidId);
      if(frame_->lookup(id) != nullptr) {
        throw error::XmlParsingDuplicateId(formatId(id), getDocumentLine(node));
      }
      auto audioTrackUid = AudioTrackUid::create(id);

      setOptionalAttribute<SampleRate>(node, "sampleRate", audioTrackUid);
      setOptionalAttribute<BitDepth>(node, "bitDepth", audioTrackUid);
      
      setOptionalReference<AudioTrackFormatId>(node, "audioTrackFormatIDRef", audioTrackUid, trackUidTrackFormatRef_, &parseAudioTrackFormatId);
      setOptionalReference<AudioPackFormatId>(node, "audioPackFormatIDRef", audioTrackUid, trackUidPackFormatRef_, &parseAudioPackFormatId);
      setOptionalReference<AudioChannelFormatId>(node, "audioChannelFormatIDRef", audioTrackUid, trackUidChannelFormatRef_, &parseAudioChannelFormatId);
              
      // clang-format on
      return audioTrackUid;
    }
  }  // namespace xml
}  // namespace adm
