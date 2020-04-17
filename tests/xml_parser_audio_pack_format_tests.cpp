#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_pack_format") {
  using namespace adm;
  auto document = parseXml("../test_data/xml_parser/audio_pack_format.xml");
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
      adm::parseXml("../test_data/xml_parser/audio_pack_format_duplicate_id.xml"),
      adm::error::XmlParsingDuplicateId);
}


TEST_CASE("xml_parser/audio_pack_format_hoa") {
  using namespace adm;
  auto document = parseXml("../test_data/xml_parser/audio_pack_format_hoa.xml");
  auto audioPackFormatGeneric =
      document->lookup(parseAudioPackFormatId("AP_00041001"));

  std::shared_ptr<adm::AudioPackFormatHoa> audioPackFormatHoa =
      std::dynamic_pointer_cast<AudioPackFormatHoa>(audioPackFormatGeneric);

  REQUIRE(audioPackFormatHoa->has<AudioPackFormatId>() == true);
  REQUIRE(audioPackFormatHoa->has<AudioPackFormatName>() == true);
  REQUIRE(
      audioPackFormatHoa->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
      0x1001u);
  REQUIRE(audioPackFormatHoa->get<AudioPackFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::HOA);
  REQUIRE(audioPackFormatHoa->get<AudioPackFormatName>() == "MyPackFormat");
  REQUIRE(audioPackFormatHoa->get<Importance>() == 10);

  if(audioPackFormatHoa) {
      REQUIRE(audioPackFormatHoa->has<Normalization>() == true);
      REQUIRE(audioPackFormatHoa->has<ScreenRef>() == true);
      REQUIRE(audioPackFormatHoa->has<NfcRefDist>() == true);
      REQUIRE(audioPackFormatHoa->get<Normalization>() == "N3D");
      REQUIRE(audioPackFormatHoa->get<ScreenRef>() == true);
      REQUIRE(audioPackFormatHoa->get<NfcRefDist>() == 2);
  }
}

