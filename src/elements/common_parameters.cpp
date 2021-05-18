#include "adm/elements/private/common_parameters.hpp"

namespace adm {
  namespace detail {
    template class DefaultParameter<Gain>;

    template class DefaultParameter<Importance>;

    template class DefaultParameter<Rtime>;
    template class OptionalParameter<Duration>;
  }  // namespace detail
}  // namespace adm
