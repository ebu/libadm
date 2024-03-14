#include "adm/elements/audio_track_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TypeDescriptor AudioTrackFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioTrackFormatIdValue AudioTrackFormatId::valueDefault_ =
      AudioTrackFormatIdValue(0);
  const AudioTrackFormatIdCounter AudioTrackFormatId::counterDefault_ =
      AudioTrackFormatIdCounter(0);

  // ---- Getter ---- //
  TypeDescriptor AudioTrackFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioTrackFormatIdValue AudioTrackFormatId::get(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }
  AudioTrackFormatIdCounter AudioTrackFormatId::get(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const {
    return boost::get_optional_value_or(counter_, counterDefault_);
  }

  // ---- Has ---- //
  bool AudioTrackFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioTrackFormatId::has(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const {
    return true;
  }
  bool AudioTrackFormatId::has(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioTrackFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioTrackFormatId::isDefault(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const {
    return value_ == boost::none;
  }
  bool AudioTrackFormatId::isDefault(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const {
    return counter_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioTrackFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioTrackFormatId::set(AudioTrackFormatIdValue value) {
    value_ = value;
  }
  void AudioTrackFormatId::set(AudioTrackFormatIdCounter counter) {
    counter_ = counter;
  }

  // ---- Unsetter ---- //
  void AudioTrackFormatId::unset(detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioTrackFormatId::unset(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) {
    value_ = boost::none;
  }
  void AudioTrackFormatId::unset(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) {
    counter_ = boost::none;
  }

  // ---- Operators ---- //
  using Compare =
      detail::comparison::OptionalComparator<TypeDescriptor,
                                             AudioTrackFormatIdValue,
                                             AudioTrackFormatIdCounter>;

  bool AudioTrackFormatId::operator==(const AudioTrackFormatId& other) const {
    return Compare::allEqual(*this, other);
  }

  bool AudioTrackFormatId::operator!=(const AudioTrackFormatId& other) const {
    return !(*this == other);
  }

  bool AudioTrackFormatId::operator<(const AudioTrackFormatId& other) const {
    return Compare::allLess(*this, other);
  }

  // ---- Common ---- //
  void AudioTrackFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace detail {
    template <>
    struct IdTraits<AudioTrackFormatId> {
      static constexpr char const* const name{"audioTrackFormatID"};
      static constexpr char const* const format{"AT_yyyyxxxx_zz"};
      static constexpr std::size_t sections{3};
    };
    template <>
    struct IdSection<AudioTrackFormatId, 0> {
      using type = TypeDescriptor;
      static constexpr char identifier{'y'};
    };
    template <>
    struct IdSection<AudioTrackFormatId, 1> {
      using type = AudioTrackFormatIdValue;
      static constexpr char identifier{'x'};
    };
    template <>
    struct IdSection<AudioTrackFormatId, 2> {
      using type = AudioTrackFormatIdCounter;
      static constexpr char identifier{'z'};
    };
  }  // namespace detail

  AudioTrackFormatId parseAudioTrackFormatId(const std::string& id) {
    detail::IDParser<AudioTrackFormatId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioTrackFormatId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
