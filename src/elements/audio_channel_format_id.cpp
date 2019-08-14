#include "adm/elements/audio_channel_format_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

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
    const static std::regex r("AC_([0-9a-fA-F]{4})([0-9a-fA-F]{4})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto type = parseTypeLabel(idMatch[1]);
      auto value = detail::parseHexValue(idMatch[2], 4);
      return AudioChannelFormatId(type, AudioChannelFormatIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioChannelFormatId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(AudioChannelFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
