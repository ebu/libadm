#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_stream_format") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_stream_format.xml");
  auto streamFormat = document->lookup(parseAudioStreamFormatId("AS_00031001"));
  REQUIRE(streamFormat->get<AudioStreamFormatId>()
              .get<AudioStreamFormatIdValue>() == 0x1001u);
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

TEST_CASE("xml_parser/audio_stream_format_forward_track_format_references") {
  using namespace adm;
  auto document =
      parseXml("xml_parser/audio_stream_format_forward_track_refs.xml");
  auto streamFormat = document->lookup(parseAudioStreamFormatId("AS_00031001"));

  auto trackA = document->lookup(parseAudioTrackFormatId("AT_00031001_01"));
  auto trackB = document->lookup(parseAudioTrackFormatId("AT_00031002_01"));

  auto trackRefs = streamFormat->getAudioTrackFormatReferences();
  REQUIRE(trackRefs.size() == 2);
  REQUIRE(trackRefs[0].lock() == trackA);
  REQUIRE(trackRefs[1].lock() == trackB);
}
