#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_pack_format") {
  using namespace adm;
  // Minimal
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioPackFormat "
        "audioPackFormatID=\"AP_00010001\" "
        "audioPackFormatName=\"MyPackFormat\" "
        "typeDefinition=\"DirectSpeakers\""
        "bitDepth=\"24\" "
        ">"
        "</audioPackFormat>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioPackFormats = document->getElements<AudioPackFormat>();
    auto audioPackFormat = *audioPackFormats.begin();

    REQUIRE(audioPackFormat->has<AudioPackFormatId>() == true);
    REQUIRE(audioPackFormat->has<AudioPackFormatName>() == true);

    REQUIRE(audioPackFormat->get<AudioPackFormatId>()
                .get<AudioPackFormatIdValue>() == 0x0001u);
    REQUIRE(audioPackFormat->get<AudioPackFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioPackFormat->get<AudioPackFormatName>() == "MyPackFormat");
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioPackFormat "
      "audioPackFormatID=\"AP_00010001\" "
      "audioPackFormatName=\"MyPackFormat\" "
      "typeDefinition=\"DirectSpeakers\""
      "bitDepth=\"24\" "
      ">"
      "</audioPackFormat>"
      "<audioPackFormat "
      "audioPackFormatID=\"AP_00010001\" "
      "audioPackFormatName=\"MyPackFormat\" "
      "typeDefinition=\"DirectSpeakers\""
      "bitDepth=\"24\" "
      ">"
      "</audioPackFormat>"
      "</audioFormatExtended>");
  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
