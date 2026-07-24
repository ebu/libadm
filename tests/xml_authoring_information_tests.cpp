#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements/audio_programme_ref_screen.hpp"
#include "adm/elements/authoring_information.hpp"
#include "adm/elements/coordinate_mode.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TEST_CASE("xml/authoring_information") {
  auto document = parseXml("xml_parser/authoring_information.xml");

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<AuthoringInformation>());
  auto info = programme->get<AuthoringInformation>();
  auto layouts = info.get<ReferenceLayouts>();
  REQUIRE(layouts.size() == 1);
  auto layoutPack = layouts.at(0).get();
  REQUIRE(layoutPack->get<AudioPackFormatId>() ==
          parseAudioPackFormatId("AP_00010003"));
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

TEST_CASE("xml/authoring_information_forward_pack_refs") {
  auto document =
      parseXml("xml_parser/authoring_information_forward_pack_refs.xml");

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<AuthoringInformation>());
  auto info = programme->get<AuthoringInformation>();
  REQUIRE(info.has<ReferenceLayouts>());
  auto layouts = info.get<ReferenceLayouts>();
  REQUIRE(layouts.size() == 1);
  auto layoutPack = layouts.at(0).get();
  REQUIRE(layoutPack->get<AudioPackFormatId>() ==
          parseAudioPackFormatId("AP_00031001"));

  auto renderers = info.get<Renderers>();
  REQUIRE(renderers.size() == 1);

  auto const& renderer = renderers.at(0);
  REQUIRE(renderer.has<RendererPackFormatIdRefs>());
  auto packRefs = renderer.get<RendererPackFormatIdRefs>();
  REQUIRE(packRefs.size() == 2);

  auto packA = document->lookup(parseAudioPackFormatId("AP_00031001"));
  REQUIRE(packA);
  auto packB = document->lookup(parseAudioPackFormatId("AP_00031002"));
  REQUIRE(packB);

  auto refA = packRefs.at(0);
  auto refB = packRefs.at(1);
  REQUIRE(refA == packA);
  REQUIRE(refB == packB);

  std::stringstream xml;
  writeXml(xml, document);
  REQUIRE(xml.str().find(
              "<audioPackFormatIDRef>AP_00031001</audioPackFormatIDRef>") !=
          std::string::npos);
}

TEST_CASE("xml/authoring_information_multiple_renderers") {
  auto document =
      parseXml("xml_parser/authoring_information_multiple_renderers.xml");

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<AuthoringInformation>());
  auto renderers = programme->get<AuthoringInformation>().get<Renderers>();
  REQUIRE(renderers.size() == 2);

  auto firstPackRefs = renderers.at(0).get<RendererPackFormatIdRefs>();
  REQUIRE(firstPackRefs.size() == 1);
  REQUIRE(firstPackRefs.at(0) ==
          document->lookup(parseAudioPackFormatId("AP_00010001")));

  auto secondPackRefs = renderers.at(1).get<RendererPackFormatIdRefs>();
  REQUIRE(secondPackRefs.size() == 1);
  REQUIRE(secondPackRefs.at(0) ==
          document->lookup(parseAudioPackFormatId("AP_00010002")));
}

TEST_CASE("xml/authoring_information_identical_renderers") {
  auto document =
      parseXml("xml_parser/authoring_information_identical_renderers.xml");

  auto programme = document->lookup(parseAudioProgrammeId("APR_1001"));
  REQUIRE(programme->has<AuthoringInformation>());
  auto renderers = programme->get<AuthoringInformation>().get<Renderers>();
  REQUIRE(renderers.size() == 1);

  auto packRefs = renderers.at(0).get<RendererPackFormatIdRefs>();
  REQUIRE(packRefs.size() == 1);
  REQUIRE(packRefs.at(0) ==
          document->lookup(parseAudioPackFormatId("AP_00010001")));
}

TEST_CASE("xml/authoring_information_renderer_requires_pack_refs") {
  REQUIRE_THROWS_AS(
      parseXml(
          "xml_parser/authoring_information_renderer_missing_pack_ref.xml"),
      error::XmlParsingError);
}

TEST_CASE("xml/reference_layout_rejects_multiple_pack_refs") {
  REQUIRE_THROWS_AS(
      parseXml("xml_parser/"
               "authoring_information_reference_layout_multiple_pack_refs.xml"),
      error::XmlParsingError);
}
