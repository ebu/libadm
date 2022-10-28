#include "adm/elements/common_parameters.hpp"

namespace adm {
  namespace detail {
    template class RequiredParameter<AudioBlockFormatId>;

    template class DefaultParameter<Gain>;

    template class DefaultParameter<Importance>;

    template class DefaultParameter<Rtime>;
    template class OptionalParameter<Duration>;
    template class OptionalParameter<Lstart>;
    template class OptionalParameter<Lduration>;
    template class OptionalParameter<InitializeBlock>;

    template class DefaultParameter<HeadphoneVirtualise>;

    template class DefaultParameter<HeadLocked>;

    template class DefaultParameter<ScreenRef>;

    template class DefaultParameter<Normalization>;
    template class DefaultParameter<NfcRefDist>;
  }  // namespace detail
}  // namespace adm
