#pragma once
#include <boost/rational.hpp>
#include "../elements/time.hpp"

namespace adm {
  // utilities for working with times as boost::rational
  using RationalTime = boost::rational<int64_t>;

  inline RationalTime asRational(const FractionalTime &t) {
    return {t.numerator(), t.denominator()};
  }

  inline RationalTime asRational(const Time &t) {
    FractionalTime fractional = t.asFractional();
    return {fractional.numerator(), fractional.denominator()};
  }

  inline FractionalTime asFractionalTime(const RationalTime &t) {
    return {t.numerator(), t.denominator()};
  }

  inline Time asTime(const RationalTime &t) { return asFractionalTime(t); }
}  // namespace adm
