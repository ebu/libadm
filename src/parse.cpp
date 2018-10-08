#include "adm/parse.hpp"
#include <memory>
#include <string>
#include "adm/common_definitions.hpp"
#include "adm/private/xml_parser.hpp"

namespace adm {

  std::shared_ptr<Document> parseXml(const std::string& filename,
                                     xml::ParserOptions options) {
    auto commonDefinitions = getCommonDefinitions();
    xml::XmlParser parser(filename, options, commonDefinitions);
    return parser.parse();
  }

  std::shared_ptr<Document> parseXml(std::istream& stream,
                                     xml::ParserOptions options) {
    auto commonDefinitions = getCommonDefinitions();
    xml::XmlParser parser(stream, options, commonDefinitions);
    return parser.parse();
  }
}  // namespace adm
