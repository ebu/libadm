#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioChannelFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_channel_format) {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00031002\" "
        "audioChannelFormatName=\"MyChannelFormat\" "
        "typeLabel=\"0003\""
        ">"
        "<frequency typeDefinition=\"lowPass\">120</frequency>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto channelFormats = document->getElements<AudioChannelFormat>();
    auto channelFormat = *channelFormats.begin();
    BOOST_TEST(channelFormat->get<AudioChannelFormatId>()
                   .get<AudioChannelFormatIdValue>() == 0x1002u);
    BOOST_TEST(
        channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::OBJECTS);
    BOOST_TEST(channelFormat->get<AudioChannelFormatName>() ==
               "MyChannelFormat");
    BOOST_TEST(channelFormat->get<TypeDescriptor>() == TypeDefinition::OBJECTS);
    auto frequency = channelFormat->get<Frequency>();
    BOOST_TEST(isLowPass(frequency) == true);
    BOOST_TEST(channelFormat->get<Frequency>().has<LowPass>() == true);
    BOOST_TEST(channelFormat->get<Frequency>().get<LowPass>() == 120.f);
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioChannelFormat "
      "audioChannelFormatID=\"AC_00031002\" "
      "audioChannelFormatName=\"MyChannelFormat\" "
      "typeLabel=\"0003\""
      ">"
      "<frequency typeDefinition=\"lowPass\">120</frequency>"
      "</audioChannelFormat>"
      "<audioChannelFormat "
      "audioChannelFormatID=\"AC_00031002\" "
      "audioChannelFormatName=\"MyChannelFormat\" "
      "typeLabel=\"0003\""
      ">"
      "<frequency typeDefinition=\"lowPass\">120</frequency>"
      "</audioChannelFormat>"
      "</audioFormatExtended>");
  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
