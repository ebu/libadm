#include "adm/xml_reader.hpp"
#include <memory>
#include <string>
#include "adm/private/xml_parser.hpp"

namespace adm {

  std::shared_ptr<Document> parseXml(const std::string& filename,
                                     xml::ParserOptions /*options*/) {
    xml::XmlParser parser(filename);
    return parser.parse();
  }

  std::shared_ptr<Document> parseXml(std::istream& stream,
                                     xml::ParserOptions /*options*/) {
    xml::XmlParser parser(stream);
    return parser.parse();
  }
}  // namespace adm
