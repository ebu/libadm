#include "adm/elements/audio_block_format_id.hpp"
#include <boost/format.hpp>
#include <sstream>
#include "adm/detail/hex_values.hpp"
#include "adm/detail/id_parser.hpp"

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
    // AB_yyyyxxxx_zzzzzzzz
    detail::IDParser parser("AudioBlockFormatId", id);
    parser.check_size(20);
    parser.check_prefix("AB_", 3);
    auto type = parser.parse_hex(3, 4);
    auto value = parser.parse_hex(7, 4);
    parser.check_underscore(11);
    auto counter = parser.parse_hex(12, 8);
    return AudioBlockFormatId(TypeDescriptor(type), AudioBlockFormatIdValue(value),
        AudioBlockFormatIdCounter(counter));
  }

  std::string formatId(AudioBlockFormatId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
