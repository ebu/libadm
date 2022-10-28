#include "adm/serial/frame_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Defaults ---- //
  const FrameFormatIdValue FrameFormatId::valueDefault_ = FrameFormatIdValue(0);

  // ---- Getter ---- //
  FrameFormatIdValue FrameFormatId::get(
      detail::ParameterTraits<FrameFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool FrameFormatId::has(
      detail::ParameterTraits<FrameFormatIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool FrameFormatId::isDefault(
      detail::ParameterTraits<FrameFormatIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void FrameFormatId::set(FrameFormatIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void FrameFormatId::unset(detail::ParameterTraits<FrameFormatIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool FrameFormatId::operator==(const FrameFormatId& other) const {
    return get<FrameFormatIdValue>() == other.get<FrameFormatIdValue>();
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

  FrameFormatId parseFrameFormatId(const std::string& id) {
    // FF_zzzzzzzzzzz
    detail::IDParser parser("FrameFormatId", id);
    parser.check_size(14);
    parser.check_prefix("FF_", 3);
    auto value = parser.parse_hex(3, 11);
    parser.check_underscore(11);
    auto counter = parser.parse_hex(12, 8);
    return FrameFormatId(FrameFormatIdValue(value));
  }

  std::string formatId(FrameFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();

    std::string s("FF_zzzzzzzzzzz");
    detail::formatHex(s, 3, 11, id.get<FrameFormatIdValue>().get());
    return s;
  }

}  // namespace adm
