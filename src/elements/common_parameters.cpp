#include "adm/elements/private/common_parameters.hpp"

namespace adm {
  namespace detail {
    template class DefaultedBase<Gain>;

    template class DefaultedBase<Importance>;

    template class DefaultedBase<Rtime>;
    template class OptionalBase<Duration>;
  }  // namespace detail
}  // namespace adm
