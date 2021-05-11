#include "adm/elements/private/common_parameters.hpp"

namespace adm {
  namespace detail {
    template class DefaultedBase<Gain>;

    template class DefaultedBase<Importance>;

    template class DefaultedBase<Rtime>;
    template class OptionalBase<Duration>;

    template <>
    ADM_EXPORT const Gain DefaultParameter<Gain>::value{Gain::fromLinear(1.0)};
    template <>
    ADM_EXPORT const Importance DefaultParameter<Importance>::value{10};
    template <>
    ADM_EXPORT const Rtime DefaultParameter<Rtime>::value{
        std::chrono::seconds(0)};
  }  // namespace detail
}  // namespace adm
