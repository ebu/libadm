#include "adm/parse.hpp"
#include <memory>
#include <string>
#include "adm/common_definitions.hpp"
#include "adm/private/document_parser.hpp"

namespace adm {
  class FrameHeader;
  std::shared_ptr<Document> parseXml(const std::string& filename,
                                     xml::ParserOptions options) {
    auto commonDefinitions = getCommonDefinitions();
    xml::DocumentParser parser(filename, options, commonDefinitions);
    return parser.parse();
  }

  std::shared_ptr<Document> parseXml(std::istream& stream,
                                     xml::ParserOptions options) {
    auto commonDefinitions = getCommonDefinitions();
    xml::DocumentParser parser(stream, options, commonDefinitions);
    return parser.parse();
  }
  std::shared_ptr<Document> parseXml(const std::string& filename,
                                     const FrameHeader& header,
                                     xml::ParserOptions options) {
    auto commonDefinitions = getCommonDefinitions();
    xml::DocumentParser parser(filename, options, commonDefinitions);
    parser.setHeader(header);
    return parser.parse();
  }
  std::shared_ptr<Document> parseXml(std::istream& stream,
                                     const FrameHeader& header,
                                     xml::ParserOptions options) {
    auto commonDefinitions = getCommonDefinitions();
    xml::DocumentParser parser(stream, options, commonDefinitions);
    parser.setHeader(header);
    return parser.parse();
  }
}  // namespace adm
