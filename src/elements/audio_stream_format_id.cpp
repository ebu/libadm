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

  AudioStreamFormatId parseAudioStreamFormatId(const std::string& id) {
    // AS_yyyyxxxx
    detail::IDParser parser("AudioStreamFormatId", id);
    parser.check_size(11);
    parser.check_prefix("AS_", 3);
    auto type = parser.parse_hex(3, 4);
    auto value = parser.parse_hex(7, 4);
    return AudioStreamFormatId(TypeDescriptor(type),
                               AudioStreamFormatIdValue(value));
  }

  std::string formatId(const AudioStreamFormatId& id) {
    std::string s("AS_yyyyxxxx");
    detail::formatHex(s, 3, 4, id.get<TypeDescriptor>().get());
    detail::formatHex(s, 7, 4, id.get<AudioStreamFormatIdValue>().get());
    return s;
  }

}  // namespace adm
