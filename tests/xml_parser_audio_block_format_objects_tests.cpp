#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"

TEST_CASE("xml_parser/audio_block_format_objects") {
  using namespace adm;
  {
    auto document = parseXml("xml_parser/audio_block_format_objects.xml");
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
