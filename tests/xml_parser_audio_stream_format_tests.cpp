#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_stream_format") {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioStreamFormat "
        "audioStreamFormatID=\"AS_00030001\" "
        "audioStreamFormatName=\"MyStreamFormat\" "
        "formatLabel=\"0001\""
        "formatDefinition=\"PCM\""
        ">"
        "</audioStreamFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto streamFormats = document->getElements<AudioStreamFormat>();
    auto streamFormat = *streamFormats.begin();
    REQUIRE(streamFormat->get<AudioStreamFormatId>()
                .get<AudioStreamFormatIdValue>() == 0x0001u);
    REQUIRE(streamFormat->get<AudioStreamFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::OBJECTS);
    REQUIRE(streamFormat->get<AudioStreamFormatName>() == "MyStreamFormat");
    REQUIRE(streamFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioStreamFormat "
      "audioStreamFormatID=\"AS_00030001\" "
      "audioStreamFormatName=\"MyStreamFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioStreamFormat>"
      "<audioStreamFormat "
      "audioStreamFormatID=\"AS_00030001\" "
      "audioStreamFormatName=\"MyStreamFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioStreamFormat>"
      "</audioFormatExtended>");
  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
