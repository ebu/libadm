#include "adm/elements/audio_content_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Constructor ---- //
  AudioContentId::AudioContentId(AudioContentIdValue value) : value_(value) {}

  // ---- Defaults ---- //
  const AudioContentIdValue AudioContentId::valueDefault_ =
      AudioContentIdValue(0);

  // ---- Getter ---- //
  AudioContentIdValue AudioContentId::get(
      detail::ParameterTraits<AudioContentIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioContentId::has(
      detail::ParameterTraits<AudioContentIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioContentId::isDefault(
      detail::ParameterTraits<AudioContentIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioContentId::set(AudioContentIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioContentId::unset(
      detail::ParameterTraits<AudioContentIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioContentId::operator==(const AudioContentId& other) const {
    return get<AudioContentIdValue>() == other.get<AudioContentIdValue>();
  }

  bool AudioContentId::operator!=(const AudioContentId& other) const {
    return !(*this == other);
  }

  bool AudioContentId::operator<(const AudioContentId& other) const {
    return get<AudioContentIdValue>() < other.get<AudioContentIdValue>();
  }

  // ---- Common ---- //
  void AudioContentId::print(std::ostream& os) const { os << formatId(*this); }

  AudioContentId parseAudioContentId(const std::string& id) {
    // ACO_xxxx
    detail::IDParser parser("AudioContentId", id);
    parser.check_size(8);
    parser.check_prefix("ACO_", 4);
    auto value = parser.parse_hex(4, 4);
    return AudioContentId(AudioContentIdValue(value));
  }

  std::string formatId(const AudioContentId& id) {
    std::string s("ACO_xxxx");
    detail::formatHex(s, 4, 4, id.get<AudioContentIdValue>().get());
    return s;
  }

}  // namespace adm
