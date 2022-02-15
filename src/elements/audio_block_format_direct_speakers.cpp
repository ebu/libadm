#include "adm/elements/audio_block_format_direct_speakers.hpp"

#include <algorithm>

namespace adm {
  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
  }

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatDirectSpeakers::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatDirectSpeakers::get(
      detail::ParameterTraits<Rtime>::tag) const {
    return boost::get_optional_value_or(rtime_, rtimeDefault);
  }
  Duration AudioBlockFormatDirectSpeakers::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }
  SpeakerLabels AudioBlockFormatDirectSpeakers::get(
      detail::ParameterTraits<SpeakerLabels>::tag) const {
    return speakerLabels_;
  }
  CartesianSpeakerPosition AudioBlockFormatDirectSpeakers::get(
      detail::ParameterTraits<CartesianSpeakerPosition>::tag) const {
    return boost::get<CartesianSpeakerPosition>(speakerPosition_);
  }
  SphericalSpeakerPosition AudioBlockFormatDirectSpeakers::get(
      detail::ParameterTraits<SphericalSpeakerPosition>::tag) const {
    return boost::get<SphericalSpeakerPosition>(speakerPosition_);
  }

  // ---- Has ---- //
  bool AudioBlockFormatDirectSpeakers::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatDirectSpeakers::has(
      detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatDirectSpeakers::has(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }
  bool AudioBlockFormatDirectSpeakers::has(
      detail::ParameterTraits<SpeakerLabels>::tag) const {
    return speakerLabels_.size() > 0;
  }
  bool AudioBlockFormatDirectSpeakers::has(
      detail::ParameterTraits<CartesianSpeakerPosition>::tag) const {
    return (boost::get<CartesianSpeakerPosition>(&speakerPosition_));
  }
  bool AudioBlockFormatDirectSpeakers::has(
      detail::ParameterTraits<SphericalSpeakerPosition>::tag) const {
    return (boost::get<SphericalSpeakerPosition>(&speakerPosition_));
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatDirectSpeakers::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatDirectSpeakers::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatDirectSpeakers::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatDirectSpeakers::set(Duration duration) {
    duration_ = duration;
  }
  void AudioBlockFormatDirectSpeakers::set(
      CartesianSpeakerPosition speakerPosition) {
    speakerPosition_ = speakerPosition;
  }
  void AudioBlockFormatDirectSpeakers::set(
      SphericalSpeakerPosition speakerPosition) {
    speakerPosition_ = speakerPosition;
  }
  void AudioBlockFormatDirectSpeakers::set(SpeakerPosition speakerPosition) {
    speakerPosition_ = speakerPosition;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatDirectSpeakers::unset(
      detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatDirectSpeakers::unset(
      detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }
  void AudioBlockFormatDirectSpeakers::unset(
      detail::ParameterTraits<SpeakerLabels>::tag) {
    speakerLabels_.clear();
  }

  // ---- Add ---- //
  bool AudioBlockFormatDirectSpeakers::add(SpeakerLabel speakerLabel) {
    auto it =
        std::find(speakerLabels_.begin(), speakerLabels_.end(), speakerLabel);
    if (it == speakerLabels_.end()) {
      speakerLabels_.push_back(speakerLabel);
      return true;
    } else {
      return false;
    }
  }

  // ---- Remove ---- //
  void AudioBlockFormatDirectSpeakers::remove(SpeakerLabel speakerLabel) {
    auto it =
        std::find(speakerLabels_.begin(), speakerLabels_.end(), speakerLabel);
    if (it != speakerLabels_.end()) {
      speakerLabels_.erase(it);
    }
  }

}  // namespace adm
