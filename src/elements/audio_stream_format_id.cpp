#include "adm/elements/audio_stream_format_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

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
    const static std::regex r("AS_([0-9a-fA-F]{4})([0-9a-fA-F]{4})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto type = parseTypeLabel(idMatch[1]);
      auto value = detail::parseHexValue(idMatch[2], 4);
      return AudioStreamFormatId(type, AudioStreamFormatIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioStreamFormatId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(AudioStreamFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
