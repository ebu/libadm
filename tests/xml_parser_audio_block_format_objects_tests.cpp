#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"

TEST_CASE("xml_parser_audio_block_format_objects") {
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
    REQUIRE(channelFormats[0]
                ->get<AudioChannelFormatId>()
                .get<AudioChannelFormatIdValue>() == 0x0001u);
    REQUIRE(
        channelFormats[0]->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::OBJECTS);
    REQUIRE(channelFormats[0]->get<AudioChannelFormatName>() ==
            "MyChannelFormat");
    REQUIRE(channelFormats[0]->get<TypeDescriptor>() ==
            TypeDefinition::OBJECTS);

    auto firstBlockFormat =
        *(channelFormats[0]->getElements<AudioBlockFormatObjects>().begin());
    REQUIRE(firstBlockFormat.get<Width>().get() == Approx(45.0f));
    REQUIRE(firstBlockFormat.get<Height>() == Approx(20.0f));
    REQUIRE(firstBlockFormat.get<Depth>() == Approx(0.2f));
    REQUIRE(firstBlockFormat.get<Gain>() == Approx(0.8f));
    REQUIRE(firstBlockFormat.get<Diffuse>() == Approx(0.5f));
    REQUIRE(firstBlockFormat.get<ChannelLock>().get<ChannelLockFlag>() == true);
    REQUIRE(firstBlockFormat.get<ChannelLock>().get<MaxDistance>() ==
            Approx(1.f));
    REQUIRE(firstBlockFormat.get<ObjectDivergence>().get<Divergence>() ==
            Approx(0.5f));
    REQUIRE(firstBlockFormat.get<ObjectDivergence>().get<AzimuthRange>() ==
            Approx(60.f));
    REQUIRE(firstBlockFormat.get<ObjectDivergence>().get<PositionRange>() ==
            Approx(0.25f));
    REQUIRE(firstBlockFormat.get<JumpPosition>().get<JumpPositionFlag>() ==
            true);
    REQUIRE(
        firstBlockFormat.get<JumpPosition>().get<InterpolationLength>().get() ==
        std::chrono::milliseconds(200));
    // TODO: add zoneExclusion test
    // TODO: add screenRef test
    REQUIRE(firstBlockFormat.get<Importance>() == 10);
  }
}
