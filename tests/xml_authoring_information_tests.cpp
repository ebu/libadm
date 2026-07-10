#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements/audio_programme_ref_screen.hpp"
#include "adm/elements/authoring_information.hpp"
#include "adm/elements/coordinate_mode.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TEST_CASE("xml/authoring_information") {
  auto document = parseXml("xml_parser/authoring_information.xml");

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<AuthoringInformation>());
  auto info = programme->get<AuthoringInformation>();
  REQUIRE(info.get<ReferenceLayouts>().size() == 1);
  auto renderers = info.get<Renderers>();
  REQUIRE(renderers.size() == 1);
  auto const& r = renderers.at(0);
  REQUIRE(r.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(r.get<CoordinateMode>() == std::string{"polar"});
  REQUIRE(r.get<RendererPackFormatIdRefs>().size() == 2);

  REQUIRE(programme->has<AudioProgrammeReferenceScreen>());
  auto screen = programme->get<AudioProgrammeReferenceScreen>();
  REQUIRE(screen.has<CoordinateMode>());
  REQUIRE(screen.get<CoordinateMode>() == std::string{"cartesian"});

  std::stringstream xml;
  writeXml(xml, document);
  CHECK_THAT(xml.str(), EqualsXmlFile("authoring_information"));
}
