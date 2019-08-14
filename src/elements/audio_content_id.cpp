#include "adm/elements/audio_content_id.hpp"
#include <boost/format.hpp>
#include <regex>
#include <sstream>
#include "adm/detail/hex_values.hpp"

namespace adm {

  // ---- Defaults ---- //
  const AudioContentIdValue AudioContentId::valueDefault_ =
      AudioContentIdValue(0);

  // ---- Getter ---- //
  AudioContentIdValue AudioContentId::get(
      detail::ParameterTraits<AudioContentIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }

  // ---- Has ---- //
  bool AudioContentId::has(
      detail::ParameterTraits<AudioContentIdValue>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioContentId::isDefault(
      detail::ParameterTraits<AudioContentIdValue>::tag) const {
    return value_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioContentId::set(AudioContentIdValue value) { value_ = value; }

  // ---- Unsetter ---- //
  void AudioContentId::unset(
      detail::ParameterTraits<AudioContentIdValue>::tag) {
    value_ = boost::none;
  }

  // ---- Operators ---- //
  bool AudioContentId::operator==(const AudioContentId& other) const {
    return get<AudioContentIdValue>() == other.get<AudioContentIdValue>();
  }

  bool AudioContentId::operator!=(const AudioContentId& other) const {
    return !(*this == other);
  }

  bool AudioContentId::operator<(const AudioContentId& other) const {
    return formatId(*this) < formatId(other);
  }

  // ---- Common ---- //
  void AudioContentId::print(std::ostream& os) const {
    os << boost::str(boost::format("ACO_%1%") %
                     detail::formatHexValue(get<AudioContentIdValue>().get()));
  }

  AudioContentId parseAudioContentId(const std::string& id) {
    const static std::regex r("ACO_([0-9a-fA-F]{4})");
    std::smatch idMatch;
    if (std::regex_match(id, idMatch, r)) {
      auto value = detail::parseHexValue(idMatch[1], 4);
      return AudioContentId(AudioContentIdValue(value));
    } else {
      std::stringstream errorString;
      errorString << "invalid AudioContentId: " << id;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatId(AudioContentId id) {
    std::stringstream idStream;
    id.print(idStream);
    return idStream.str();
  }

}  // namespace adm
