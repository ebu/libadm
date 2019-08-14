#include "adm/elements/audio_pack_format_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

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
  bool AudioPackFormatId::operator==(const AudioPackFormatId& other) const {
    return get<TypeDescriptor>() == other.get<TypeDescriptor>() &&
           get<AudioPackFormatIdValue>() == other.get<AudioPackFormatIdValue>();
  }

  bool AudioPackFormatId::operator!=(const AudioPackFormatId& other) const {
    return !(*this == other);
  }

  bool AudioPackFormatId::operator<(const AudioPackFormatId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioPackFormatId::print(std::ostream& os) const {
    os << boost::str(
        boost::format("AP_%1%%2%") % formatTypeLabel(get<TypeDescriptor>()) %
        detail::formatHexValue(get<AudioPackFormatIdValue>().get()));
  }

  AudioPackFormatId parseAudioPackFormatId(const std::string& id) {
    const static std::regex r("AP_([0-9a-fA-F]{4})([0-9a-fA-F]{4})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto type = parseTypeLabel(idMatch[1]);
      auto value = detail::parseHexValue(idMatch[2], 4);
      return AudioPackFormatId(type, AudioPackFormatIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioPackFormatId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(AudioPackFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
