#include "adm/elements/audio_object_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Constructor ---- //
  AudioObjectId::AudioObjectId(AudioObjectIdValue value) : value_(value) {}

  // ---- Defaults ---- //
  const AudioObjectIdValue AudioObjectId::valueDefault_ = AudioObjectIdValue(0);

  // ---- Getter ---- //
  AudioObjectIdValue AudioObjectId::get(
      detail::ParameterTraits<AudioObjectIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioObjectId::has(
      detail::ParameterTraits<AudioObjectIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioObjectId::isDefault(
      detail::ParameterTraits<AudioObjectIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioObjectId::set(AudioObjectIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioObjectId::unset(detail::ParameterTraits<AudioObjectIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioObjectId::operator==(const AudioObjectId& other) const {
    return get<AudioObjectIdValue>() == other.get<AudioObjectIdValue>();
  }

  bool AudioObjectId::operator!=(const AudioObjectId& other) const {
    return !(*this == other);
  }

  bool AudioObjectId::operator<(const AudioObjectId& other) const {
    return get<AudioObjectIdValue>() < other.get<AudioObjectIdValue>();
  }

  // ---- Common ---- //
  void AudioObjectId::print(std::ostream& os) const { os << formatId(*this); }

  AudioObjectId parseAudioObjectId(const std::string& id) {
    // AO_xxxx
    detail::IDParser parser("AudioObjectId", id);
    parser.check_size(7);
    parser.check_prefix("AO_", 3);
    auto value = parser.parse_hex(3, 4);
    return AudioObjectId(AudioObjectIdValue(value));
  }

  std::string formatId(const AudioObjectId& id) {
    std::string s("AO_xxxx");
    detail::formatHex(s, 3, 4, id.get<AudioObjectIdValue>().get());
    return s;
  }

}  // namespace adm
