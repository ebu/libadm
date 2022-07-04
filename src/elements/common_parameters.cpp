#include "adm/elements/common_parameters.hpp"

namespace adm {
  namespace detail {
    template class RequiredParameter<AudioBlockFormatId>;

    template class DefaultParameter<Gain>;

    template class DefaultParameter<Importance>;

    template class DefaultParameter<Rtime>;
    template class OptionalParameter<Duration>;

    template class DefaultParameter<HeadphoneVirtualise>;

    template class DefaultParameter<HeadLocked>;

    template class DefaultParameter<ScreenRef>;
  }  // namespace detail
}  // namespace adm
