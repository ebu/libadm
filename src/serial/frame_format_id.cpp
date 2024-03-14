#include "adm/serial/frame_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

  // ---- Operators ---- //
  bool FrameFormatId::operator==(const FrameFormatId& other) const {
    return detail::optionalsEqual<FrameIndex, ChunkIndex>(*this, other);
  }

  bool FrameFormatId::operator!=(const FrameFormatId& other) const {
    return !(*this == other);
  }

  bool FrameFormatId::operator<(const FrameFormatId& other) const {
    return detail::optionalsLess<FrameIndex, ChunkIndex>(*this, other);
  }

  // ---- Common ---- //
  void FrameFormatId::print(std::ostream& os) const { os << formatId(*this); }

  namespace {
    bool is_valid_frame_format_id_size(std::string const& id) {
      return id.size() == 11 || id.size() == 14;
    }

    struct ShortFrameFormatId {};
    struct LongFrameFormatId {};
  }  // namespace

  namespace detail {
    template <>
    struct IdTypeFor<ShortFrameFormatId> {
      using type = FrameFormatId;
    };
    template <>
    struct IdTypeFor<LongFrameFormatId> {
      using type = FrameFormatId;
    };
    template <>
    struct IdTraits<ShortFrameFormatId> {
      static constexpr char const* name{"frameFormatID (short format)"};
      static constexpr char const* format{"FF_xxxxxxxx"};
      static constexpr std::size_t sections{1u};
    };
    template <>
    struct IdSection<ShortFrameFormatId, 0> {
      using type = FrameIndex;
      static constexpr char identifier{'x'};
    };
    template <>
    struct IdTraits<LongFrameFormatId> {
      static constexpr char const* name{"frameFormatID (long format)"};
      static constexpr char const* format{"FF_xxxxxxxx_zz"};
      static constexpr std::size_t sections{2u};
    };
    template <>
    struct IdSection<LongFrameFormatId, 0> {
      using type = FrameIndex;
      static constexpr char identifier{'x'};
    };
    template <>
    struct IdSection<LongFrameFormatId, 1> {
      using type = ChunkIndex;
      static constexpr char identifier{'z'};
    };
  }  // namespace detail

  FrameFormatId parseFrameFormatId(const std::string& id) {
    // Slightly less helpful error message as we do this before prefix check,
    // but otherwise we don't know which format to validate against
    if (!is_valid_frame_format_id_size(id)) {
      throw std::runtime_error(
          std::string("frameFormatID \"").append(id) +
          "\" is not the correct length, it should be 11 chars long in the "
          "format "
          "FF_xxxxxxxx or 14 chars long in the format FF_xxxxxxxx_zz");
    }
    if (id.size() == 14) {
      detail::IDParser<LongFrameFormatId> parser{id};
      parser.validate();
      return parser.parse();
    } else {
      detail::IDParser<ShortFrameFormatId> parser{id};
      parser.validate();
      return parser.parse();
    }
  }

  std::string formatId(FrameFormatId id) {
    std::string s{};
    if (id.has<ChunkIndex>()) {
      return detail::formatId<LongFrameFormatId>(id);
    } else {
      return detail::formatId<ShortFrameFormatId>(id);
    }
  }

}  // namespace adm
