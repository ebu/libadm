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

  AudioPackFormatId parseAudioPackFormatId(const std::string& id) {
    // AP_yyyyxxxx
    detail::IDParser parser("AudioPackFormatId", id);
    parser.check_size(11);
    parser.check_prefix("AP_", 3);
    auto type = parser.parse_hex(3, 4);
    auto value = parser.parse_hex(7, 4);
    return AudioPackFormatId(TypeDescriptor(type),
                             AudioPackFormatIdValue(value));
  }

  std::string formatId(const AudioPackFormatId& id) {
    std::string s("AP_yyyyxxxx");
    detail::formatHex(s, 3, 4, id.get<TypeDescriptor>().get());
    detail::formatHex(s, 7, 4, id.get<AudioPackFormatIdValue>().get());
    return s;
  }

}  // namespace adm
