#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "test_config.hpp"

TEST_CASE("xml_parser/audio_block_format_direct_speakers") {
  using namespace adm;
  auto document =
      parseXml(data_file("xml_parser/audio_block_format_direct_speakers.xml"));
  auto channelFormat =
      document->lookup(parseAudioChannelFormatId("AC_00011001"));
  REQUIRE(channelFormat->get<AudioChannelFormatId>()
              .get<AudioChannelFormatIdValue>() == 0x1001u);
  REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS);
  REQUIRE(channelFormat->get<AudioChannelFormatName>() == "FrontLeft");
  REQUIRE(channelFormat->get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS);
  auto firstBlockFormat =
      *(channelFormat->getElements<AudioBlockFormatDirectSpeakers>().begin());

  auto speakerLabels = firstBlockFormat.get<SpeakerLabels>();
  REQUIRE(*speakerLabels.begin() == "M+30");
  auto speakerPosition = firstBlockFormat.get<SphericalSpeakerPosition>();
  REQUIRE(firstBlockFormat.get<SphericalSpeakerPosition>().get<Azimuth>() ==
          30.0f);
  REQUIRE(firstBlockFormat.get<SphericalSpeakerPosition>().get<Elevation>() ==
          0.0f);
}

TEST_CASE("xml_parser/audio_block_format_direct_speakers_cartesian") {
  using namespace adm;
  auto document = parseXml(
      data_file("xml_parser/audio_block_format_direct_speakers_cartesian.xml"));
  REQUIRE(!document->getElements<AudioChannelFormat>().empty());
  auto channelFormat =
      document->lookup(parseAudioChannelFormatId("AC_00011001"));
  SECTION("has correct audioChannelFormat") {
    REQUIRE(channelFormat->get<AudioChannelFormatId>()
                .get<AudioChannelFormatIdValue>() == 0x1001u);
    REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(channelFormat->get<AudioChannelFormatName>() == "Test");
    REQUIRE(channelFormat->get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
  }
  auto blockFormats =
      channelFormat->getElements<AudioBlockFormatDirectSpeakers>();
  SECTION("Has correct block format") {
    REQUIRE(!blockFormats.empty());
    auto firstBlockFormat =
        *(channelFormat->getElements<AudioBlockFormatDirectSpeakers>().begin());
    REQUIRE(firstBlockFormat.has<Rtime>());
    REQUIRE(firstBlockFormat.get<Rtime>().get() == std::chrono::seconds{1});
    REQUIRE(firstBlockFormat.has<Duration>());
    REQUIRE(firstBlockFormat.get<Duration>().get() == std::chrono::seconds{1});
    auto speakerLabels = firstBlockFormat.get<SpeakerLabels>();
    REQUIRE(speakerLabels.size() == 1);
    REQUIRE(*speakerLabels.begin() == "testLabel");
    auto speakerPosition = firstBlockFormat.get<CartesianSpeakerPosition>();
    REQUIRE(speakerPosition.get<X>() == Approx(0.0f));
    REQUIRE(speakerPosition.get<Y>() == Approx(0.0f));
    REQUIRE(speakerPosition.has<Z>());
    REQUIRE(speakerPosition.get<Z>() == Approx(0.5f));
    REQUIRE(speakerPosition.has<XMin>());
    REQUIRE(speakerPosition.get<XMin>() == Approx(-0.1f));
    REQUIRE(speakerPosition.has<XMax>());
    REQUIRE(speakerPosition.get<XMax>() == Approx(0.1f));
    REQUIRE(speakerPosition.has<YMin>());
    REQUIRE(speakerPosition.get<YMin>() == Approx(-0.1f));
    REQUIRE(speakerPosition.has<YMax>());
    REQUIRE(speakerPosition.get<YMax>() == Approx(0.1f));
    REQUIRE(speakerPosition.has<ZMin>());
    REQUIRE(speakerPosition.get<ZMin>() == Approx(0.4f));
    REQUIRE(speakerPosition.has<ZMax>());
    REQUIRE(speakerPosition.get<ZMax>() == Approx(0.6f));
    REQUIRE(speakerPosition.has<ScreenEdgeLock>());
    auto edgeLock = speakerPosition.get<ScreenEdgeLock>();
    REQUIRE(edgeLock.get<HorizontalEdge>().get() == "left");
  }
}

TEST_CASE("xml_parser/audio_block_format_direct_speakers_cartesian_bad_bound") {
  using namespace adm;
  REQUIRE_THROWS_AS(
      parseXml(data_file(
          "xml_parser/"
          "audio_block_format_direct_speakers_cartesian_bad_bound.xml")),
      error::XmlParsingError);
}

TEST_CASE("xml_parser/audio_block_format_direct_speakers_cartesian_bad_coord") {
  using namespace adm;
  REQUIRE_THROWS_AS(
      parseXml(data_file(
          "xml_parser/"
          "audio_block_format_direct_speakers_cartesian_bad_coord.xml")),
      error::XmlParsingError);
}
