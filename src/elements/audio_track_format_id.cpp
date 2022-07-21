#include "adm/elements/audio_track_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TypeDescriptor AudioTrackFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioTrackFormatIdValue AudioTrackFormatId::valueDefault_ =
      AudioTrackFormatIdValue(0);
  const AudioTrackFormatIdCounter AudioTrackFormatId::counterDefault_ =
      AudioTrackFormatIdCounter(0);

  // ---- Getter ---- //
  TypeDescriptor AudioTrackFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioTrackFormatIdValue AudioTrackFormatId::get(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }
  AudioTrackFormatIdCounter AudioTrackFormatId::get(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const {
    return boost::get_optional_value_or(counter_, counterDefault_);
  }

  // ---- Has ---- //
  bool AudioTrackFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioTrackFormatId::has(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const {
    return true;
  }
  bool AudioTrackFormatId::has(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioTrackFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioTrackFormatId::isDefault(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const {
    return value_ == boost::none;
  }
  bool AudioTrackFormatId::isDefault(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const {
    return counter_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioTrackFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioTrackFormatId::set(AudioTrackFormatIdValue value) {
    value_ = value;
  }
  void AudioTrackFormatId::set(AudioTrackFormatIdCounter counter) {
    counter_ = counter;
  }

  // ---- Unsetter ---- //
  void AudioTrackFormatId::unset(detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioTrackFormatId::unset(
      detail::ParameterTraits<AudioTrackFormatIdValue>::tag) {
    value_ = boost::none;
  }
  void AudioTrackFormatId::unset(
      detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) {
    counter_ = boost::none;
  }

  // ---- Operators ---- //
  using Compare =
      detail::comparison::OptionalComparator<TypeDescriptor,
                                             AudioTrackFormatIdValue,
                                             AudioTrackFormatIdCounter>;

  bool AudioTrackFormatId::operator==(const AudioTrackFormatId& other) const {
    return Compare::allEqual(*this, other);
  }

  bool AudioTrackFormatId::operator!=(const AudioTrackFormatId& other) const {
    return !(*this == other);
  }

  bool AudioTrackFormatId::operator<(const AudioTrackFormatId& other) const {
    return Compare::allLess(*this, other);
  }

  // ---- Common ---- //
  void AudioTrackFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  AudioTrackFormatId parseAudioTrackFormatId(const std::string& id) {
    // AT_yyyyxxxx_zz
    detail::IDParser parser("AudioTrackFormatId", id);
    parser.check_size(14);
    parser.check_prefix("AT_", 3);
    auto type = parser.parse_hex(3, 4);
    auto value = parser.parse_hex(7, 4);
    parser.check_underscore(11);
    auto counter = parser.parse_hex(12, 2);
    return AudioTrackFormatId(TypeDescriptor(type), AudioTrackFormatIdValue(value),
        AudioTrackFormatIdCounter(counter));
  }

  std::string formatId(AudioTrackFormatId id) {
    std::string s("AT_yyyyxxxx_zz");
    detail::formatHex(s, 3, 4, id.get<TypeDescriptor>().get());
    detail::formatHex(s, 7, 4, id.get<AudioTrackFormatIdValue>().get());
    detail::formatHex(s, 12, 2, id.get<AudioTrackFormatIdCounter>().get());
    return s;
  }

}  // namespace adm
