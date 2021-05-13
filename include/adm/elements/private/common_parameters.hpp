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

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultedBase<Gain>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultedBase<Importance>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultedBase<Rtime>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS OptionalBase<Duration>;
  }  // namespace detail
}  // namespace adm
