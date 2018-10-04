#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_stream_format") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_stream_format.xml");
  auto streamFormats = document->getElements<AudioStreamFormat>();
  auto streamFormat = *streamFormats.begin();
  REQUIRE(streamFormat->get<AudioStreamFormatId>()
              .get<AudioStreamFormatIdValue>() == 0x0001u);
  REQUIRE(streamFormat->get<AudioStreamFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::OBJECTS);
  REQUIRE(streamFormat->get<AudioStreamFormatName>() == "MyStreamFormat");
  REQUIRE(streamFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
}

TEST_CASE("xml_parser/audio_stream_format_duplicate_id") {
  REQUIRE_THROWS_AS(
      adm::parseXml("xml_parser/audio_stream_format_duplicate_id.xml"),
      adm::error::XmlParsingDuplicateId);
}
