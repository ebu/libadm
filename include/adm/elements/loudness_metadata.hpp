#pragma once
#include <string>
#include <ostream>

namespace adm {

  struct LoudnessMetadataTag {};

  class LoudnessMetadata {
   public:
    typedef LoudnessMetadataTag tag;

    LoudnessMetadata(){};

    std::string method;
    std::string recType;
    std::string correctionType;

    float integratedLoudness;
    float loudnessRange;
    float maxTruePeak;
    float maxMomentary;
    float maxShortTerm;
    float dialogueLoudness;

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const { os << "()"; };
  };
}  // namespace adm
