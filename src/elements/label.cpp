#include "adm/elements/label.hpp"

namespace adm {
  Label::Label(const char* s) {
    // to avoid UB from std::string
    if (!s) {
      throw error::AdmGenericRuntimeError{
          "Cannot construct Label from null const char*"};
    }
    set(LabelValue{std::string{s}});
  }

  void adm::Label::print(std::ostream& os) const {
    os << get<LabelValue>().get();
    if (has<LabelLanguage>()) {
      os << " (language= " << get<LabelLanguage>().get() << ")";
    }
  }

  namespace detail {
    template class RequiredParameter<LabelValue>;
    template class OptionalParameter<LabelLanguage>;
    template class VectorParameter<Labels>;
  }  // namespace detail
}  // namespace adm
