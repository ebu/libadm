#include <catch2/catch.hpp>
#include "adm/elements/loudness_metadata.hpp"

TEST_CASE("loudness_metadata") {
  using namespace adm;
  {
    LoudnessMetadata loudness_metadata;
    REQUIRE(loudness_metadata.has<LoudnessMethod>() == false);
    REQUIRE(loudness_metadata.has<LoudnessRecType>() == false);
    REQUIRE(loudness_metadata.has<LoudnessCorrectionType>() == false);
    REQUIRE(loudness_metadata.has<IntegratedLoudness>() == false);
    REQUIRE(loudness_metadata.has<LoudnessRange>() == false);
    REQUIRE(loudness_metadata.has<MaxTruePeak>() == false);
    REQUIRE(loudness_metadata.has<MaxMomentary>() == false);
    REQUIRE(loudness_metadata.has<MaxShortTerm>() == false);
    REQUIRE(loudness_metadata.has<DialogueLoudness>() == false);

    REQUIRE(loudness_metadata.isDefault<LoudnessMethod>() == false);
    REQUIRE(loudness_metadata.isDefault<LoudnessRecType>() == false);
    REQUIRE(loudness_metadata.isDefault<LoudnessCorrectionType>() == false);
    REQUIRE(loudness_metadata.isDefault<IntegratedLoudness>() == false);
    REQUIRE(loudness_metadata.isDefault<LoudnessRange>() == false);
    REQUIRE(loudness_metadata.isDefault<MaxTruePeak>() == false);
    REQUIRE(loudness_metadata.isDefault<MaxMomentary>() == false);
    REQUIRE(loudness_metadata.isDefault<MaxShortTerm>() == false);
    REQUIRE(loudness_metadata.isDefault<DialogueLoudness>() == false);

    loudness_metadata.set(LoudnessMethod("ITU-R BS.1770"));
    loudness_metadata.set(LoudnessRecType("EBU R128"));
    loudness_metadata.set(LoudnessCorrectionType("File-based"));
    loudness_metadata.set(IntegratedLoudness(-23.f));
    loudness_metadata.set(LoudnessRange(10.f));
    loudness_metadata.set(MaxTruePeak(-2.3f));
    loudness_metadata.set(MaxMomentary(-19.f));
    loudness_metadata.set(MaxShortTerm(-21.2f));
    loudness_metadata.set(DialogueLoudness(-24.f));

    REQUIRE(loudness_metadata.get<LoudnessMethod>() == "ITU-R BS.1770");
    REQUIRE(loudness_metadata.get<LoudnessRecType>() == "EBU R128");
    REQUIRE(loudness_metadata.get<LoudnessCorrectionType>() == "File-based");
    REQUIRE(loudness_metadata.get<IntegratedLoudness>() == Approx(-23.f));
    REQUIRE(loudness_metadata.get<LoudnessRange>() == Approx(10.f));
    REQUIRE(loudness_metadata.get<MaxTruePeak>() == Approx(-2.3f));
    REQUIRE(loudness_metadata.get<MaxMomentary>() == Approx(-19.f));
    REQUIRE(loudness_metadata.get<MaxShortTerm>() == Approx(-21.2f));
    REQUIRE(loudness_metadata.get<DialogueLoudness>() == Approx(-24.f));

    loudness_metadata.unset<LoudnessMethod>();
    loudness_metadata.unset<LoudnessRecType>();
    loudness_metadata.unset<LoudnessCorrectionType>();
    loudness_metadata.unset<IntegratedLoudness>();
    loudness_metadata.unset<LoudnessRange>();
    loudness_metadata.unset<MaxTruePeak>();
    loudness_metadata.unset<MaxMomentary>();
    loudness_metadata.unset<MaxShortTerm>();
    loudness_metadata.unset<DialogueLoudness>();

    REQUIRE(loudness_metadata.has<LoudnessMethod>() == false);
    REQUIRE(loudness_metadata.has<LoudnessRecType>() == false);
    REQUIRE(loudness_metadata.has<LoudnessCorrectionType>() == false);
    REQUIRE(loudness_metadata.has<IntegratedLoudness>() == false);
    REQUIRE(loudness_metadata.has<LoudnessRange>() == false);
    REQUIRE(loudness_metadata.has<MaxTruePeak>() == false);
    REQUIRE(loudness_metadata.has<MaxMomentary>() == false);
    REQUIRE(loudness_metadata.has<MaxShortTerm>() == false);
    REQUIRE(loudness_metadata.has<DialogueLoudness>() == false);
  }
}
