#pragma once
#include "adm/detail/auto_base.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/gain.hpp"
#include "adm/elements/headphone_virtualise.hpp"
#include "adm/elements/head_locked.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/nfc_ref_dist.hpp"
#include "adm/elements/normalization.hpp"
#include "adm/elements/screen_ref.hpp"
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

    template <>
    inline HeadLocked getDefault<HeadLocked>() {
      return HeadLocked{false};
    }

    template <>
    inline ScreenRef getDefault<ScreenRef>() {
      return ScreenRef{false};
    }

    template <>
    inline Normalization getDefault<Normalization>() {
      return Normalization{"SN3D"};
    }

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<AudioBlockFormatId>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Gain>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<Importance>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Rtime>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<Duration>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<Lstart>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<Lduration>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<InitializeBlock>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<HeadphoneVirtualise>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<HeadLocked>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<ScreenRef>;

    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<Normalization>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<NfcRefDist>;
  }  // namespace detail

  /// @brief Tag for NamedType ::Cartesian
  struct CartesianTag {};
}  // namespace adm
