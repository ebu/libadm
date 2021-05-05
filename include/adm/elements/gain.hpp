#pragma once
#include <cmath>

namespace adm {
  /// storage for gain parameters, which can either be a linear or dB value

  struct GainTag {};
  class Gain {
   public:
    typedef GainTag tag;
    static Gain fromLinear(double linearGain) {
      return Gain{linearGain, false};
    }

    static Gain fromDb(double dbGain) { return Gain{dbGain, true}; }

    double asLinear() const {
      if (isLinear())
        return _gain;
      else
        return std::pow(10, _gain / 20.0);
    }

    double asDb() const {
      if (isDb())
        return _gain;
      else
        return 20.0 * std::log10(_gain);
    }

    bool isLinear() const { return !_isDb; }
    bool isDb() const { return _isDb; }

   private:
    Gain(double gain, bool isDb) : _gain(gain), _isDb(isDb) {}

    double _gain;
    bool _isDb;
  };
}  // namespace adm
