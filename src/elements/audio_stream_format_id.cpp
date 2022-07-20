#include "adm/elements/audio_stream_format_id.hpp"
#include <boost/format.hpp>
#include <sstream>
#include "adm/detail/hex_values.hpp"
#include "adm/detail/id_parser.hpp"

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
  bool AudioStreamFormatId::operator==(const AudioStreamFormatId& other) const {
    return get<TypeDescriptor>() == other.get<TypeDescriptor>() &&
           get<AudioStreamFormatIdValue>() ==
               other.get<AudioStreamFormatIdValue>();
  }

  bool AudioStreamFormatId::operator!=(const AudioStreamFormatId& other) const {
    return !(*this == other);
  }

  bool AudioStreamFormatId::operator<(const AudioStreamFormatId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioStreamFormatId::print(std::ostream& os) const {
    os << boost::str(
        boost::format("AS_%1%%2%") % formatTypeLabel(get<TypeDescriptor>()) %
        detail::formatHexValue(get<AudioStreamFormatIdValue>().get()));
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

  std::string formatId(AudioStreamFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
