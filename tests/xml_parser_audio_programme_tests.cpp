#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_programme") {
  {
    using namespace adm;
    auto document = parseXml("xml_parser/audio_programme.xml");
    auto audioProgramme = document->lookup(parseAudioProgrammeId("APR_1001"));

    REQUIRE(audioProgramme->has<AudioProgrammeName>() == true);
    REQUIRE(audioProgramme->has<AudioProgrammeId>() == true);
    REQUIRE(audioProgramme->has<AudioProgrammeLanguage>() == true);
    REQUIRE(audioProgramme->has<Start>() == true);
    REQUIRE(audioProgramme->has<End>() == true);
    REQUIRE(audioProgramme->has<MaxDuckingDepth>() == true);
    REQUIRE(audioProgramme->has<AudioProgrammeReferenceScreen>() == false);

    REQUIRE(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
    REQUIRE(
        audioProgramme->get<AudioProgrammeId>().get<AudioProgrammeIdValue>() ==
        0x1001u);
    REQUIRE(audioProgramme->get<AudioProgrammeLanguage>() == "en");
    REQUIRE(audioProgramme->get<Start>().get() == std::chrono::seconds(0));
    REQUIRE(audioProgramme->get<End>().get() == std::chrono::seconds(10));
    REQUIRE(audioProgramme->get<MaxDuckingDepth>() == -15);

    auto labels = audioProgramme->get<Labels>();
    REQUIRE(labels.size() == 2);
    REQUIRE(labels[0].get<LabelLanguage>() == "en");
    REQUIRE(labels[0].get<LabelValue>() == "Good morning");
    REQUIRE(labels[1].get<LabelLanguage>() == "fr");
    REQUIRE(labels[1].get<LabelValue>() == "Bonjour");
  }
}

TEST_CASE("xml_parser/audio_programme_duplicate_id") {
  REQUIRE_THROWS_AS(
      adm::parseXml("xml_parser/audio_programme_duplicate_id.xml"),
      adm::error::XmlParsingDuplicateId);
}
