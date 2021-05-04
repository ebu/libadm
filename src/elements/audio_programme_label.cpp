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

  // TODO don't have a clear pattern to parse from as value can be any string
  /*
  AudioProgrammeLabel parseAudioProgrammeLabel(const std::string& id) {
    const static std::regex r(
        "AB_([0-9a-fA-F]{4})([0-9a-fA-F]{4})_([0-9a-fA-F]{8})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto type = parseTypeLabel(idMatch[1]);
      auto value = detail::parseHexValue(idMatch[2], 4);
      auto counter = detail::parseHexValue(idMatch[3], 8);
      return AudioProgrammeLabel(type, AudioProgrammeLabelValue(value),
                                 AudioProgrammeLabelCounter(counter));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioProgrammeLabel: " << id;
      throw std::runtime_error(errorString.str());
    }
  }
  */
}  // namespace adm
