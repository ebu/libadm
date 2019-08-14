#include "adm/elements/audio_object_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

namespace adm {

  // ---- Defaults ---- //
  const AudioObjectIdValue AudioObjectId::valueDefault_ = AudioObjectIdValue(0);

  // ---- Getter ---- //
  AudioObjectIdValue AudioObjectId::get(
      detail::ParameterTraits<AudioObjectIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioObjectId::has(
      detail::ParameterTraits<AudioObjectIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioObjectId::isDefault(
      detail::ParameterTraits<AudioObjectIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioObjectId::set(AudioObjectIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioObjectId::unset(detail::ParameterTraits<AudioObjectIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioObjectId::operator==(const AudioObjectId& other) const {
    return get<AudioObjectIdValue>() == other.get<AudioObjectIdValue>();
  }

  bool AudioObjectId::operator!=(const AudioObjectId& other) const {
    return !(*this == other);
  }

  bool AudioObjectId::operator<(const AudioObjectId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioObjectId::print(std::ostream& os) const {
    os << boost::str(boost::format("AO_%1%") %
                     detail::formatHexValue(get<AudioObjectIdValue>().get()));
  }

  AudioObjectId parseAudioObjectId(const std::string& id) {
    const static std::regex r("AO_([0-9a-fA-F]{4})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto value = detail::parseHexValue(idMatch[1], 4);
      return AudioObjectId(AudioObjectIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioObjectId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(AudioObjectId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
