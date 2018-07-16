#include "adm/serialized/frame_format_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

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
    return value_ != boost::none;
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

  bool FrameFormatId::operator<(const FrameFormatId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void FrameFormatId::print(std::ostream& os) const {
    os << boost::str(boost::format("FF_%1%") %
                     detail::formatHexValue(get<FrameFormatIdValue>().get()));
  }

  FrameFormatId parseFrameFormatId(const std::string& id) {
    const std::regex r("FF_([0-9]{11})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto value = detail::parseHexValue(idMatch[1], 4);
      return FrameFormatId(FrameFormatIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid FrameFormatId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(FrameFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
