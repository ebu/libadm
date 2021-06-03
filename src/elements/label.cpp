#include "adm/elements/label.hpp"

namespace adm {
  namespace detail {
    template class RequiredParameter<LabelValue>;
    template class OptionalParameter<LabelLanguage>;
    template class VectorParameter<Labels>;
  }  // namespace detail
}  // namespace adm
