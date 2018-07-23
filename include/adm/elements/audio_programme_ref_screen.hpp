#pragma once
#include <ostream>

namespace adm {

  struct AudioProgrammeReferenceScreenTag {};

  class AudioProgrammeReferenceScreen {
   public:
    typedef AudioProgrammeReferenceScreenTag tag;

    AudioProgrammeReferenceScreen() {}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const { os << "()"; };
  };

}  // namespace adm
