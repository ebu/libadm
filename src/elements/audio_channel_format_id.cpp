#include "adm/elements/audio_channel_format_id.hpp"
#include <boost/format.hpp>
#include <sstream>
#include "adm/detail/hex_values.hpp"
#include "adm/detail/id_parser.hpp"

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
  bool AudioChannelFormatId::operator==(
      const AudioChannelFormatId& other) const {
    return get<TypeDescriptor>() == other.get<TypeDescriptor>() &&
           get<AudioChannelFormatIdValue>() ==
               other.get<AudioChannelFormatIdValue>();
  }

  bool AudioChannelFormatId::operator!=(
      const AudioChannelFormatId& other) const {
    return !(*this == other);
  }

  bool AudioChannelFormatId::operator<(
      const AudioChannelFormatId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioChannelFormatId::print(std::ostream& os) const {
    os << boost::str(
        boost::format("AC_%1%%2%") % formatTypeLabel(get<TypeDescriptor>()) %
        detail::formatHexValue(get<AudioChannelFormatIdValue>().get()));
  }

  AudioChannelFormatId parseAudioChannelFormatId(const std::string& id) {
    // AC_yyyyxxxx
    detail::IDParser parser("AudioChannelFormatId", id);
    parser.check_size(11);
    parser.check_prefix("AC_", 3);
    auto type = parser.parse_hex(3, 4);
    auto value = parser.parse_hex(7, 4);
    return AudioChannelFormatId(TypeDescriptor(type),
                                AudioChannelFormatIdValue(value));
  }

  std::string formatId(AudioChannelFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
