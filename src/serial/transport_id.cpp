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
      static constexpr char const* format{"TP_xxxx"};
      static constexpr std::size_t sections{1};
    };
    template <>
    struct IdSection<TransportId, 0> {
      using type = TransportIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  TransportId parseTransportId(const std::string& id) {
    detail::IDParser<TransportId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(TransportId id) { return detail::formatId(id); }

}  // namespace adm
