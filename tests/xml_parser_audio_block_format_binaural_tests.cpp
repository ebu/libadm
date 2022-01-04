#include <sstream>
#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"

using namespace adm;

TEST_CASE("xml_parser/audio_block_format_hoa") {
  auto document = parseXml("xml_parser/audio_block_format_binaural.xml");
  auto channelFormat =
      document->lookup(parseAudioChannelFormatId("AC_00051001"));

  auto blockFormatsIterator =
      channelFormat->getElements<AudioBlockFormatBinaural>().begin();
  auto firstBlockFormat = *(blockFormatsIterator++);

  CHECK(firstBlockFormat.get<Rtime>().get() == std::chrono::seconds(0));
  CHECK(firstBlockFormat.get<Duration>().get() == std::chrono::seconds(1));
  CHECK(firstBlockFormat.get<Gain>().asLinear() == 0.5);
  CHECK(firstBlockFormat.get<Importance>() == 5);
}
