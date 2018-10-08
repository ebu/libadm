#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_pack_format") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_pack_format.xml");
  auto audioPackFormat =
      document->lookup(parseAudioPackFormatId("AP_00011001"));

  REQUIRE(audioPackFormat->has<AudioPackFormatId>() == true);
  REQUIRE(audioPackFormat->has<AudioPackFormatName>() == true);

  REQUIRE(
      audioPackFormat->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
      0x1001u);
  REQUIRE(audioPackFormat->get<AudioPackFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS);
  REQUIRE(audioPackFormat->get<AudioPackFormatName>() == "MyPackFormat");
  REQUIRE(audioPackFormat->get<Importance>() == 10);
}

TEST_CASE("xml_parser/audio_pack_format_duplicate_id") {
  REQUIRE_THROWS_AS(
      adm::parseXml("xml_parser/audio_pack_format_duplicate_id.xml"),
      adm::error::XmlParsingDuplicateId);
}
