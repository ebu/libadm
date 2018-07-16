#include "adm/elements/audio_block_format_hoa.hpp"

namespace adm {

  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
  }

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatHoa::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatHoa::get(detail::ParameterTraits<Rtime>::tag) const {
    return boost::get_optional_value_or(rtime_, rtimeDefault);
  }
  Duration AudioBlockFormatHoa::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }
  Lstart AudioBlockFormatHoa::get(detail::ParameterTraits<Lstart>::tag) const {
    return lstart_.get();
  }
  Lduration AudioBlockFormatHoa::get(
      detail::ParameterTraits<Lduration>::tag) const {
    return lduration_.get();
  }
  InitializeBlock AudioBlockFormatHoa::get(
      detail::ParameterTraits<InitializeBlock>::tag) const {
    return initializeBlock_.get();
  }

  // ---- Has ---- //
  bool AudioBlockFormatHoa::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Lstart>::tag) const {
    return lstart_ != boost::none;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Lduration>::tag) const {
    return lduration_ != boost::none;
  }
  bool AudioBlockFormatHoa::has(
      detail::ParameterTraits<InitializeBlock>::tag) const {
    return initializeBlock_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatHoa::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatHoa::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatHoa::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatHoa::set(Duration duration) { duration_ = duration; }
  void AudioBlockFormatHoa::set(Lstart lstart) { lstart_ = lstart; }
  void AudioBlockFormatHoa::set(Lduration lduration) { lduration_ = lduration; }
  void AudioBlockFormatHoa::set(InitializeBlock initializeBlock) {
    initializeBlock_ = initializeBlock;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Lstart>::tag) {
    lstart_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Lduration>::tag) {
    lduration_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(
      detail::ParameterTraits<InitializeBlock>::tag) {
    initializeBlock_ = boost::none;
  }

}  // namespace adm
