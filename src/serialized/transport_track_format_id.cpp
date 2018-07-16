#include "adm/serialized/transport_track_format_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TransportIdValue TransportId::valueDefault_ = TransportIdValue(0);

  // ---- Getter ---- //
  TransportIdValue TransportId::get(
      detail::ParameterTraits<TransportIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool TransportId::has(detail::ParameterTraits<TransportIdValue>::tag) const {
    return value_ != boost::none;
  }

  // ---- isDefault ---- //
  bool TransportId::isDefault(
      detail::ParameterTraits<TransportIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void TransportId::set(TransportIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void TransportId::unset(detail::ParameterTraits<TransportIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool TransportId::operator==(const TransportId& other) const {
    return get<TransportIdValue>() == other.get<TransportIdValue>();
  }

  bool TransportId::operator<(const TransportId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void TransportId::print(std::ostream& os) const {
    os << boost::str(boost::format("TP_%1%") %
                     detail::formatHexValue(get<TransportIdValue>().get()));
  }

  TransportId parseTransportId(const std::string& id) {
    const std::regex r("TP_([0-9]{4})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto value = detail::parseHexValue(idMatch[1], 4);
      return TransportId(TransportIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid TransportId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(TransportId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
