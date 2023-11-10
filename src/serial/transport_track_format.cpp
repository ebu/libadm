#include "adm/serial/transport_track_format.hpp"

#include <algorithm>
#include <functional>
#include "adm/utilities/element_io.hpp"

namespace adm {

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
