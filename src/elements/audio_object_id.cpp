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

  namespace detail {
    template <>
    struct IdTraits<AudioObjectId> {
      static constexpr char const* name{"audioObjectID"};
      static constexpr char const* format{"AO_xxxx"};
      static constexpr std::size_t sections{1};
    };
    template <>
    struct IdSection<AudioObjectId, 0> {
      using type = AudioObjectIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  AudioObjectId parseAudioObjectId(const std::string& id) {
    detail::IDParser<AudioObjectId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioObjectId& id) { return detail::formatId(id); }

}  // namespace adm
