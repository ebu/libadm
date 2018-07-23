#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioContent
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_content) {
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

    BOOST_TEST(audioContent->has<AudioContentName>() == true);
    BOOST_TEST(audioContent->has<AudioContentId>() == true);
    BOOST_TEST(audioContent->has<LoudnessMetadata>() == false);
    BOOST_TEST(audioContent->has<DialogueId>() == false);

    BOOST_TEST(audioContent->get<AudioContentName>() == "MyContent");
    BOOST_TEST(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
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

    BOOST_TEST(audioContent->has<AudioContentName>() == true);
    BOOST_TEST(audioContent->has<AudioContentId>() == true);
    BOOST_TEST(audioContent->has<AudioContentLanguage>() == true);
    BOOST_TEST(audioContent->has<LoudnessMetadata>() == true);
    BOOST_TEST(audioContent->has<DialogueId>() == true);

    BOOST_TEST(audioContent->get<AudioContentName>() == "MyContent");
    BOOST_TEST(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
               0x1001u);
    BOOST_TEST(audioContent->get<AudioContentLanguage>() == "en");
    BOOST_TEST(audioContent->get<DialogueId>() == Dialogue::DIALOGUE);
    BOOST_TEST(audioContent->get<DialogueContentKind>() ==
               DialogueContent::VOICEOVER);
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
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
  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
