#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"

#define BOOST_TEST_MODULE XmlParserAudioBlockFormatObjects
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_block_format_objects) {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00030001\" "
        "audioChannelFormatName=\"MyChannelFormat\" "
        "typeLabel=\"0003\" "
        "typeDefinition=\"Objects\">"
        "<audioBlockFormat audioBlockFormatID=\"AB_00010001_00000001\">"
        "<speakerLabel>M+30</speakerLabel>"
        "<position coordinate=\"azimuth\">30.0</position>"
        "<position coordinate=\"elevation\">0.0</position>"
        "<position coordinate=\"distance\">1.0</position>"
        "<width>45.0</width>"
        "<height>20.0</height>"
        "<depth>0.2</depth>"
        "<gain>0.8</gain>"
        "<diffuse>0.5</diffuse>"
        "<channelLock maxDistance=\"1.0\">1</channelLock>"
        "<objectDivergence "
        "azimuthRange=\"60.0\" "
        "positionRange=\"0.25\">0.5"
        "</objectDivergence>"
        "<jumpPosition interpolationLength=\"0.2\">1</jumpPosition>"
        "<importance>10</importance>"
        "</audioBlockFormat>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto channelFormats = document->getElements<AudioChannelFormat>();
    BOOST_TEST(channelFormats[0]
                   ->get<AudioChannelFormatId>()
                   .get<AudioChannelFormatIdValue>() == 0x0001u);
    BOOST_TEST(
        channelFormats[0]->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::OBJECTS);
    BOOST_TEST(channelFormats[0]->get<AudioChannelFormatName>() ==
               "MyChannelFormat");
    BOOST_TEST(channelFormats[0]->get<TypeDescriptor>() ==
               TypeDefinition::OBJECTS);

    auto firstBlockFormat =
        *(channelFormats[0]->getElements<AudioBlockFormatObjects>().begin());
    BOOST_TEST(firstBlockFormat.get<Width>().get() == 45.0f);
    BOOST_TEST(firstBlockFormat.get<Height>() == 20.0f);
    BOOST_TEST(firstBlockFormat.get<Depth>() == 0.2f);
    BOOST_TEST(firstBlockFormat.get<Gain>() == 0.8f);
    BOOST_TEST(firstBlockFormat.get<Diffuse>() == 0.5f);
    BOOST_TEST(firstBlockFormat.get<ChannelLock>().get<ChannelLockFlag>() ==
               true);
    BOOST_TEST(firstBlockFormat.get<ChannelLock>().get<MaxDistance>() == 1.f);
    BOOST_TEST(firstBlockFormat.get<ObjectDivergence>().get<Divergence>() ==
               0.5f);
    BOOST_TEST(firstBlockFormat.get<ObjectDivergence>().get<AzimuthRange>() ==
               60.f);
    BOOST_TEST(firstBlockFormat.get<ObjectDivergence>().get<PositionRange>() ==
               0.25f);
    BOOST_TEST(firstBlockFormat.get<JumpPosition>().get<JumpPositionFlag>() ==
               true);
    BOOST_CHECK(
        firstBlockFormat.get<JumpPosition>().get<InterpolationLength>() ==
        InterpolationLength(std::chrono::milliseconds(200)));
    // TODO: add zoneExclusion test
    // TODO: add screenRef test
    BOOST_TEST(firstBlockFormat.get<Importance>() == 10);
  }
}
