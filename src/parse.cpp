#include "adm/parse.hpp"
#include <memory>
#include <string>
#include "adm/common_definitions.hpp"
#include "adm/private/xml_parser.hpp"

namespace adm {

  std::shared_ptr<Document> parseXml(const std::string& filename,
                                     xml::ParserOptions options) {
    std::shared_ptr<Document> doc = getCommonDefinitions();

    xml::XmlParser parser(doc, options);
    parser.parseFile(filename);

    return doc;
  }

  std::shared_ptr<Document> parseXml(std::istream& stream,
                                     xml::ParserOptions options) {
    std::shared_ptr<Document> doc = getCommonDefinitions();

    xml::XmlParser parser(doc, options);
    parser.parseStream(stream);

    return doc;
  }
}  // namespace adm
