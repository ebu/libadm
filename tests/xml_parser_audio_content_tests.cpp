#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_content") {
  using namespace adm;
  // Minimal
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioContent "
        "audioContentID=\"ACO_1001\" "
        "audioContentName=\"MyContent\" "
        ">"
        "</audioContent>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioContents = document->getElements<AudioContent>();
    auto audioContent = *audioContents.begin();

    REQUIRE(audioContent->has<AudioContentName>() == true);
    REQUIRE(audioContent->has<AudioContentId>() == true);
    REQUIRE(audioContent->has<LoudnessMetadata>() == false);
    REQUIRE(audioContent->has<DialogueId>() == false);

    REQUIRE(audioContent->get<AudioContentName>() == "MyContent");
    REQUIRE(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
            0x1001u);
  }

  // Full
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioContent "
        "audioContentID=\"ACO_1001\" "
        "audioContentName=\"MyContent\" "
        "audioContentLanguage=\"en\" "
        ">"
        "<dialogue dialogueContentKind=\"2\">1</dialogue>"
        "<loudnessMetadata "
        "loudnessMethod=\"BS.1770\""
        "loudnessRecType=\"R128\""
        "loudnessCorrectionType=\"R128\""
        ">"
        "<integratedLoudness>-23.0</integratedLoudness>"
        "<loudnessRange>10.0</loudnessRange>"
        "<maxTruePeak>-2.3</maxTruePeak>"
        "<maxMomentary>-19.0</maxMomentary>"
        "<maxShortTerm>-21.2</integratedLoudness>"
        "<dialogueLoudness>-24.0</dialogueLoudness>"
        "</loudnessMetadata>"
        "</audioContent>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioContents = document->getElements<AudioContent>();
    auto audioContent = *audioContents.begin();

    REQUIRE(audioContent->has<AudioContentName>() == true);
    REQUIRE(audioContent->has<AudioContentId>() == true);
    REQUIRE(audioContent->has<AudioContentLanguage>() == true);
    REQUIRE(audioContent->has<LoudnessMetadata>() == true);
    REQUIRE(audioContent->has<DialogueId>() == true);

    REQUIRE(audioContent->get<AudioContentName>() == "MyContent");
    REQUIRE(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
            0x1001u);
    REQUIRE(audioContent->get<AudioContentLanguage>() == "en");
    REQUIRE(audioContent->get<DialogueId>() == Dialogue::DIALOGUE);
    REQUIRE(audioContent->get<DialogueContentKind>() ==
            DialogueContent::VOICEOVER);
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioContent "
      "audioContentID=\"ACO_1001\" "
      "audioContentName=\"MyContent\" "
      "audioContentLanguage=\"en\" "
      ">"
      "</audioContent>"
      "<audioContent "
      "audioContentID=\"ACO_1001\" "
      "audioContentName=\"MyContent\" "
      "audioContentLanguage=\"en\" "
      ">"
      "</audioContent>"
      "</audioFormatExtended>");
  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
