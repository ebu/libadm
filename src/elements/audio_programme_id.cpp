#include "adm/elements/audio_programme_id.hpp"
#include <boost/format.hpp>
#include <sstream>
#include "adm/detail/hex_values.hpp"
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Constructor ---- //
  AudioProgrammeId::AudioProgrammeId(AudioProgrammeIdValue value)
      : value_(value) {}

  // ---- Defaults ---- //
  const AudioProgrammeIdValue AudioProgrammeId::valueDefault_ =
      AudioProgrammeIdValue(0);

  // ---- Getter ---- //
  AudioProgrammeIdValue AudioProgrammeId::get(
      detail::ParameterTraits<AudioProgrammeIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioProgrammeId::has(
      detail::ParameterTraits<AudioProgrammeIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioProgrammeId::isDefault(
      detail::ParameterTraits<AudioProgrammeIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioProgrammeId::set(AudioProgrammeIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioProgrammeId::unset(
      detail::ParameterTraits<AudioProgrammeIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioProgrammeId::operator==(const AudioProgrammeId& other) const {
    return get<AudioProgrammeIdValue>() == other.get<AudioProgrammeIdValue>();
  }

  bool AudioProgrammeId::operator!=(const AudioProgrammeId& other) const {
    return !(*this == other);
  }

  bool AudioProgrammeId::operator<(const AudioProgrammeId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioProgrammeId::print(std::ostream& os) const {
    os << boost::str(
        boost::format("APR_%1%") %
        detail::formatHexValue(get<AudioProgrammeIdValue>().get()));
  }

  AudioProgrammeId parseAudioProgrammeId(const std::string& id) {
    // APR_xxxx
    detail::IDParser parser("AudioProgrammeId", id);
    parser.check_size(8);
    parser.check_prefix("APR_", 4);
    auto value = parser.parse_hex(4, 4);
    return AudioProgrammeId(AudioProgrammeIdValue(value));
  }

  std::string formatId(AudioProgrammeId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
