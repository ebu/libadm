#include "adm/elements/audio_block_format_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TypeDescriptor AudioBlockFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioBlockFormatIdValue AudioBlockFormatId::valueDefault_ =
      AudioBlockFormatIdValue(0);
  const AudioBlockFormatIdCounter AudioBlockFormatId::counterDefault_ =
      AudioBlockFormatIdCounter(0);

  // ---- Getter ---- //
  TypeDescriptor AudioBlockFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioBlockFormatIdValue AudioBlockFormatId::get(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }
  AudioBlockFormatIdCounter AudioBlockFormatId::get(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const {
    return boost::get_optional_value_or(counter_, counterDefault_);
  }

  // ---- Has ---- //
  bool AudioBlockFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioBlockFormatId::has(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const {
    return true;
  }
  bool AudioBlockFormatId::has(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioBlockFormatId::isDefault(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const {
    return value_ == boost::none;
  }
  bool AudioBlockFormatId::isDefault(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const {
    return counter_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioBlockFormatId::set(AudioBlockFormatIdValue value) {
    value_ = value;
  }
  void AudioBlockFormatId::set(AudioBlockFormatIdCounter counter) {
    counter_ = counter;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatId::unset(detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioBlockFormatId::unset(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) {
    value_ = boost::none;
  }
  void AudioBlockFormatId::unset(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) {
    counter_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioBlockFormatId::operator==(const AudioBlockFormatId& other) const {
    return get<TypeDescriptor>() == other.get<TypeDescriptor>() &&
           get<AudioBlockFormatIdValue>() ==
               other.get<AudioBlockFormatIdValue>() &&
           get<AudioBlockFormatIdCounter>() ==
               other.get<AudioBlockFormatIdCounter>();
  }

  bool AudioBlockFormatId::operator!=(const AudioBlockFormatId& other) const {
    return !(*this == other);
  }

  bool AudioBlockFormatId::operator<(const AudioBlockFormatId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioBlockFormatId::print(std::ostream& os) const {
    os << boost::str(
        boost::format("AB_%1%%2%_%3%") %
        formatTypeLabel(get<TypeDescriptor>()) %
        detail::formatHexValue(get<AudioBlockFormatIdValue>().get()) %
        detail::formatHexValue(get<AudioBlockFormatIdCounter>().get(), 8));
  }

  AudioBlockFormatId parseAudioBlockFormatId(const std::string& id) {
    const static std::regex r("AB_([0-9a-fA-F]{4})([0-9a-fA-F]{4})_([0-9a-fA-F]{8})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto type = parseTypeLabel(idMatch[1]);
      auto value = detail::parseHexValue(idMatch[2], 4);
      auto counter = detail::parseHexValue(idMatch[3], 8);
      return AudioBlockFormatId(type, AudioBlockFormatIdValue(value),
                                AudioBlockFormatIdCounter(counter));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioBlockFormatId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(AudioBlockFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
