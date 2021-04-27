#pragma once
#include "adm/detail/auto_base.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/time.hpp"

namespace adm {
  /// @brief Tag for NamedType ::Gain
  struct GainTag {};
  /// @brief NamedType for gain parameter
  using Gain = detail::NamedType<float, GainTag>;

  namespace detail {
    template <>
    inline Importance getDefault<Importance>() {
      return Importance{10};
    }

    template <>
    inline Gain getDefault<Gain>() {
      return Gain{1.0f};
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
  }  // namespace detail
}  // namespace adm
