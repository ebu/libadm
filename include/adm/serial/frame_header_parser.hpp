#pragma once
#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/errors.hpp"
#include "adm/parse_sadm.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "adm/serial/frame_format.hpp"
#include "adm/serial/frame_format_id.hpp"
#include "adm/serial/transport_track_format.hpp"
#include "adm/serial/transport_id.hpp"
#include "adm/serial/audio_track.hpp"

#include <iostream>

namespace adm {
  /**
   * @brief XML parsing implementation
   *
   * This namespace collects parser implementations and helpers for writing
   * parsers.
   *
   * If you're just looking for a way to parse an XML file in client code,
   * refer to `adm::parseXml()`
   */
  namespace xml {
    using NodePtr = rapidxml::xml_node<>*;

    class FrameHeaderParser {
     public:
      FrameHeaderParser(const std::string& filename,
                        ParserOptions options = ParserOptions::none);

      explicit FrameHeaderParser(std::istream& stream,
                                 ParserOptions options = ParserOptions::none);

      FrameHeader parse();

     private:
      explicit FrameHeaderParser(rapidxml::file<> file,
                                 ParserOptions options = ParserOptions::none);
      /// add an element to both the document and idMap_
      template <typename Element>
      void add(std::shared_ptr<Element> el);

      NodePtr findFrameNode(NodePtr root);
      FrameHeader parseFrameHeader(NodePtr node);

      rapidxml::file<> file;
      ParserOptions options;
    };

  }  // namespace xml
}  // namespace adm
