#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"

TEST_CASE("xml_parser/audio_block_format_hoa") {
  using namespace adm;
  auto document = parseXml("../test_data/xml_parser/audio_block_format_hoa.xml");
  auto channelFormat =
      document->lookup(parseAudioChannelFormatId("AC_00041001"));
  REQUIRE(channelFormat->get<AudioChannelFormatId>()
              .get<AudioChannelFormatIdValue>() == 0x1001u);
  REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::HOA);

  REQUIRE(channelFormat->get<AudioChannelFormatName>() == "HOA_Channel");
  REQUIRE(channelFormat->get<TypeDescriptor>() ==
          TypeDefinition::HOA);
  auto firstBlockFormat =
      *(channelFormat->getElements<AudioBlockFormatHoa>().begin());
  REQUIRE(firstBlockFormat.get<Order>() == 1);
  REQUIRE(firstBlockFormat.get<Degree>() == -1);
  REQUIRE(firstBlockFormat.get<NfcRefDist>() == 2.0f);


  //TODO: Add invalid range test
}
