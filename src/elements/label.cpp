#include "adm/elements/label.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>

namespace adm {
  // ---- Getter ---- //
  LabelLanguage Label::get(detail::ParameterTraits<LabelLanguage>::tag) const {
    return this->language_.get();
  }
  LabelValue Label::get(detail::ParameterTraits<LabelValue>::tag) const {
    return this->value_.get();
  }

  // ---- Has ---- //
  bool Label::has(detail::ParameterTraits<LabelLanguage>::tag) const {
    return this->language_ != boost::none;
  }
  bool Label::has(detail::ParameterTraits<LabelValue>::tag) const {
    return this->value_ != boost::none;
  }

  // ---- Setter ---- //
  void Label::set(LabelValue value) { value_ = value; }
  void Label::set(LabelLanguage language) { language_ = language; }

  // ---- Unsetter ---- //
  void Label::unset(detail::ParameterTraits<LabelValue>::tag) {
    value_ = boost::none;
  }
  void Label::unset(detail::ParameterTraits<LabelLanguage>::tag) {
    language_ = boost::none;
  }

  // ---- Operators ---- //
  bool Label::operator==(const Label& other) const {
    return get<LabelValue>() == other.get<LabelValue>() &&
           get<LabelLanguage>() == other.get<LabelLanguage>();
  }

  bool Label::operator!=(const Label& other) const { return !(*this == other); }

  // TODO don't have an Id to compare here - does it make sense to have a <
  // operator?

  // bool Label::operator<(const Label&
  // other) const {
  //   return formatId(*this) < formatId(other);
  // }

  // ---- Common ---- //
  // TODO what should the format be for a value string and a language string?
  /*
  void Label::print(std::ostream& os) const {
    os << boost::str(
        boost::format("value = %2% lang = %1%") %
        has<LabelValue>() ? get<LabelValue> : "" %
        has<LabelLanguage>() ? get<LabelLanguage> :
  "" %
    );
  }
  */

}  // namespace adm
