#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioProgramme
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_programme) {
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

      BOOST_TEST(audioProgramme->has<AudioProgrammeName>() == true);
      BOOST_TEST(audioProgramme->has<AudioProgrammeId>() == true);
      BOOST_TEST(audioProgramme->has<AudioProgrammeLanguage>() == false);
      BOOST_TEST(audioProgramme->has<Start>() == true);
      BOOST_TEST(audioProgramme->has<End>() == false);
      BOOST_TEST(audioProgramme->has<MaxDuckingDepth>() == false);
      BOOST_TEST(audioProgramme->has<LoudnessMetadata>() == false);
      BOOST_TEST(audioProgramme->has<AudioProgrammeReferenceScreen>() == false);

      BOOST_TEST(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
      BOOST_TEST(audioProgramme->get<AudioProgrammeId>()
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
      BOOST_TEST(audioProgrammes.size() == 1);
      auto audioProgramme = *audioProgrammes.begin();
      auto audioContents = document->getElements<AudioContent>();
      BOOST_TEST(audioContents.size() == 1);
      auto audioContent = *audioContents.begin();

      BOOST_TEST(audioProgramme->has<AudioProgrammeName>() == true);
      BOOST_TEST(audioProgramme->has<AudioProgrammeId>() == true);
      BOOST_TEST(audioProgramme->has<AudioProgrammeLanguage>() == true);
      BOOST_TEST(audioProgramme->has<Start>() == true);
      BOOST_TEST(audioProgramme->has<End>() == true);
      BOOST_TEST(audioProgramme->has<MaxDuckingDepth>() == true);
      BOOST_TEST(audioProgramme->has<LoudnessMetadata>() == false);
      BOOST_TEST(audioProgramme->has<AudioProgrammeReferenceScreen>() == false);
      BOOST_TEST(audioProgramme->getReferences<AudioContent>().size() == 1);
      BOOST_TEST(audioProgramme->getReferences<AudioContent>()[0] ==
                 audioContent);

      BOOST_TEST(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
      BOOST_TEST(audioProgramme->get<AudioProgrammeId>()
                     .get<AudioProgrammeIdValue>() == 0x1001u);
      BOOST_TEST(audioProgramme->get<AudioProgrammeLanguage>() == "en");
      BOOST_CHECK(audioProgramme->get<Start>() == std::chrono::seconds(0));
      BOOST_CHECK(audioProgramme->get<End>() == std::chrono::seconds(10));
      BOOST_TEST(audioProgramme->get<MaxDuckingDepth>() == -15);
    }
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
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

  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
