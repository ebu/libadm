#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "test_config.hpp"

TEST_CASE("xml_parser/audio_programme") {
  {
    using namespace adm;
    auto document = parseXml(data_file("xml_parser/audio_programme.xml"));
    auto audioProgramme = document->lookup(parseAudioProgrammeId("APR_1001"));

    REQUIRE(audioProgramme->has<AudioProgrammeName>() == true);
    REQUIRE(audioProgramme->has<AudioProgrammeId>() == true);
    REQUIRE(audioProgramme->has<AudioProgrammeLanguage>() == true);
    REQUIRE(audioProgramme->has<Start>() == true);
    REQUIRE(audioProgramme->has<End>() == true);
    REQUIRE(audioProgramme->has<MaxDuckingDepth>() == true);
    REQUIRE(audioProgramme->has<LoudnessMetadata>() == true);
    REQUIRE(audioProgramme->has<AudioProgrammeReferenceScreen>() == false);

    REQUIRE(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
    REQUIRE(
        audioProgramme->get<AudioProgrammeId>().get<AudioProgrammeIdValue>() ==
        0x1001u);
    REQUIRE(audioProgramme->get<AudioProgrammeLanguage>() == "en");
    REQUIRE(audioProgramme->get<Start>().get() == std::chrono::seconds(0));
    REQUIRE(audioProgramme->get<End>().get() == std::chrono::seconds(10));
    REQUIRE(audioProgramme->get<MaxDuckingDepth>() == -15);
    auto loudnessMetadata = audioProgramme->get<LoudnessMetadata>();
    REQUIRE(loudnessMetadata.get<LoudnessMethod>() == "ITU-R BS.1770");
    REQUIRE(loudnessMetadata.get<LoudnessRecType>() == "EBU R128");
    REQUIRE(loudnessMetadata.get<LoudnessCorrectionType>() == "File-based");
    REQUIRE(loudnessMetadata.get<IntegratedLoudness>() == Approx(-23.f));
    REQUIRE(loudnessMetadata.get<LoudnessRange>() == Approx(10.0f));
    REQUIRE(loudnessMetadata.get<MaxTruePeak>() == Approx(-2.3f));
    REQUIRE(loudnessMetadata.get<MaxMomentary>() == Approx(-19.0f));
    REQUIRE(loudnessMetadata.get<MaxShortTerm>() == Approx(-21.2f));
    REQUIRE(loudnessMetadata.get<DialogueLoudness>() == Approx(-24.0f));
  }
}

TEST_CASE("xml_parser/audio_programme_duplicate_id") {
  REQUIRE_THROWS_AS(
      adm::parseXml(data_file("xml_parser/audio_programme_duplicate_id.xml")),
      adm::error::XmlParsingDuplicateId);
}
