#include "adm/serial/sadm_xml_reader.hpp"
#include <memory>
#include <string>
#include "adm/serial/sadm_xml_parser.hpp"

namespace adm {

  std::shared_ptr<Frame> parseSadmXml(std::istream& stream,
                                      xml::ParserOptions /*options*/) {
    xml::SadmXmlParser parser(stream);
    return parser.parse();
  }
}  // namespace adm
