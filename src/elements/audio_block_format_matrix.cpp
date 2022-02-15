#include "adm/elements/audio_block_format_matrix.hpp"

namespace adm {
  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
  }

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatMatrix::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatMatrix::get(detail::ParameterTraits<Rtime>::tag) const {
    return boost::get_optional_value_or(rtime_, rtimeDefault);
  }
  Duration AudioBlockFormatMatrix::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }

  // ---- Has ---- //
  bool AudioBlockFormatMatrix::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatMatrix::has(detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatMatrix::has(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatMatrix::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatMatrix::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatMatrix::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatMatrix::set(Duration duration) { duration_ = duration; }

  // ---- Unsetter ---- //
  void AudioBlockFormatMatrix::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatMatrix::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }

}  // namespace adm
