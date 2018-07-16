#pragma once

#include "adm/detail/named_type.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/serialized_fwd.hpp"

namespace adm {

#define ADD_TRAIT(TYPE, TAG)       \
  namespace detail {               \
    template <>                    \
    struct ParameterTraits<TYPE> { \
      struct TAG {};               \
      typedef TAG tag;             \
    };                             \
  }
  namespace detail {
    template <typename T>
    struct ParameterTraits {
      typedef typename T::tag tag;
    };

    /**
     * NOTE: These template specializations are necessary because the
     * AudioTrackFormat and the AudioStreamFormat reference each other.
     */
    template <>
    struct ParameterTraits<AudioTrackFormat> {
      typedef AudioTrackFormatTag tag;
    };

    template <>
    struct ParameterTraits<AudioStreamFormat> {
      typedef AudioStreamFormatTag tag;
    };
  }  // namespace detail
}  // namespace adm
