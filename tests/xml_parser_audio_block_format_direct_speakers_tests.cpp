#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"

TEST_CASE("xml_parser/audio_block_format_direct_speakers") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_block_format_direct_speakers.xml");
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
  auto speakerPosition = firstBlockFormat.get<SpeakerPosition>();
  REQUIRE(firstBlockFormat.get<SpeakerPosition>().get<Azimuth>() == 30.0f);
  REQUIRE(firstBlockFormat.get<SpeakerPosition>().get<Elevation>() == 0.0f);
}
