#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioStreamFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_stream_format) {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioStreamFormat "
        "audioStreamFormatID=\"AS_00030001\" "
        "audioStreamFormatName=\"MyStreamFormat\" "
        "formatLabel=\"0001\""
        "formatDefinition=\"PCM\""
        ">"
        "</audioStreamFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto streamFormats = document->getElements<AudioStreamFormat>();
    auto streamFormat = *streamFormats.begin();
    BOOST_TEST(streamFormat->get<AudioStreamFormatId>()
                   .get<AudioStreamFormatIdValue>() == 0x0001u);
    BOOST_TEST(streamFormat->get<AudioStreamFormatId>().get<TypeDescriptor>() ==
               TypeDefinition::OBJECTS);
    BOOST_TEST(streamFormat->get<AudioStreamFormatName>() == "MyStreamFormat");
    BOOST_TEST(streamFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioStreamFormat "
      "audioStreamFormatID=\"AS_00030001\" "
      "audioStreamFormatName=\"MyStreamFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioStreamFormat>"
      "<audioStreamFormat "
      "audioStreamFormatID=\"AS_00030001\" "
      "audioStreamFormatName=\"MyStreamFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioStreamFormat>"
      "</audioFormatExtended>");
  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
