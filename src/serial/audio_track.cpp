#include "adm/serial/audio_track.hpp"
#include "adm/utilities/element_io.hpp"

namespace adm {

  // ---- Common ---- //
  void AudioTrack::print(std::ostream& os) const {
    os << get<TrackId>();
    os << ", formatLabel=" << formatFormatLabel(get<FormatDescriptor>());
    os << ", formatDefinition="
       << formatFormatDefinition(get<FormatDescriptor>());
    os << ")";
  }

}  // namespace adm
