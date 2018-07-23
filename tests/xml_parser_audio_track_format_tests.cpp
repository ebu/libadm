#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioTrackFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_track_format) {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioTrackFormat "
        "audioTrackFormatID=\"AT_00030001_01\" "
        "audioTrackFormatName=\"MyTrackFormat\" "
        "formatLabel=\"0001\""
        "formatDefinition=\"PCM\""
        ">"
        "</audioTrackFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto trackFormats = document->getElements<AudioTrackFormat>();
    auto trackFormat = *trackFormats.begin();
    BOOST_TEST(
        trackFormat->get<AudioTrackFormatId>().get<AudioTrackFormatIdValue>() ==
        0x0001u);
    BOOST_TEST(trackFormat->get<AudioTrackFormatId>().get<TypeDescriptor>() ==
               TypeDefinition::OBJECTS);
    BOOST_TEST(trackFormat->get<AudioTrackFormatId>()
                   .get<AudioTrackFormatIdCounter>() == 0x01u);
    BOOST_TEST(trackFormat->get<AudioTrackFormatName>() == "MyTrackFormat");
    BOOST_TEST(trackFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioTrackFormat "
      "audioTrackFormatID=\"AT_00030001_01\" "
      "audioTrackFormatName=\"MyTrackFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioTrackFormat>"
      "<audioTrackFormat "
      "audioTrackFormatID=\"AT_00030001_01\" "
      "audioTrackFormatName=\"MyTrackFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioTrackFormat>"
      "</audioFormatExtended>");
  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
