#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioObject
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_object) {
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

    BOOST_TEST(audioObject->has<AudioObjectName>() == true);
    BOOST_TEST(audioObject->has<AudioObjectId>() == true);

    BOOST_TEST(audioObject->get<AudioObjectName>() == "MyObject");
    BOOST_TEST(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() ==
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

    BOOST_TEST(audioObject->has<AudioObjectName>() == true);
    BOOST_TEST(audioObject->has<AudioObjectId>() == true);
    BOOST_TEST(audioObject->has<Start>() == true);
    BOOST_TEST(audioObject->has<Duration>() == true);
    BOOST_TEST(audioObject->has<DialogueId>() == true);
    BOOST_TEST(audioObject->has<Importance>() == true);
    BOOST_TEST(audioObject->has<Interact>() == true);
    BOOST_TEST(audioObject->has<DisableDucking>() == true);

    BOOST_TEST(audioObject->get<AudioObjectName>() == "MyObject");
    BOOST_TEST(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() ==
               0x1001u);
    BOOST_CHECK(audioObject->get<Start>() == std::chrono::seconds(0));
    BOOST_CHECK(audioObject->get<Duration>() == std::chrono::seconds(10));
    BOOST_TEST(audioObject->get<DialogueId>() == Dialogue::NON_DIALOGUE);
    BOOST_TEST(audioObject->get<Importance>() == 10);
    BOOST_TEST(audioObject->get<Interact>() == false);
    BOOST_TEST(audioObject->get<DisableDucking>() == true);
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
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

  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
