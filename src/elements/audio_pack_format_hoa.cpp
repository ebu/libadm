#include "adm/elements/audio_pack_format_hoa.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

#include <iostream>

namespace adm {
  // audio_pack_format_hoa::audio_pack_format_hoa(){
  //}

  // ---- Defaults ---- //
  namespace {
    const Normalization normalizationDefault{"SN3D"};
    const NfcRefDist nfcRefDistDefault{0};
    const ScreenRef screenRefDefualt{false};
  }  // namespace

  // ---- Getter ---- //
  ScreenRef AudioPackFormatHoa::get(
      detail::ParameterTraits<ScreenRef>::tag) const {
    return boost::get_optional_value_or(screenRef_, screenRefDefualt);
  }
  Normalization AudioPackFormatHoa::get(
      detail::ParameterTraits<Normalization>::tag) const {
    return boost::get_optional_value_or(normalization_, normalizationDefault);
  }
  NfcRefDist AudioPackFormatHoa::get(
      detail::ParameterTraits<NfcRefDist>::tag) const {
    return boost::get_optional_value_or(nfcRefDist_, nfcRefDistDefault);
  }

  // ---- isDefault ---- //
  bool AudioPackFormatHoa::isDefault(
      detail::ParameterTraits<ScreenRef>::tag) const {
    return screenRef_ == boost::none;
  }
  bool AudioPackFormatHoa::isDefault(
      detail::ParameterTraits<Normalization>::tag) const {
    return normalization_ == boost::none;
  }
  bool AudioPackFormatHoa::isDefault(
      detail::ParameterTraits<NfcRefDist>::tag) const {
    return nfcRefDist_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioPackFormatHoa::set(ScreenRef screenRef) { screenRef_ = screenRef; }
  void AudioPackFormatHoa::set(Normalization normalization) {
    normalization_ = normalization;
  }
  void AudioPackFormatHoa::set(NfcRefDist nfcRefDist) {
    nfcRefDist_ = nfcRefDist;
  }

  // ---- Has ---- //
  bool AudioPackFormatHoa::has(detail::ParameterTraits<ScreenRef>::tag) const {
    return screenRef_ != boost::none;
  }
  bool AudioPackFormatHoa::has(
      detail::ParameterTraits<Normalization>::tag) const {
    return normalization_ != boost::none;
  }
  bool AudioPackFormatHoa::has(detail::ParameterTraits<NfcRefDist>::tag) const {
    return nfcRefDist_ != boost::none;
  }

  // ---- Unsetter ---- //
  void AudioPackFormatHoa::unset(detail::ParameterTraits<ScreenRef>::tag) {
    screenRef_ = boost::none;
  }
  void AudioPackFormatHoa::unset(detail::ParameterTraits<Normalization>::tag) {
    normalization_ = boost::none;
  }
  void AudioPackFormatHoa::unset(detail::ParameterTraits<NfcRefDist>::tag) {
    nfcRefDist_ = boost::none;
  }

  // ---- Common ---- //
  void AudioPackFormatHoa::print(std::ostream& os) const {
    os << get<AudioPackFormatId>();
    os << " (";
    os << "audioPackFormatName=" << get<AudioPackFormatName>();
    os << ", typeLabel=" << formatTypeLabel(get<TypeDescriptor>());
    os << ", typeDefinition=" << formatTypeDefinition(get<TypeDescriptor>());
    if (has<Importance>()) {
      os << ", importance=" << get<Importance>();
    }
    if (has<AbsoluteDistance>()) {
      os << ", absoluteDistance=" << get<AbsoluteDistance>();
    }
    if (has<ScreenRef>()) {
      os << ", screenRef=" << get<ScreenRef>();
    }
    if (has<Normalization>()) {
      os << ", normalization=" << get<Normalization>();
    }
    if (has<NfcRefDist>()) {
      os << ", nfcRefDist=" << get<NfcRefDist>();
    }
    os << ")";
  }

  AudioPackFormatHoa::AudioPackFormatHoa(AudioPackFormatName name)
      : AudioPackFormat(name, adm::TypeDefinition::HOA) {}
}  // namespace adm
