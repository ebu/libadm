#include "adm/elements/audio_stream_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TypeDescriptor AudioStreamFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioStreamFormatIdValue AudioStreamFormatId::valueDefault_ =
      AudioStreamFormatIdValue(0);

  // ---- Getter ---- //
  TypeDescriptor AudioStreamFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioStreamFormatIdValue AudioStreamFormatId::get(
      detail::ParameterTraits<AudioStreamFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioStreamFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioStreamFormatId::has(
      detail::ParameterTraits<AudioStreamFormatIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioStreamFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioStreamFormatId::isDefault(
      detail::ParameterTraits<AudioStreamFormatIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioStreamFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioStreamFormatId::set(AudioStreamFormatIdValue value) {
    value_ = value;
  }

  // ---- Unsetter ---- //
  void AudioStreamFormatId::unset(
      detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioStreamFormatId::unset(
      detail::ParameterTraits<AudioStreamFormatIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  using Compare =
      detail::comparison::OptionalComparator<TypeDescriptor,
                                             AudioStreamFormatIdValue>;

  bool AudioStreamFormatId::operator==(const AudioStreamFormatId& other) const {
    return Compare::allEqual(*this, other);
  }

  bool AudioStreamFormatId::operator!=(const AudioStreamFormatId& other) const {
    return !(*this == other);
  }

  bool AudioStreamFormatId::operator<(const AudioStreamFormatId& other) const {
    return Compare::allLess(*this, other);
  }

  // ---- Common ---- //
  void AudioStreamFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace detail {
    template <>
    struct IdTraits<AudioStreamFormatId> {
      static constexpr char const* name{"audioStreamFormatID"};
      static constexpr char const* format{"AS_yyyyxxxx"};
      static constexpr std::size_t sections{2};
    };
    template <>
    struct IdSection<AudioStreamFormatId, 0> {
      using type = TypeDescriptor;
      static constexpr char identifier{'y'};
    };
    template <>
    struct IdSection<AudioStreamFormatId, 1> {
      using type = AudioStreamFormatIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  AudioStreamFormatId parseAudioStreamFormatId(const std::string& id) {
    detail::IDParser<AudioStreamFormatId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioStreamFormatId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
