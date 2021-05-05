#include "adm/elements/audio_programme_label.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>

namespace adm {
  // ---- Getter ---- //
  AudioProgrammeLabelValue AudioProgrammeLabel::get(
      detail::ParameterTraits<AudioProgrammeLabelValue>::tag) const {
    return this->value_.get();
  }
  AudioProgrammeLabelLanguage AudioProgrammeLabel::get(
      detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag) const {
    return this->language_.get();
  }

  // ---- Has ---- //
  bool AudioProgrammeLabel::has(
      detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag) const {
    return this->language_ != boost::none;
  }
  bool AudioProgrammeLabel::has(
      detail::ParameterTraits<AudioProgrammeLabelValue>::tag) const {
    return this->language_ != boost::none;
  }

  // ---- Setter ---- //
  void AudioProgrammeLabel::set(AudioProgrammeLabelValue value) {
    value_ = value;
  }
  void AudioProgrammeLabel::set(AudioProgrammeLabelLanguage language) {
    language_ = language;
  }

  // ---- Unsetter ---- //
  void AudioProgrammeLabel::unset(
      detail::ParameterTraits<AudioProgrammeLabelValue>::tag) {
    value_ = boost::none;
  }
  void AudioProgrammeLabel::unset(
      detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag) {
    language_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioProgrammeLabel::operator==(const AudioProgrammeLabel& other) const {
    return get<AudioProgrammeLabelValue>() ==
               other.get<AudioProgrammeLabelValue>() &&
           get<AudioProgrammeLabelLanguage>() ==
               other.get<AudioProgrammeLabelLanguage>();
  }

  bool AudioProgrammeLabel::operator!=(const AudioProgrammeLabel& other) const {
    return !(*this == other);
  }

  // TODO don't have an Id to compare here - does it make sense to have a <
  // operator?

  // bool AudioProgrammeLabel::operator<(const AudioProgrammeLabel&
  // other) const {
  //   return formatId(*this) < formatId(other);
  // }

  // ---- Common ---- //
  // TODO what should the format be for a value string and a language string?
  /*
  void AudioProgrammeLabel::print(std::ostream& os) const {
    os << boost::str(
        boost::format("value = %2% lang = %1%") %
        has<AudioProgrammeLabelValue>() ? get<AudioProgrammeLabelValue> : "" %
        has<AudioProgrammeLabelLanguage>() ? get<AudioProgrammeLabelLanguage> :
  "" %
    );
  }
  */

}  // namespace adm
