#include "adm/serial/sadm_xml_parser.hpp"
#include "adm/private/xml_parser_helper.hpp"
#include "adm/common_definitions.hpp"
#include "adm/errors.hpp"
#include "adm/private/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
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
    
    SadmXmlParser::SadmXmlParser(rapidxml::file<> file, ParserOptions options,
                         std::shared_ptr<Frame> destFrame)
        : BaseXmlParser(file, options) {
          xmlFile_ = std::move(file);
          frame_ = destFrame;
          idMap_ = *destFrame;
        }

    std::shared_ptr<Frame> SadmXmlParser::parse() {
      rapidxml::xml_document<> xmlDocument;
      xmlDocument.parse<0>(xmlFile_.data());

      if (!xmlDocument.first_node())
        throw error::XmlParsingError("xml document is empty");

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
            NodePtr afe = findAudioFormatExtendedNodeFullRecursive(node);
            if (afe) {
              // add ADM elements to ADM document
              parseAudioFormatExtended<std::shared_ptr<Frame>>(afe, frame_);
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
        
      FrameHeader frameHeader(ff_start, ff_duration, ff_type, ff_id);
      FrameFormat frameFormat = frameHeader.frameFormat();
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

  }  // namespace xml
}  // namespace adm
