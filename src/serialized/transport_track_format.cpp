#include "adm/serialized/transport_track_format.hpp"

#include <algorithm>
#include <functional>
#include "adm/utilities/element_io.hpp"

namespace adm {

  // ---- Getter ---- //
  TransportId TransportTrackFormat::get(
      detail::ParameterTraits<TransportId>::tag) const {
    return transportId_;
  }
  TransportName TransportTrackFormat::get(
      detail::ParameterTraits<TransportName>::tag) const {
    return transportName_.get();
  }
  NumTracks TransportTrackFormat::get(
      detail::ParameterTraits<NumTracks>::tag) const {
    return NumTracks(audioTracks_.size());
  }
  NumIds TransportTrackFormat::get(detail::ParameterTraits<NumIds>::tag) const {
    unsigned int numIdsCounter = 0;
    for (const auto audioTrack : audioTracks_) {
      numIdsCounter += audioTrack.audioTrackUidIds().size();
    }
    return NumIds(numIdsCounter);
  }

  // ---- Has ---- //
  bool TransportTrackFormat::has(
      detail::ParameterTraits<TransportId>::tag) const {
    return true;
  }
  bool TransportTrackFormat::has(
      detail::ParameterTraits<TransportName>::tag) const {
    return transportName_ != boost::none;
  }
  bool TransportTrackFormat::has(
      detail::ParameterTraits<NumTracks>::tag) const {
    return true;
  }
  bool TransportTrackFormat::has(detail::ParameterTraits<NumIds>::tag) const {
    return true;
  }

  // ---- Setter ---- //
  void TransportTrackFormat::set(TransportId id) { transportId_ = id; }
  void TransportTrackFormat::set(TransportName name) { transportName_ = name; }

  // ---- Unsetter ---- //
  void TransportTrackFormat::unset(
      detail::ParameterTraits<TransportName>::tag) {
    transportName_ = boost::none;
  }

  // ---- AudioTracks ---- //
  void TransportTrackFormat::add(const AudioTrack& audioTrack) {
    audioTracks_.push_back(audioTrack);
  }

  AudioTrackConstRange TransportTrackFormat::audioTracks() const {
    return boost::make_iterator_range(audioTracks_.cbegin(),
                                      audioTracks_.cend());
  }

  AudioTrackRange TransportTrackFormat::audioTracks() {
    return boost::make_iterator_range(audioTracks_.begin(), audioTracks_.end());
  }

  void TransportTrackFormat::clearAudioTracks() { audioTracks_.clear(); }

  // ---- Common ---- //
  void TransportTrackFormat::print(std::ostream& os) const {
    os << get<TransportId>();
    if (has<TransportName>()) {
      os << " (TransportName=";
      os << get<TransportName>();
    }
    os << ", NumTracks=" << get<NumTracks>().get();
    os << ", NumIds=" << get<NumIds>().get();
    os << ")";
  }

}  // namespace adm
