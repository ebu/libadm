#include "adm/serial/transport_track_format.hpp"

#include <algorithm>
#include <functional>
#include "adm/utilities/element_io.hpp"

namespace adm {

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
