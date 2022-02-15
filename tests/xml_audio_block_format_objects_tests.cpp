#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TEST_CASE("xml/audio_block_format_objects") {
  auto document = parseXml("xml_parser/audio_block_format_objects.xml");
  auto channelFormat =
      document->lookup(parseAudioChannelFormatId("AC_00031001"));
  REQUIRE(channelFormat->get<AudioChannelFormatId>()
              .get<AudioChannelFormatIdValue>() == 0x1001u);
  REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::OBJECTS);
  REQUIRE(channelFormat->get<AudioChannelFormatName>() == "MyChannelFormat");
  REQUIRE(channelFormat->get<TypeDescriptor>() == TypeDefinition::OBJECTS);

  auto blocksIter =
      channelFormat->getElements<AudioBlockFormatObjects>().begin();

  auto firstBlockFormat = *blocksIter++;
  REQUIRE(firstBlockFormat.get<Width>().get() == Approx(45.0f));
  REQUIRE(firstBlockFormat.get<Height>() == Approx(20.0f));
  REQUIRE(firstBlockFormat.get<Depth>() == Approx(0.2f));
  REQUIRE(firstBlockFormat.get<Gain>().isLinear());
  REQUIRE(firstBlockFormat.get<Gain>().asLinear() == Approx(0.8f));
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
  REQUIRE(firstBlockFormat.get<JumpPosition>().get<JumpPositionFlag>() == true);
  REQUIRE(
      firstBlockFormat.get<JumpPosition>().get<InterpolationLength>().get() ==
      std::chrono::milliseconds(200));
  REQUIRE(firstBlockFormat.get<ScreenRef>() == true);
  // TODO: add zoneExclusion test
  REQUIRE(firstBlockFormat.get<Importance>() == 10);
  REQUIRE(firstBlockFormat.get<HeadphoneVirtualise>().get<Bypass>() == false);
  REQUIRE(firstBlockFormat.get<HeadphoneVirtualise>()
              .get<DirectToReverberantRatio>() == -60);

  auto secondBlockFormat = *blocksIter++;
  REQUIRE(secondBlockFormat.get<ScreenRef>() == false);
  REQUIRE(secondBlockFormat.get<JumpPosition>().get<JumpPositionFlag>() ==
          false);
  REQUIRE(secondBlockFormat.get<Gain>().isDb());
  REQUIRE(secondBlockFormat.get<Gain>().asDb() == -6.0);

  auto thirdBlockFormat = *blocksIter++;
  REQUIRE(thirdBlockFormat.get<Gain>().isLinear());
  REQUIRE(thirdBlockFormat.get<Gain>().asLinear() == 0.5);

  REQUIRE(secondBlockFormat.get<HeadphoneVirtualise>().get<Bypass>() == true);
  REQUIRE(secondBlockFormat.get<HeadphoneVirtualise>()
              .get<DirectToReverberantRatio>() == 60);

  SECTION("writer") {
    std::stringstream xml;
    writeXml(xml, document);

    CHECK_THAT(xml.str(), EqualsXmlFile("write_audio_block_format_objects"));
  }
}

TEST_CASE("xml_parser/audio_block_format_objects_gain_unit_error") {
  REQUIRE_THROWS_AS(
      parseXml("xml_parser/audio_block_format_objects_gain_unit_error.xml"),
      error::XmlParsingUnexpectedAttrError);
}
