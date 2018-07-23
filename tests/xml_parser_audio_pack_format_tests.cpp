#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioPackFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_pack_format) {
  using namespace adm;
  // Minimal
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioPackFormat "
        "audioPackFormatID=\"AP_00010001\" "
        "audioPackFormatName=\"MyPackFormat\" "
        "typeDefinition=\"DirectSpeakers\""
        "bitDepth=\"24\" "
        ">"
        "</audioPackFormat>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioPackFormats = document->getElements<AudioPackFormat>();
    auto audioPackFormat = *audioPackFormats.begin();

    BOOST_TEST(audioPackFormat->has<AudioPackFormatId>() == true);
    BOOST_TEST(audioPackFormat->has<AudioPackFormatName>() == true);

    BOOST_TEST(audioPackFormat->get<AudioPackFormatId>()
                   .get<AudioPackFormatIdValue>() == 0x0001u);
    BOOST_TEST(
        audioPackFormat->get<AudioPackFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioPackFormat->get<AudioPackFormatName>() == "MyPackFormat");
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioPackFormat "
      "audioPackFormatID=\"AP_00010001\" "
      "audioPackFormatName=\"MyPackFormat\" "
      "typeDefinition=\"DirectSpeakers\""
      "bitDepth=\"24\" "
      ">"
      "</audioPackFormat>"
      "<audioPackFormat "
      "audioPackFormatID=\"AP_00010001\" "
      "audioPackFormatName=\"MyPackFormat\" "
      "typeDefinition=\"DirectSpeakers\""
      "bitDepth=\"24\" "
      ">"
      "</audioPackFormat>"
      "</audioFormatExtended>");
  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
