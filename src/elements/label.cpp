#include "adm/elements/label.hpp"

namespace adm {


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
