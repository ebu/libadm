#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TEST_CASE("xml_time_format") {
  auto document = parseXml("xml_parser/time_format.xml");

  SECTION("parser") {
    SECTION("DirectSpeakers") {
      auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
      REQUIRE(programme);
      REQUIRE(programme->get<Start>().get() == FractionalTime{1, 48000});
      REQUIRE(programme->get<End>().get() == FractionalTime{123, 48000});
    }

    SECTION("DirectSpeakers") {
      auto channelFormat =
          document->lookup(parseAudioChannelFormatId("AC_00011001"));
      REQUIRE(channelFormat);
      auto firstBlockFormat = *(
          channelFormat->getElements<AudioBlockFormatDirectSpeakers>().begin());
      REQUIRE(firstBlockFormat.get<Rtime>().get() == FractionalTime{1, 2});
      REQUIRE(firstBlockFormat.get<Duration>().get() == FractionalTime{5, 4});
    }

    SECTION("Objects") {
      auto channelFormat =
          document->lookup(parseAudioChannelFormatId("AC_00031001"));
      REQUIRE(channelFormat);
      auto firstBlockFormat =
          *(channelFormat->getElements<AudioBlockFormatObjects>().begin());
      REQUIRE(firstBlockFormat.get<Rtime>().get() == FractionalTime{1, 2});
      REQUIRE(firstBlockFormat.get<Duration>().get() == FractionalTime{5, 4});
    }
  }

  SECTION("writer") {
    std::stringstream xml;
    writeXml(xml, document);

    CHECK_THAT(xml.str(), EqualsXmlFile("write_time_format"));
  }
}
