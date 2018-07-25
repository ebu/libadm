#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_track_format") {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioTrackFormat "
        "audioTrackFormatID=\"AT_00030001_01\" "
        "audioTrackFormatName=\"MyTrackFormat\" "
        "formatLabel=\"0001\""
        "formatDefinition=\"PCM\""
        ">"
        "</audioTrackFormat>"
        "</audioFormatExtended>");
    auto document = parseXml(admStream);
    auto trackFormats = document->getElements<AudioTrackFormat>();
    auto trackFormat = *trackFormats.begin();
    REQUIRE(
        trackFormat->get<AudioTrackFormatId>().get<AudioTrackFormatIdValue>() ==
        0x0001u);
    REQUIRE(trackFormat->get<AudioTrackFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::OBJECTS);
    REQUIRE(trackFormat->get<AudioTrackFormatId>()
                .get<AudioTrackFormatIdCounter>() == 0x01u);
    REQUIRE(trackFormat->get<AudioTrackFormatName>() == "MyTrackFormat");
    REQUIRE(trackFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioTrackFormat "
      "audioTrackFormatID=\"AT_00030001_01\" "
      "audioTrackFormatName=\"MyTrackFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioTrackFormat>"
      "<audioTrackFormat "
      "audioTrackFormatID=\"AT_00030001_01\" "
      "audioTrackFormatName=\"MyTrackFormat\" "
      "formatLabel=\"0001\""
      "formatDefinition=\"PCM\""
      ">"
      "</audioTrackFormat>"
      "</audioFormatExtended>");
  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
