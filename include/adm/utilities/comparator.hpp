#pragma once

#include <algorithm>
#include "adm/elements/time.hpp"

namespace adm {

  /**
   * @brief Compare `AudioBlockFormats` by Rtime
   * @headerfile comparator.hpp <adm/utilities/comparator.hpp>
   *
   * This is a binary predicate that returns true if
   * the `adm::Rtime` of one `AudioBlockFormat` is less than the `Rtime`
   * of another one.
   *
   * Thus this struct can be used with STL algorithms for sorting/sorted
   * AudioBlockFormats containers.
   *
   * @code
     std::vector<adm::AudioBlockFormatObjects> blockFormats;

     // ...

     std::sort(blockFormats.begin(),
               blockFormats.end(),
               adm::CompareRtimeLess());
     @endcode
   */
  struct CompareRtimeLess {
    template <typename AudioBlockFormat>
    bool operator()(const AudioBlockFormat& lhs, const AudioBlockFormat& rhs) {
      if (lhs.template has<Rtime>() && rhs.template has<Rtime>()) {
        return lhs.template get<Rtime>() < rhs.template get<Rtime>();
      }

      return false;
    }
  };

  /**
   * @brief Compare `AudioBlockFormats` by Rtime and Duration
   * @headerfile comparator.hpp <adm/utilities/comparator.hpp>
   *
   * This is a binary predicate that returns true if
   * the `adm::Rtime` of one `AudioBlockFormat` is less than the `Rtime`
   * of another one. If `RTime` is equal, `Duration` has to be taken into
   * consideration
   *
   * Thus this struct can be used with STL algorithms for sorting/sorted
   * AudioBlockFormats containers.
   *
   * @code
     std::vector<adm::AudioBlockFormatObjects> blockFormats;

     // ...

     std::sort(blockFormats.begin(),
               blockFormats.end(),
               adm::CompareRtimeLess());
     @endcode
   */
  struct CompareRtimeDurationLess {
    template <typename AudioBlockFormat>
    bool operator()(const AudioBlockFormat& lhs, const AudioBlockFormat& rhs) {
      if (lhs.template has<Rtime>() && rhs.template has<Rtime>()) {
        if (lhs.template get<Rtime>() == rhs.template get<Rtime>()) {
          if (lhs.template has<Duration>() && rhs.template has<Duration>()) {
            return lhs.template get<Duration>() < rhs.template get<Duration>();
          }
        }

        return lhs.template get<Rtime>() < rhs.template get<Rtime>();
      }

      return false;
    }
  };
}  // namespace adm
