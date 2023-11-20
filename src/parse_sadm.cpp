#include "adm/parse_sadm.hpp"
#include "adm/parse.hpp"
#include <string>
#include "adm/serial/frame_header_parser.hpp"

namespace adm {
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
