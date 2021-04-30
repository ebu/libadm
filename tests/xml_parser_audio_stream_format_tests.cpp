#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "test_config.hpp"

TEST_CASE("xml_parser/audio_stream_format") {
  using namespace adm;
  auto document = parseXml(data_file("xml_parser/audio_stream_format.xml"));
  auto streamFormat = document->lookup(parseAudioStreamFormatId("AS_00031001"));
  REQUIRE(streamFormat->get<AudioStreamFormatId>()
              .get<AudioStreamFormatIdValue>() == 0x1001u);
  REQUIRE(streamFormat->get<AudioStreamFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::OBJECTS);
  REQUIRE(streamFormat->get<AudioStreamFormatName>() == "MyStreamFormat");
  REQUIRE(streamFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
}

TEST_CASE("xml_parser/audio_stream_format_duplicate_id") {
  REQUIRE_THROWS_AS(adm::parseXml(data_file(
                        "xml_parser/audio_stream_format_duplicate_id.xml")),
                    adm::error::XmlParsingDuplicateId);
}
