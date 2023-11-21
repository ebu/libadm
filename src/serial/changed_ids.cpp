#include "adm/serial/changed_ids.hpp"

namespace adm {
  namespace detail {
    template class VectorParameter<ChangedAudioChannelFormatIds>;
    template class VectorParameter<ChangedAudioPackFormatIds>;
    template class VectorParameter<ChangedAudioTrackUidIds>;
    template class VectorParameter<ChangedAudioTrackFormatIds>;
    template class VectorParameter<ChangedAudioStreamFormatIds>;
    template class VectorParameter<ChangedAudioObjectIds>;
    template class VectorParameter<ChangedAudioContentIds>;
    template class VectorParameter<ChangedAudioProgrammeIds>;
  }  // namespace detail
}  // namespace adm

std::string adm::formatValue(adm::StatusValue status) {
  switch (status) {
    case StatusValue::NEW:
      return "new";
    case StatusValue::CHANGED:
      return "changed";
    case StatusValue::EXTENDED:
      return "extended";
    case StatusValue::EXPIRED:
      return "expired";
  }
  throw std::runtime_error("Unsupported StatusValue");
}
