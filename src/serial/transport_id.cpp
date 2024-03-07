#include "adm/serial/transport_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

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
    return true;
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
    return detail::optionalsEqual<TransportIdValue>(*this, other);
  }

  bool TransportId::operator!=(const TransportId& other) const {
    return !(*this == other);
  }

  bool TransportId::operator<(const TransportId& other) const {
    return detail::optionalsLess<TransportIdValue>(*this, other);
  }

  // ---- Common ---- //
  void TransportId::print(std::ostream& os) const { os << formatId(*this); }

  namespace detail {
    template <>
    struct IdTraits<TransportId> {
      static constexpr char const* name{"transportID"};
      static constexpr char const* prefix{"TP_"};
      static constexpr char const* format{"TP_xxxx"};
    };
  }  // namespace detail

  TransportId parseTransportId(const std::string& id) {
    // TP_xxxx
    detail::IDParser parser{id};
    parser.check_size<TransportId>();
    parser.check_prefix<TransportId>();
    auto value = parser.parse_hex<TransportId>(3, 4);
    return TransportId(TransportIdValue(value));
  }

  std::string formatId(TransportId id) {
    std::string s("TP_xxxx");
    detail::formatHex(s, 3, 4, id.get<TransportIdValue>().get());
    return s;
  }

}  // namespace adm
