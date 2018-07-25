#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_channel_format") {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00031002\" "
        "audioChannelFormatName=\"MyChannelFormat\" "
        "typeLabel=\"0003\""
        ">"
        "<frequency typeDefinition=\"lowPass\">120</frequency>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto channelFormats = document->getElements<AudioChannelFormat>();
    auto channelFormat = *channelFormats.begin();
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

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioChannelFormat "
      "audioChannelFormatID=\"AC_00031002\" "
      "audioChannelFormatName=\"MyChannelFormat\" "
      "typeLabel=\"0003\""
      ">"
      "<frequency typeDefinition=\"lowPass\">120</frequency>"
      "</audioChannelFormat>"
      "<audioChannelFormat "
      "audioChannelFormatID=\"AC_00031002\" "
      "audioChannelFormatName=\"MyChannelFormat\" "
      "typeLabel=\"0003\""
      ">"
      "<frequency typeDefinition=\"lowPass\">120</frequency>"
      "</audioChannelFormat>"
      "</audioFormatExtended>");
  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
