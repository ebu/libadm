#pragma once
#include "adm/detail/magic_base.hpp"
#include "adm/elements/gain.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/time.hpp"
#include "adm/export.h"

namespace adm {
  namespace detail {
    extern template class ADM_EXPORT DefaultedBase<Gain>;

    extern template class ADM_EXPORT DefaultedBase<Importance>;

    extern template class ADM_EXPORT DefaultedBase<Rtime>;
    extern template class ADM_EXPORT OptionalBase<Duration>;
  }  // namespace detail
}  // namespace adm
