#include "adm/elements/private/common_parameters.hpp"

namespace adm {
  namespace detail {
    template class DefaultParameter<Gain>;

    template class DefaultParameter<Importance>;

    template class DefaultParameter<Rtime>;
    template class OptionalParameter<Duration>;

    template class DefaultParameter<HeadphoneVirtualise>;

    template class DefaultParameter<HeadLocked>;
  }  // namespace detail
}  // namespace adm
