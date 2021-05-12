#pragma once
#include "adm/detail/magic_base.hpp"
#include "adm/elements/gain.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/time.hpp"

namespace adm {
  namespace detail {
    template <>
    inline Importance getDefault<Importance>() {
      return Importance{10};
    }

    template <>
    inline Gain getDefault<Gain>() {
      return Gain::fromLinear(1.0);
    }

    extern template class DefaultedBase<Gain>;

    extern template class DefaultedBase<Importance>;

    extern template class DefaultedBase<Rtime>;
    extern template class OptionalBase<Duration>;
  }  // namespace detail
}  // namespace adm
