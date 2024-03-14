#include "adm/elements/audio_channel_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {
  // ---- Defaults ---- //
  const TypeDescriptor AudioChannelFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioChannelFormatIdValue AudioChannelFormatId::valueDefault_ =
      AudioChannelFormatIdValue(0);

  // ---- Getter ---- //
  TypeDescriptor AudioChannelFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioChannelFormatIdValue AudioChannelFormatId::get(
      detail::ParameterTraits<AudioChannelFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioChannelFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioChannelFormatId::has(
      detail::ParameterTraits<AudioChannelFormatIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioChannelFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioChannelFormatId::isDefault(
      detail::ParameterTraits<AudioChannelFormatIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioChannelFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioChannelFormatId::set(AudioChannelFormatIdValue value) {
    value_ = value;
  }

  // ---- Unsetter ---- //
  void AudioChannelFormatId::unset(
      detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioChannelFormatId::unset(
      detail::ParameterTraits<AudioChannelFormatIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  using Compare =
      detail::comparison::OptionalComparator<TypeDescriptor,
                                             AudioChannelFormatIdValue>;

  bool AudioChannelFormatId::operator==(
      const AudioChannelFormatId& other) const {
    return Compare::allEqual(*this, other);
  }

  bool AudioChannelFormatId::operator!=(
      const AudioChannelFormatId& other) const {
    return !(*this == other);
  }

  bool AudioChannelFormatId::operator<(
      const AudioChannelFormatId& other) const {
    return Compare::allLess(*this, other);
  }

  // ---- Common ---- //
  void AudioChannelFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace detail {
    template <>
    struct IdTraits<AudioChannelFormatId> {
      static constexpr char const* name{"audioChannelFormatID"};
      static constexpr char const* format{"AC_yyyyxxxx"};
      static constexpr std::size_t sections{2};
    };
    template <>
    struct IdSection<AudioChannelFormatId, 0> {
      using type = TypeDescriptor;
      static constexpr char identifier{'y'};
    };
    template <>
    struct IdSection<AudioChannelFormatId, 1> {
      using type = AudioChannelFormatIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  AudioChannelFormatId parseAudioChannelFormatId(const std::string& id) {
    detail::IDParser<AudioChannelFormatId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioChannelFormatId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
