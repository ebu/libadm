#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/loudness_renderer.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TEST_CASE("xml/loudness_renderer") {
  auto document = parseXml("xml_parser/loudness_renderer.xml");

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<LoudnessMetadatas>());
  auto const& lms = programme->get<LoudnessMetadatas>();
  REQUIRE(lms.size() == 1);
  auto const& lm = lms.at(0);
  REQUIRE(lm.has<LoudnessRenderer>());
  auto renderer = lm.get<LoudnessRenderer>();
  REQUIRE(renderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(renderer.get<RendererName>() == std::string{"Rec. ITU-R BS.2127"});
  REQUIRE(renderer.get<RendererVersion>() == std::string{"1.0.0"});
  REQUIRE(renderer.get<CoordinateMode>() == std::string{"polar"});
  REQUIRE(renderer.get<RendererPackFormatIdRefs>().size() == 1);
  REQUIRE(renderer.get<RendererObjectIdRefs>().size() == 2);

  std::stringstream xml;
  writeXml(xml, document);
  CHECK_THAT(xml.str(), EqualsXmlFile("loudness_renderer"));
}

TEST_CASE("xml/loudness_renderer_forward_refs") {
  auto document = parseXml("xml_parser/loudness_renderer_forward_refs.xml");

  auto packA = document->lookup(parseAudioPackFormatId("AP_00031001"));
  auto packB = document->lookup(parseAudioPackFormatId("AP_00031002"));
  auto objectA = document->lookup(parseAudioObjectId("AO_1001"));
  auto objectB = document->lookup(parseAudioObjectId("AO_1002"));

  auto verifyRendererRefs = [&](LoudnessRenderer const& renderer) {
    REQUIRE(renderer.has<RendererPackFormatIdRefs>());
    auto packRefs = renderer.get<RendererPackFormatIdRefs>();
    REQUIRE(packRefs.size() == 2);
    REQUIRE(packRefs.at(0) == packA);
    REQUIRE(packRefs.at(1) == packB);

    REQUIRE(renderer.has<RendererObjectIdRefs>());
    auto objectRefs = renderer.get<RendererObjectIdRefs>();
    REQUIRE(objectRefs.size() == 2);
    REQUIRE(objectRefs.at(0) == objectA);
    REQUIRE(objectRefs.at(1) == objectB);
  };

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<LoudnessMetadatas>());
  auto programmeLms = programme->get<LoudnessMetadatas>();
  REQUIRE(programmeLms.size() == 1);
  REQUIRE(programmeLms.at(0).has<LoudnessRenderer>());
  verifyRendererRefs(programmeLms.at(0).get<LoudnessRenderer>());

  auto content = document->lookup(parseAudioContentId("ACO_1001"));
  REQUIRE(content->has<LoudnessMetadatas>());
  auto contentLms = content->get<LoudnessMetadatas>();
  REQUIRE(contentLms.size() == 1);
  REQUIRE(contentLms.at(0).has<LoudnessRenderer>());
  verifyRendererRefs(contentLms.at(0).get<LoudnessRenderer>());
}
