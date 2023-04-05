#pragma once
#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/errors.hpp"
#include "adm/parse.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "adm/elements/audio_pack_format_hoa.hpp"
//#include "adm/detail/id_map.hpp"
#include "adm/private/base_xml_parser.hpp"

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

    class XmlParser : public BaseXmlParser {
     public:
      explicit XmlParser(
          const std::string& filename,
          ParserOptions options = ParserOptions::none,
          std::shared_ptr<Document> destDocument = Document::create());

      explicit XmlParser(
          std::istream& stream, ParserOptions options = ParserOptions::none,
          std::shared_ptr<Document> destDocument = Document::create());

      std::shared_ptr<Document> parse();

     private:
      explicit XmlParser(
          rapidxml::file<> file, ParserOptions options = ParserOptions::none,
          std::shared_ptr<Document> destDocument = Document::create());

      /// add an element to both the document and idMap_
      template <typename Element>
      void add(std::shared_ptr<Element> el);

      std::shared_ptr<Document> document_;
    };

  }  // namespace xml
}  // namespace adm
