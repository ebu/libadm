#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"

#define BOOST_TEST_MODULE XmlParserAudioBlockFormatDirectSpeakers
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_block_format_direct_speakers) {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00010001\" "
        "audioChannelFormatName=\"FrontLeft\" "
        "typeLabel=\"0001\" "
        "typeDefinition=\"DirectSpeakers\">"
        "<audioBlockFormat audioBlockFormatID=\"AB_00010001_00000001\">"
        "<speakerLabel>M+30</speakerLabel>"
        "<position coordinate=\"azimuth\">30.0</position>"
        "<position coordinate=\"elevation\">0.0</position>"
        "<position coordinate=\"distance\">1.0</position>"
        "</audioBlockFormat>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto channelFormats = document->getElements<AudioChannelFormat>();
    auto channelFormat = *channelFormats.begin();
    BOOST_TEST(channelFormat->get<AudioChannelFormatId>()
                   .get<AudioChannelFormatIdValue>() == 0x0001u);
    BOOST_TEST(
        channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(channelFormat->get<AudioChannelFormatName>() == "FrontLeft");
    BOOST_TEST(channelFormat->get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    auto firstBlockFormat =
        *(channelFormat->getElements<AudioBlockFormatDirectSpeakers>().begin());

    auto speakerLabels = firstBlockFormat.get<SpeakerLabels>();
    BOOST_TEST(*speakerLabels.begin() == "M+30");
    BOOST_TEST(firstBlockFormat.get<SpeakerPosition>().get<Azimuth>() == 30.0f);
    BOOST_TEST(firstBlockFormat.get<SpeakerPosition>().get<Elevation>() ==
               0.0f);
  }
}
