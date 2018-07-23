#include "adm/elements/frequency.hpp"
#include "adm/utilities/element_io.hpp"

namespace adm {

  // ---- Getter ---- //
  HighPass Frequency::get(detail::ParameterTraits<HighPass>::tag) const {
    return highPassFrequency_.get();
  }
  LowPass Frequency::get(detail::ParameterTraits<LowPass>::tag) const {
    return lowPassFrequency_.get();
  }

  // ---- Has ---- //
  bool Frequency::has(detail::ParameterTraits<HighPass>::tag) const {
    return highPassFrequency_ != boost::none;
  }
  bool Frequency::has(detail::ParameterTraits<LowPass>::tag) const {
    return lowPassFrequency_ != boost::none;
  }

  // ---- Setter ---- //
  void Frequency::set(HighPass frequency) { highPassFrequency_ = frequency; }
  void Frequency::set(LowPass frequency) { lowPassFrequency_ = frequency; }

  // ---- Unsetter ---- //
  void Frequency::unset(detail::ParameterTraits<HighPass>::tag) {
    highPassFrequency_ = boost::none;
  }
  void Frequency::unset(detail::ParameterTraits<LowPass>::tag) {
    lowPassFrequency_ = boost::none;
  }

  void Frequency::print(std::ostream &os) const {
    os << "(";
    if (has<HighPass>()) {
      os << "highPass=" << get<HighPass>() << ", ";
    }
    if (has<LowPass>()) {
      os << "lowPass=" << get<LowPass>();
    }
    os << ")";
  }

  // ---- Free Functions ---- //
  bool isLowPass(const Frequency &frequency) {
    return frequency.has<LowPass>() && !frequency.has<HighPass>();
  }
  bool isHighPass(const Frequency &frequency) {
    return !frequency.has<LowPass>() && frequency.has<HighPass>();
  }
  bool isBandPass(const Frequency &frequency) {
    return frequency.has<LowPass>() && frequency.has<HighPass>();
  }

}  // namespace adm
