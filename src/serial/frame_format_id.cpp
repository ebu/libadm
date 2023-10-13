#include "adm/serial/frame_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Operators ---- //
  bool FrameFormatId::operator==(const FrameFormatId& other) const {
    bool equal{get<FrameIndex>() == other.get<FrameIndex>()};
    equal = equal && (has<ChunkIndex>() == other.has<ChunkIndex>());
    if (equal && has<ChunkIndex>()) {
      equal = (get<ChunkIndex>() == other.get<ChunkIndex>());
    }
    return equal;
  }

  bool FrameFormatId::operator!=(const FrameFormatId& other) const {
    return !(*this == other);
  }

  bool FrameFormatId::operator<(const FrameFormatId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void FrameFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace {
    bool is_valid_frame_format_id_size(std::string const& id) {
      return id.size() == 11 || id.size() == 14;
    }

    void validate_frame_format_id(detail::IDParser const& parser,
                                  const std::string& id) {
      // According to BS.2125-1 A 1.5.1 / Table 19
      // FF_xxxxxxxx for non-divided frames
      // FF_xxxxxxxx_zz for dividec frames
      if (!is_valid_frame_format_id_size(id)) {
        throw std::runtime_error(
            std::string("frameFormatID \"").append(id) +
            "\" is not the correct length, it should be 11 chars long in the "
            "format "
            "FF_xxxxxxxx or 14 chars long in the format FF_xxxxxxxx_zz");
      }
      if (id.size() == 14 && id.at(11) != '_') {
        throw std::runtime_error(
            std::string("frameFormatID \"").append(id) +
            "\""
            " is 14 characters long so should be in the format FF_xxxxxxxx_zz, "
            "but character 11 is not \'_\'");
      }
      parser.check_prefix("FF_", 3);
    }
  }  // namespace

  FrameFormatId parseFrameFormatId(const std::string& id) {
    detail::IDParser parser("FrameFormatId", id);
    validate_frame_format_id(parser, id);
    FrameIndex index{parser.parse_hex(3, 8)};
    if (id.size() == 14) {
      return FrameFormatId(index, ChunkIndex{parser.parse_hex(12, 2)});
    } else {
      return FrameFormatId(index);
    }
  }

  std::string formatId(FrameFormatId id) {
    std::string s{};
    s.reserve(14);
    if (id.has<ChunkIndex>()) {
      s = "FF_zzzzzzzz_zz";
      detail::formatHex(s, 3, 8, id.get<FrameIndex>().get());
      detail::formatHex(s, 12, 2, id.get<ChunkIndex>().get());
    } else {
      s = "FF_zzzzzzzz";
      detail::formatHex(s, 3, 8, id.get<FrameIndex>().get());
    }
    return s;
  }

}  // namespace adm
