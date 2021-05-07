#include "adm/elements/audio_block_format.hpp"

namespace adm {
  namespace {
    const Gain gainDefault{Gain::fromLinear(1.0)};
  }

  Gain AudioBlockFormat::get(detail::ParameterTraits<Gain>::tag) const {
    return boost::get_optional_value_or(gain_, gainDefault);
  }
  bool AudioBlockFormat::has(detail::ParameterTraits<Gain>::tag) const {
    return true;
  }
  bool AudioBlockFormat::isDefault(detail::ParameterTraits<Gain>::tag) const {
    return gain_ == boost::none;
  }
  void AudioBlockFormat::set(Gain gain) { gain_ = gain; }
  void AudioBlockFormat::unset(detail::ParameterTraits<Gain>::tag) {
    gain_ = boost::none;
  }
};  // namespace adm
