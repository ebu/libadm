#include "adm/elements/audio_track_uid_id.hpp"
#include <boost/format.hpp>
#include <sstream>
#include "adm/detail/hex_values.hpp"
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Constructor ---- //
  AudioTrackUidId::AudioTrackUidId(AudioTrackUidIdValue value)
      : value_(value) {}

  // ---- Defaults ---- //
  const AudioTrackUidIdValue AudioTrackUidId::valueDefault_ =
      AudioTrackUidIdValue(0);

  // ---- Getter ---- //
  AudioTrackUidIdValue AudioTrackUidId::get(
      detail::ParameterTraits<AudioTrackUidIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioTrackUidId::has(
      detail::ParameterTraits<AudioTrackUidIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioTrackUidId::isDefault(
      detail::ParameterTraits<AudioTrackUidIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioTrackUidId::set(AudioTrackUidIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioTrackUidId::unset(
      detail::ParameterTraits<AudioTrackUidIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioTrackUidId::operator==(const AudioTrackUidId& other) const {
    return get<AudioTrackUidIdValue>() == other.get<AudioTrackUidIdValue>();
  }

  bool AudioTrackUidId::operator!=(const AudioTrackUidId& other) const {
    return !(*this == other);
  }

  bool AudioTrackUidId::operator<(const AudioTrackUidId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioTrackUidId::print(std::ostream& os) const {
    os << boost::str(
        boost::format("ATU_%1%") %
        detail::formatHexValue(get<AudioTrackUidIdValue>().get(), 8));
  }

  AudioTrackUidId parseAudioTrackUidId(const std::string& id) {
    // ATU_xxxxxxxx
    detail::IDParser parser("AudioChannelFormatId", id);
    parser.check_size(12);
    parser.check_prefix("ATU_", 4);
    auto value = parser.parse_hex(4, 8);
    return AudioTrackUidId(AudioTrackUidIdValue(value));
  }

  std::string formatId(AudioTrackUidId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
