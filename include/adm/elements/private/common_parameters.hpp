#pragma once
#include "adm/detail/auto_base.hpp"
#include "adm/elements/gain.hpp"
#include "adm/elements/headphone_virtualise.hpp"
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

    template <>
    inline Rtime getDefault<Rtime>() {
      return Rtime{std::chrono::nanoseconds{0}};
    }

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Gain>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<Importance>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Rtime>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<Duration>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<HeadphoneVirtualise>;
  }  // namespace detail
}  // namespace adm
