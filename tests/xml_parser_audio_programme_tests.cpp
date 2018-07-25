#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_programme") {
  {
    using namespace adm;
    // Minimal
    {
      std::istringstream admStream(
          "<audioFormatExtended>"
          "<audioProgramme audioProgrammeID=\"APR_1001\" "
          "audioProgrammeName=\"MyProgramme\">"
          "</audioProgramme>"
          "</audioFormatExtended>");
      auto document = adm::parseXml(admStream);
      auto audioProgrammes = document->getElements<AudioProgramme>();
      auto audioProgramme = *audioProgrammes.begin();

      REQUIRE(audioProgramme->has<AudioProgrammeName>() == true);
      REQUIRE(audioProgramme->has<AudioProgrammeId>() == true);
      REQUIRE(audioProgramme->has<AudioProgrammeLanguage>() == false);
      REQUIRE(audioProgramme->has<Start>() == true);
      REQUIRE(audioProgramme->has<End>() == false);
      REQUIRE(audioProgramme->has<MaxDuckingDepth>() == false);
      REQUIRE(audioProgramme->has<LoudnessMetadata>() == false);
      REQUIRE(audioProgramme->has<AudioProgrammeReferenceScreen>() == false);

      REQUIRE(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
      REQUIRE(audioProgramme->get<AudioProgrammeId>()
                  .get<AudioProgrammeIdValue>() == 0x1001u);
    }
    // Full
    {
      std::istringstream admStream(
          "<audioFormatExtended>"
          "<audioProgramme "
          "audioProgrammeID=\"APR_1001\" "
          "audioProgrammeName=\"MyProgramme\" "
          "audioProgrammeLanguage=\"en\" "
          "start=\"00:00:00.00000\" "
          "end=\"00:00:10.00000\" "
          "maxDuckingDepth=\"-15\" "
          ">"
          "<audioContentIDRef>ACO_1001</audioContentIDRef>"
          "</audioProgramme>"
          "<audioContent "
          "audioContentID=\"ACO_1001\" "
          "audioContentName=\"MyContent\" "
          ">"
          "</audioContent>"
          "</audioFormatExtended>");
      auto document = adm::parseXml(admStream);
      auto audioProgrammes = document->getElements<AudioProgramme>();
      REQUIRE(audioProgrammes.size() == 1);
      auto audioProgramme = *audioProgrammes.begin();
      auto audioContents = document->getElements<AudioContent>();
      REQUIRE(audioContents.size() == 1);
      auto audioContent = *audioContents.begin();

      REQUIRE(audioProgramme->has<AudioProgrammeName>() == true);
      REQUIRE(audioProgramme->has<AudioProgrammeId>() == true);
      REQUIRE(audioProgramme->has<AudioProgrammeLanguage>() == true);
      REQUIRE(audioProgramme->has<Start>() == true);
      REQUIRE(audioProgramme->has<End>() == true);
      REQUIRE(audioProgramme->has<MaxDuckingDepth>() == true);
      REQUIRE(audioProgramme->has<LoudnessMetadata>() == false);
      REQUIRE(audioProgramme->has<AudioProgrammeReferenceScreen>() == false);
      REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 1);
      REQUIRE(audioProgramme->getReferences<AudioContent>()[0] == audioContent);

      REQUIRE(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
      REQUIRE(audioProgramme->get<AudioProgrammeId>()
                  .get<AudioProgrammeIdValue>() == 0x1001u);
      REQUIRE(audioProgramme->get<AudioProgrammeLanguage>() == "en");
      REQUIRE(audioProgramme->get<Start>().get() == std::chrono::seconds(0));
      REQUIRE(audioProgramme->get<End>().get() == std::chrono::seconds(10));
      REQUIRE(audioProgramme->get<MaxDuckingDepth>() == -15);
    }
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioProgramme "
      "audioProgrammeID=\"APR_1001\" "
      "audioProgrammeName=\"MyProgramme\" "
      "audioProgrammeLanguage=\"en\" "
      "start=\"00:00:00.00000\" "
      "end=\"00:00:10.00000\" "
      "maxDuckingDepth=\"-15\" "
      ">"
      "<audioContentIDRef>ACO_1001</audioContentIDRef>"
      "</audioProgramme>"

      "<audioProgramme "
      "audioProgrammeID=\"APR_1001\" "
      "audioProgrammeName=\"MyProgramme\" "
      "audioProgrammeLanguage=\"en\" "
      "start=\"00:00:00.00000\" "
      "end=\"00:00:10.00000\" "
      "maxDuckingDepth=\"-15\" "
      ">"
      "<audioContentIDRef>ACO_1001</audioContentIDRef>"
      "</audioProgramme>"
      "</audioFormatExtended>");

  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
