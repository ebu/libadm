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
  Order AudioBlockFormatHoa::get(
      detail::ParameterTraits<Order>::tag) const {
    return order_.get();
  }
  Degree AudioBlockFormatHoa::get(
      detail::ParameterTraits<Degree>::tag) const {
    return degree_.get();
  }
  NfcRefDist AudioBlockFormatHoa::get(
          detail::ParameterTraits<NfcRefDist>::tag) const {
      return nfcRefDist_.get();
  }
  ScreenRef AudioBlockFormatHoa::get(
          detail::ParameterTraits<ScreenRef>::tag) const {
      return screenRef_.get();
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
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Order>::tag) const {
    return order_ != boost::none;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Degree>::tag) const {
    return degree_ != boost::none;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<NfcRefDist>::tag) const {
      return nfcRefDist_ != boost::none;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<ScreenRef>::tag) const {
      return screenRef_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatHoa::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == boost::none;
  }
  bool AudioBlockFormatHoa::isDefault(
          detail::ParameterTraits<NfcRefDist>::tag) const {
      return nfcRefDist_ == boost::none;
  }
  bool AudioBlockFormatHoa::isDefault(
      detail::ParameterTraits<ScreenRef>::tag) const {
    return screenRef_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatHoa::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatHoa::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatHoa::set(Duration duration) { duration_ = duration; }
  void AudioBlockFormatHoa::set(Order order) { order_ = order; }
  void AudioBlockFormatHoa::set(Degree degree) { degree_ = degree; }
  void AudioBlockFormatHoa::set(NfcRefDist nfcRefDist) { nfcRefDist_ = nfcRefDist; }
  void AudioBlockFormatHoa::set(ScreenRef screenRef) { screenRef_ = screenRef; }

  // ---- Unsetter ---- //
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Order>::tag) {
    order_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Degree>::tag) {
      degree_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<NfcRefDist>::tag) {
      nfcRefDist_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<ScreenRef>::tag) {
      screenRef_ = boost::none;
  }

}  // namespace adm
