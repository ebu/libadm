#pragma once
#include <adm/elements/time.hpp>
#include <adm/elements/jump_position.hpp>
#include <adm/elements/gain.hpp>
#include <adm/elements/label.hpp>

namespace adm {
  std::ostream& operator<<(std::ostream& stream, const FractionalTime& time) {
    return stream << "FractionalTime{" << time.numerator() << ", "
                  << time.denominator() << "}";
  }

  // required because catch2 stringify doesn't know about boost::variants
  std::ostream& operator<<(std::ostream& stream,
                           const std::chrono::nanoseconds& time) {
    return stream << "nanoseconds{" << time.count() << "}";
  }

  std::ostream& operator<<(std::ostream& stream, const Time& time) {
    return boost::apply_visitor(
        [&](const auto& t) -> std::ostream& { return stream << t; },
        time.asVariant());
  }

  std::ostream& operator<<(std::ostream& stream,
                           const InterpolationLength& interpolationLength) {
    return stream << interpolationLength.get();
  }

  std::ostream& operator<<(std::ostream& stream, const Gain& gain) {
    if (gain.isDb()) {
      stream << gain.asDb() << " dB";
    } else {
      stream << gain.asLinear();
    }
    return stream;
  }

  std::ostream& operator<<(std::ostream& os, const Label& label) {
    label.print(os);
    return os;
  }
}  // namespace adm
