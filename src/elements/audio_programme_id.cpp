#include "adm/elements/audio_programme_id.hpp"
#include <sstream>
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
    return get<AudioProgrammeIdValue>() < other.get<AudioProgrammeIdValue>();
  }

  // ---- Common ---- //
  void AudioProgrammeId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace detail {
    template <>
    struct IdTraits<AudioProgrammeId> {
      static constexpr char const* name{"audioProgrammeID"};
      static constexpr char const* prefix{"APR_"};
      static constexpr char const* format{"APR_xxxx"};
    };
  }  // namespace detail

  AudioProgrammeId parseAudioProgrammeId(const std::string& id) {
    // APR_xxxx
    detail::IDParser parser{id};
    parser.check_prefix<AudioProgrammeId>();
    parser.check_size<AudioProgrammeId>();
    auto value = parser.parse_hex<AudioProgrammeId>(4, 4);
    return AudioProgrammeId(AudioProgrammeIdValue(value));
  }

  std::string formatId(const AudioProgrammeId& id) {
    std::string s("APR_xxxx");
    detail::formatHex(s, 4, 4, id.get<AudioProgrammeIdValue>().get());
    return s;
  }

}  // namespace adm
