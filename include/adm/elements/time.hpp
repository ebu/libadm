/// @file time.hpp
#pragma once

#include <chrono>
#include <string>
#include <boost/variant.hpp>
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief storage for ADM times of format 'hh:mm:ss.zzzzzSfffff'
  struct FractionalTime {
    int64_t numerator;
    int64_t denominator;

    bool operator==(const FractionalTime& other) const {
      return numerator == other.numerator && denominator == other.denominator;
    }

    bool operator!=(const FractionalTime& other) const {
      return !(*this == other);
    }

    /// normalised fraction, such that numerator and denominator have no common
    /// fractors
    ADM_EXPORT FractionalTime normalised() const;
  };

  /// representation of ADM times; this can either be decimal times,
  /// represented as nanoseconds, or fractional times, represented as a
  /// FractionalTime
  class Time {
   public:
    Time(const std::chrono::nanoseconds& time) : time(time) {}
    Time(const FractionalTime& time) : time(time) {}

    /// convert to nanoseconds, rounding down
    ADM_EXPORT std::chrono::nanoseconds asNanoseconds() const;
    ADM_EXPORT FractionalTime asFractional() const;

    bool isNanoseconds() const {
      return time.type() == typeid(std::chrono::nanoseconds);
    }
    bool isFractional() const { return time.type() == typeid(FractionalTime); };

    using Variant = boost::variant<std::chrono::nanoseconds, FractionalTime>;
    Variant asVariant() const { return time; }

    bool operator==(const Time& other) const { return time == other.time; }
    bool operator!=(const Time& other) const { return time != other.time; }

   private:
    Variant time;
  };

  /// @brief Tag for NamedType ::Start
  struct StartTag {};
  /// @brief NamedType for the start attribute
  using Start = detail::NamedType<std::chrono::nanoseconds, StartTag>;
  /// @brief Tag for NamedType ::End
  struct EndTag {};
  /// @brief NamedType for the end attribute
  using End = detail::NamedType<std::chrono::nanoseconds, EndTag>;
  /// @brief Tag for NamedType ::Duration
  struct DurationTag {};
  /// @brief NamedType for the duration attribute
  using Duration = detail::NamedType<std::chrono::nanoseconds, DurationTag>;
  /// @brief Tag for NamedType ::Rtime
  struct RtimeTag {};
  /// @brief NamedType for the rtime attribute
  using Rtime = detail::NamedType<std::chrono::nanoseconds, RtimeTag>;

  /// @brief Parse an adm timecode and convert it to a std::chrono::duration
  ADM_EXPORT std::chrono::nanoseconds parseTimecode(
      const std::string& timecode);
  /// @brief Format a std::chrono::duration object as an adm timecode string
  ADM_EXPORT std::string formatTimecode(const std::chrono::nanoseconds& time);

  /// @brief Parse an adm timecode and convert it to a std::chrono::duration
  ADM_EXPORT Time parseFractionalTimecode(const std::string& timecode);
  /// @brief Format a std::chrono::duration object as an adm timecode string
  ADM_EXPORT std::string formatFractionalTimecode(const Time& time);
}  // namespace adm
