#include "adm/elements/audio_pack_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TypeDescriptor AudioPackFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioPackFormatIdValue AudioPackFormatId::valueDefault_ =
      AudioPackFormatIdValue(0);

  // ---- Getter ---- //
  TypeDescriptor AudioPackFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioPackFormatIdValue AudioPackFormatId::get(
      detail::ParameterTraits<AudioPackFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioPackFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioPackFormatId::has(
      detail::ParameterTraits<AudioPackFormatIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioPackFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioPackFormatId::isDefault(
      detail::ParameterTraits<AudioPackFormatIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioPackFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioPackFormatId::set(AudioPackFormatIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioPackFormatId::unset(detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioPackFormatId::unset(
      detail::ParameterTraits<AudioPackFormatIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  using Compare =
      detail::comparison::OptionalComparator<TypeDescriptor,
                                             AudioPackFormatIdValue>;

  bool AudioPackFormatId::operator==(const AudioPackFormatId& other) const {
    return Compare::allEqual(*this, other);
  }

  bool AudioPackFormatId::operator!=(const AudioPackFormatId& other) const {
    return !(*this == other);
  }

  bool AudioPackFormatId::operator<(const AudioPackFormatId& other) const {
    return Compare::allLess(*this, other);
  }

  // ---- Common ---- //
  void AudioPackFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace detail {
    template <>
    struct IdTraits<AudioPackFormatId> {
      static constexpr char const* name{"audioPackFormatID"};
      static constexpr char const* prefix{"AP_"};
      static constexpr char const* format{"AP_yyyyxxxx"};
      static constexpr std::size_t sections{2u};
    };
    template <>
    struct IdSection<AudioPackFormatId, 0> {
      using type = TypeDescriptor;
      static constexpr char identifier{'y'};
    };
    template <>
    struct IdSection<AudioPackFormatId, 1> {
      using type = AudioPackFormatIdValue;
      static constexpr char identifier{'x'};
    };
  }  // namespace detail

  AudioPackFormatId parseAudioPackFormatId(const std::string& id) {
    detail::IDParser<AudioPackFormatId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioPackFormatId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
