/// @file time.hpp
#pragma once

#include <chrono>
#include <stdexcept>
#include <string>
#include <boost/variant.hpp>
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

#include <iostream>

namespace adm {

  /// @brief storage for ADM times of format 'hh:mm:ss.zzzzzSfffff'
  class FractionalTime {
   public:
    FractionalTime(int64_t numerator, int64_t denominator)
        : _numerator(numerator), _denominator(denominator) {
      if (denominator < 1)
        throw std::invalid_argument(
            "FractionalTime denominator must be positive");
    }

    int64_t numerator() const { return _numerator; }
    int64_t denominator() const { return _denominator; }

    bool operator==(const FractionalTime& other) const {
      return numerator() == other.numerator() &&
             denominator() == other.denominator();
    }

    bool operator!=(const FractionalTime& other) const {
      return !(*this == other);
    }

    /// normalised fraction, such that numerator and denominator have no common
    /// factors
    ADM_EXPORT FractionalTime normalised() const;

   private:
    int64_t _numerator;
    int64_t _denominator;
  };

  /// representation of ADM times; this can either be decimal times,
  /// represented as nanoseconds, or fractional times, represented as a
  /// FractionalTime
  class Time {
   public:
    // non-explicit, as this should act like a variant
    template <typename Rep, typename Period>
    // NOLINTNEXTLINE(google-explicit-constructor)
    Time(const std::chrono::duration<Rep, Period>& time) : time(time) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    Time(const FractionalTime& time) : time(time) {}
    Time() : time(std::chrono::nanoseconds::zero()) {}

    /// convert to nanoseconds, rounding down
    ADM_EXPORT std::chrono::nanoseconds asNanoseconds() const;
    ADM_EXPORT FractionalTime asFractional() const;

    bool isNanoseconds() const {
      return time.type() == typeid(std::chrono::nanoseconds);
    }
    bool isFractional() const { return time.type() == typeid(FractionalTime); };

    using Variant = boost::variant<std::chrono::nanoseconds, FractionalTime>;
    const Variant& asVariant() const { return time; }

    bool operator==(const Time& other) const { return time == other.time; }
    bool operator!=(const Time& other) const { return time != other.time; }

   private:
    Variant time;
  };

  /// @brief Tag for NamedType ::Start
  struct StartTag {};
  /// @brief NamedType for the start attribute
  using Start = detail::NamedType<Time, StartTag>;
  /// @brief Tag for NamedType ::End
  struct EndTag {};
  /// @brief NamedType for the end attribute
  using End = detail::NamedType<Time, EndTag>;
  /// @brief Tag for NamedType ::Duration
  struct DurationTag {};
  /// @brief NamedType for the duration attribute
  using Duration = detail::NamedType<Time, DurationTag>;
  /// @brief Tag for NamedType ::Rtime
  struct RtimeTag {};
  /// @brief NamedType for the rtime attribute
  using Rtime = detail::NamedType<Time, RtimeTag>;
  /// @brief Tag for NamedType ::InitializeBlock
  struct InitializeBlockTag {};
  /// @brief NamedType for the InitializeBlock attribute
  using InitializeBlock = detail::NamedType<bool, InitializeBlockTag>;

  enum class TimeReference { TOTAL, LOCAL };
  /// @brief Tag for NamedType ::TimeReference
  struct TimeReferenceTag {};

  namespace detail {
    template <>
    struct ParameterTraits<TimeReference> {
      using tag = TimeReferenceTag;
    };
  }  // namespace detail

  namespace detail {
    template <>
    ADM_EXPORT TimeReference getDefault<TimeReference>();
  }

  /// @brief Parse an adm timecode and convert it to a std::chrono::duration
  ADM_EXPORT Time parseTimecode(const std::string& timecode);
  /// @brief Format a std::chrono::duration object as an adm timecode string
  ADM_EXPORT std::string formatTimecode(const Time& time);
}  // namespace adm
