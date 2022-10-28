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

  namespace detail {
    template <>
    struct IdTraits<AudioContentId> {
      static constexpr char const* name{"audioContentID"};
      static constexpr char const* format{"ACO_xxxx"};
      static constexpr std::size_t sections{1};
    };
    template <>
    struct IdSection<AudioContentId, 0> {
      using type = AudioContentIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  AudioContentId parseAudioContentId(const std::string& id) {
    detail::IDParser<AudioContentId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioContentId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
