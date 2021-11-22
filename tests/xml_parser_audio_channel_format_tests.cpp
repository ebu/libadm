#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "test_config.hpp"

TEST_CASE("xml_parser/audio_channel_format") {
  using namespace adm;
  {
    auto document = parseXml(data_file("xml_parser/audio_channel_format.xml"));
    auto channelFormat =
        document->lookup(parseAudioChannelFormatId("AC_00031002"));

    REQUIRE(channelFormat->get<AudioChannelFormatId>()
                .get<AudioChannelFormatIdValue>() == 0x1002u);
    REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::OBJECTS);
    REQUIRE(channelFormat->get<AudioChannelFormatName>() == "MyChannelFormat");
    REQUIRE(channelFormat->get<TypeDescriptor>() == TypeDefinition::OBJECTS);
    auto frequency = channelFormat->get<Frequency>();
    REQUIRE(isLowPass(frequency) == true);
    REQUIRE(channelFormat->get<Frequency>().has<LowPass>() == true);
    REQUIRE(channelFormat->get<Frequency>().get<LowPass>() == Approx(120.f));
  }
}

TEST_CASE("xml_parser/audio_channel_format_duplicate_id") {
  REQUIRE_THROWS_AS(adm::parseXml(data_file(
                        "xml_parser/audio_channel_format_duplicate_id.xml")),
                    adm::error::XmlParsingDuplicateId);
}
