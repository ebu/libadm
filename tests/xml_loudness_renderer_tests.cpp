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
