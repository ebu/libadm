#include "adm/elements/audio_track_uid_id.hpp"
#include <limits>
#include <sstream>
#include "adm/detail/id_parser.hpp"

namespace adm {

  // ---- Constructor ---- //
  AudioTrackUidId::AudioTrackUidId(AudioTrackUidIdValue value)
      : value_(value) {}

  // ---- Defaults ---- //
  const AudioTrackUidIdValue AudioTrackUidId::valueDefault_ =
      AudioTrackUidIdValue(
          std::numeric_limits<AudioTrackUidIdValue::value_type>::max());

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
    return get<AudioTrackUidIdValue>() < other.get<AudioTrackUidIdValue>();
  }

  // ---- Common ---- //
  void AudioTrackUidId::print(std::ostream& os) const { os << formatId(*this); }

  namespace detail {
    template <>
    struct IdTraits<AudioTrackUidId> {
      static constexpr char const* name{"audioTrackUidID"};
      static constexpr char const* format{"ATU_xxxxxxxx"};
      static constexpr std::size_t sections{1};
    };
    template <>
    struct IdSection<AudioTrackUidId, 0> {
      using type = AudioTrackUidIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  AudioTrackUidId parseAudioTrackUidId(const std::string& id) {
    detail::IDParser<AudioTrackUidId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioTrackUidId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
