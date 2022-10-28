#include "adm/parse.hpp"
#include <memory>
#include <string>
#include "adm/common_definitions.hpp"
#include "adm/private/document_parser.hpp"
#include "adm/serial/frame_header_parser.hpp"

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

  FrameHeader parseFrameHeader(std::istream& stream,
                               xml::ParserOptions options) {
    xml::FrameHeaderParser parser(stream, options);
    return parser.parse();
  }

  FrameHeader parseFrameHeader(std::string const& fileName,
                               xml::ParserOptions options) {
    xml::FrameHeaderParser parser(fileName, options);
    return parser.parse();
  }
}  // namespace adm
