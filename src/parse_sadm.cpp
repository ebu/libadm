#include "adm/parse_sadm.hpp"
#include <memory>
#include <string>
#include "adm/common_definitions.hpp"
#include "adm/serial/sadm_xml_parser.hpp"

namespace adm {
  std::shared_ptr<Frame> parseSadmXml(std::istream& stream,
                                      xml::ParserOptions options) {
    auto commonDefinitions = getSadmCommonDefinitions();
    xml::SadmXmlParser parser(stream, options, commonDefinitions);
    return parser.parse();
  }
  
  std::shared_ptr<Frame> parseSadmXmlWithCommonDefs(std::istream& stream,
                                      std::shared_ptr<Frame> cd_frame,
                                      xml::ParserOptions options) {
    xml::SadmXmlParser parser(stream, options, cd_frame);
    return parser.parse();
  }
}  // namespace adm
