#include "adm/elements/audio_block_format_binaural.hpp"

namespace adm {
  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
  }

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatBinaural::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatBinaural::get(
      detail::ParameterTraits<Rtime>::tag) const {
    return boost::get_optional_value_or(rtime_, rtimeDefault);
  }
  Duration AudioBlockFormatBinaural::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }
  InitializeBlock AudioBlockFormatBinaural::get(
      detail::ParameterTraits<InitializeBlock>::tag) const {
    return initializeBlock_.get();
  }

  // ---- Has ---- //
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }
  bool AudioBlockFormatBinaural::has(
      detail::ParameterTraits<InitializeBlock>::tag) const {
    return initializeBlock_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatBinaural::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatBinaural::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatBinaural::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatBinaural::set(Duration duration) {
    duration_ = duration;
  }
  void AudioBlockFormatBinaural::set(InitializeBlock initializeBlock) {
    initializeBlock_ = initializeBlock;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatBinaural::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatBinaural::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }
  void AudioBlockFormatBinaural::unset(
      detail::ParameterTraits<InitializeBlock>::tag) {
    initializeBlock_ = boost::none;
  }
  
}  // namespace adm
