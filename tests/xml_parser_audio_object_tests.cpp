#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_object") {
  using namespace adm;
  // Minimal
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioObject "
        "audioObjectID=\"AO_1001\" "
        "audioObjectName=\"MyObject\" "
        ">"
        "</audioObject>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioObjects = document->getElements<AudioObject>();
    auto audioObject = *audioObjects.begin();

    REQUIRE(audioObject->has<AudioObjectName>() == true);
    REQUIRE(audioObject->has<AudioObjectId>() == true);

    REQUIRE(audioObject->get<AudioObjectName>() == "MyObject");
    REQUIRE(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() ==
            0x1001u);
  }

  // Full
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioObject "
        "audioObjectID=\"AO_1001\" "
        "audioObjectName=\"MyObject\" "
        "start=\"00:00:00.00000\" "
        "duration=\"00:00:10.00000\" "
        "dialogue=\"0\" "
        "importance=\"10\" "
        "interact=\"0\" "
        "disableDucking=\"1\" "
        ">"
        "</audioObject>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioObjects = document->getElements<AudioObject>();
    auto audioObject = *audioObjects.begin();

    REQUIRE(audioObject->has<AudioObjectName>() == true);
    REQUIRE(audioObject->has<AudioObjectId>() == true);
    REQUIRE(audioObject->has<Start>() == true);
    REQUIRE(audioObject->has<Duration>() == true);
    REQUIRE(audioObject->has<DialogueId>() == true);
    REQUIRE(audioObject->has<Importance>() == true);
    REQUIRE(audioObject->has<Interact>() == true);
    REQUIRE(audioObject->has<DisableDucking>() == true);

    REQUIRE(audioObject->get<AudioObjectName>() == "MyObject");
    REQUIRE(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() ==
            0x1001u);
    REQUIRE(audioObject->get<Start>().get() == std::chrono::seconds(0));
    REQUIRE(audioObject->get<Duration>().get() == std::chrono::seconds(10));
    REQUIRE(audioObject->get<DialogueId>() == Dialogue::NON_DIALOGUE);
    REQUIRE(audioObject->get<Importance>() == 10);
    REQUIRE(audioObject->get<Interact>() == false);
    REQUIRE(audioObject->get<DisableDucking>() == true);
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"

      "<audioObject "
      "audioObjectID=\"AO_1001\" "
      "audioObjectName=\"MyObject\" "
      "start=\"00:00:00.00000\" "
      "duration=\"00:00:10.00000\" "
      "dialogue=\"0\" "
      "importance=\"10\" "
      "interact=\"0\" "
      "disableDucking=\"1\" "
      ">"
      "</audioObject>"

      "<audioObject "
      "audioObjectID=\"AO_1001\" "
      "audioObjectName=\"MyObject2\" "
      "start=\"00:00:00.00000\" "
      "duration=\"00:00:10.00000\" "
      "dialogue=\"0\" "
      "importance=\"10\" "
      "interact=\"0\" "
      "disableDucking=\"1\" "
      ">"
      "</audioObject>"

      "</audioFormatExtended>");

  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
