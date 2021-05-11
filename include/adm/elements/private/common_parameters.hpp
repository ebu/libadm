#pragma once
#include "adm/detail/magic_base.hpp"
#include "adm/elements/gain.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/time.hpp"
#include "adm/export.h"

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

    extern template class ADM_EXPORT DefaultedBase<Gain>;

    extern template class ADM_EXPORT DefaultedBase<Importance>;

    extern template class ADM_EXPORT DefaultedBase<Rtime>;
    extern template class ADM_EXPORT OptionalBase<Duration>;
  }  // namespace detail
}  // namespace adm
