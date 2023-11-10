#include "adm/serial/frame_header_parser.hpp"
#include "adm/private/xml_parser_helper.hpp"
#include "adm/common_definitions.hpp"
#include "adm/errors.hpp"
#include "adm/private/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <iostream>
#include "adm/private/document_parser.hpp"
#include "adm/serial/changed_ids.hpp"
#include "adm/private/id_ref_traits.hpp"

namespace adm {
  namespace xml {
    FrameHeaderParser::FrameHeaderParser(const std::string& filename,
                                         ParserOptions options)
        : FrameHeaderParser(rapidxml::file<>(filename.c_str()), options){}

    FrameHeaderParser::FrameHeaderParser(std::istream& stream,
                                         ParserOptions options)
        : FrameHeaderParser(rapidxml::file<>(stream), options){}

    FrameHeaderParser::FrameHeaderParser(rapidxml::file<> file,
                                         ParserOptions options)
        : file{std::move(file)},
          options{std::move(options)}
    {}

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
    
    FrameHeader FrameHeaderParser::parse() {
      rapidxml::xml_document<> document;
      document.parse<0>(file.data());

      if (!document.first_node())
        throw error::XmlParsingError("xml document is empty");

      auto frame = findFrameNode(document.first_node());
      if (frame) {
        for (NodePtr node = frame->first_node(); node;
             node = frame->next_sibling()) {
          if (std::string(node->name()) == "frameHeader") {
            return parseFrameHeader(node);
          }
        }
      }
      throw std::runtime_error("frameHeader not found");
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
    NodePtr FrameHeaderParser::findFrameNode(NodePtr root) {
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

    namespace {
      Status parseStatus(std::string attribute) {
        if (attribute == "new") {
          return Status{StatusValue::NEW};
        }
        if (attribute == "changed") {
          return Status{StatusValue::CHANGED};
        }
        if (attribute == "extended") {
          return Status{StatusValue::EXTENDED};
        }
        if (attribute == "expired") {
          return Status{StatusValue::EXPIRED};
        }
        throw std::runtime_error(
            attribute.append("is not a valid 'status' value. Valid values are: "
                             "'new', 'changed', 'extended' and 'expired'"));
      }

      template <typename T>
      typename T::value_type parseIdRef(NodePtr idRefNode) {
        auto id = detail::ChangedIdTraits<T>::parseId(idRefNode->value());
        auto status = parseAttribute<Status>(idRefNode, "status", &parseStatus);
        return typename T::value_type{id, status};
      }

      template <typename T>
      void addIdReferences(NodePtr changedIdsNode, ChangedIds& changedIds) {
        addOptionalElements<typename T::value_type>(
            changedIdsNode, detail::ChangedIdTraits<T>::elementName, changedIds,
            &parseIdRef<T>);
      }

      ChangedIds parseChangedIds(NodePtr changedIdsNode) {
        ChangedIds ids;
        addIdReferences<ChangedAudioChannelFormatIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioPackFormatIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioTrackUidIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioTrackFormatIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioStreamFormatIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioObjectIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioContentIds>(changedIdsNode, ids);
        addIdReferences<ChangedAudioProgrammeIds>(changedIdsNode, ids);
        return ids;
      }

      FrameType parseFrameType(std::string const& attribute) {
        if (attribute == "full") {
          return FrameType{FrameTypeValue::FULL};
        }
        if (attribute == "header") {
          return FrameType{FrameTypeValue::HEADER};
        }
        if (attribute == "intermediate") {
          return FrameType{FrameTypeValue::INTERMEDIATE};
        }
        if (attribute == "all") {
          return FrameType{FrameTypeValue::ALL};
        }
        if (attribute == "divided") {
          return FrameType{FrameTypeValue::DIVIDED};
        }
        throw std::runtime_error(
            attribute +
            " is not a valid FrameType value, valid values are \"full\","
            " \"header\", \"intermediate\", \"all\" and \"divided\"");
      }

      FrameFormat createFrameFormat(NodePtr frameFormatNode) {
        FrameFormatId id = parseAttribute<FrameFormatId>(
            frameFormatNode, "frameFormatID", &parseFrameFormatId);
        FrameStart start = parseAttribute<FrameStart>(frameFormatNode, "start",
                                                      &parseTimecode);
        FrameDuration duration = parseAttribute<FrameDuration>(
            frameFormatNode, "duration", &parseTimecode);
        FrameType type =
            parseAttribute<FrameType>(frameFormatNode, "type", &parseFrameType);
        return {id, start, duration, type};
      }

      TimeReference parseTimeReference(std::string attribute) {
        if (attribute == "local") {
          return TimeReference{TimeReferenceValue::LOCAL};
        }
        if (attribute == "total") {
          return TimeReference{TimeReferenceValue::TOTAL};
        }
        throw std::runtime_error(attribute +
                                 " is not a valid value for timeReference. "
                                 "Valid values are 'local' and 'total'");
      }

      void parseOptionalElements(NodePtr frameFormatNode, FrameFormat& format) {
        setOptionalAttribute<TimeReference>(frameFormatNode, "timeReference",
                                            format, &parseTimeReference);
        setOptionalAttribute<CountToFull>(frameFormatNode, "countToFull",
                                          format);
        setOptionalAttribute<NumMetadataChunks>(frameFormatNode,
                                                "numMetadataChunks", format);
        setOptionalAttribute<CountToSameChunk>(frameFormatNode,
                                               "countToSameChunk", format);
        setOptionalAttribute<FlowId>(frameFormatNode, "flowID", format);
        auto id_node = detail::findElement(frameFormatNode, "changedIDs");
        if (id_node) {
          format.set(parseChangedIds(id_node));
        }
      }

      FrameFormat parseFrameFormat(NodePtr frameFormatNode) {
        auto format = createFrameFormat(frameFormatNode);
        parseOptionalElements(frameFormatNode, format);
        return format;
      }

      AudioTrack createAudioTrack(NodePtr audioTrackNode) {
        TrackId track_id = parseAttribute<TrackId>(audioTrackNode, "trackID");
        AudioTrack audioTrack(track_id);
        auto atu_elements =
            detail::findElements(audioTrackNode, "audioTrackUIDRef");
        for (auto atu_element : atu_elements) {
          audioTrack.add(parseAudioTrackUidId(atu_element->value()));
        }
        return audioTrack;
      }

      TransportTrackFormat createTrackPortTrackFormat(
          NodePtr transportTrackFormatNode) {
        TransportId tr_id = parseAttribute<TransportId>(
            transportTrackFormatNode, "transportID", &parseTransportId);
        return TransportTrackFormat{tr_id};
      }

      void parseAllAudioTracks(NodePtr transportTrackFormatNode,
                               TransportTrackFormat& transportTrackFormat) {
        auto audioTracks =
            detail::findElements(transportTrackFormatNode, "audioTrack");
        for (auto audioTrackNode : audioTracks) {
          transportTrackFormat.add(createAudioTrack(audioTrackNode));
        }
      }

      TransportTrackFormat parseTransportTrackFormat(
          NodePtr transportTrackFormatNode) {
        auto transportTrackFormat =
            createTrackPortTrackFormat(transportTrackFormatNode);
        parseAllAudioTracks(transportTrackFormatNode, transportTrackFormat);
        return transportTrackFormat;
      }

      void parseAllTransportTrackFormats(NodePtr frameHeaderNode,
                                         FrameHeader& frameHeader) {
        auto elements =
            detail::findElements(frameHeaderNode, "transportTrackFormat");
        for (auto el : elements) {
          frameHeader.add(parseTransportTrackFormat(el));
        }
      }

      //      ChangedIds parse
    }  // namespace

    FrameHeader FrameHeaderParser::parseFrameHeader(NodePtr node) {
      auto element = detail::findElement(node, "frameFormat");
      if (!element) {
        throw std::runtime_error("Missing frameFormat element");
      }

      FrameHeader frameHeader(parseFrameFormat(element));
      parseAllTransportTrackFormats(node, frameHeader);
      if (auto profileListNode = detail::findElement(node, "profileList")) {
        frameHeader.set(parseProfileList(profileListNode));
      }
      return frameHeader;
    }

  }  // namespace xml
}  // namespace adm
