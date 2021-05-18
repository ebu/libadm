#pragma once
#include <cmath>

namespace adm {

  struct GainTag {};

  /// Storage for gain parameters, which can either be a linear or dB value.
  ///
  /// Gain objects should be constructed using fromLinear() or fromDb().
  class Gain {
   public:
    typedef GainTag tag;

    /// alias for Gain::fromLinear for compatibility with existing code
    explicit Gain(double linearGain) : _gain(linearGain), _isDb(false) {}

    /// Make a linear Gain.
    static Gain fromLinear(double linearGain) {
      return Gain{linearGain, false};
    }

    /// Make a Gain in dB.
    static Gain fromDb(double dbGain) { return Gain{dbGain, true}; }

    /// Get the gain as a linear value, converting if necessary.
    double asLinear() const {
      if (isLinear())
        return _gain;
      else
        return std::pow(10, _gain / 20.0);
    }

    /// Get the gain as a value in dB, converting if necessary.
    double asDb() const {
      if (isDb())
        return _gain;
      else
        return 20.0 * std::log10(_gain);
    }

    /// Is this gain stored as a linear value?
    bool isLinear() const { return !_isDb; }
    /// Is this gain stored in dB?
    bool isDb() const { return _isDb; }

    /// alias for asLinear for compatibility with existing code
    double get() const { return asLinear(); }
    /// alias for asLinear for compatibility with existing code
    double operator*() const { return asLinear(); }

   private:
    Gain(double gain, bool isDb) : _gain(gain), _isDb(isDb) {}

    double _gain;
    bool _isDb;
  };
}  // namespace adm
