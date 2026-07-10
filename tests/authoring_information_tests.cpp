#include <catch2/catch.hpp>
#include "adm/elements/authoring_information.hpp"
#include "adm/elements/audio_pack_format.hpp"

using namespace adm;

TEST_CASE("authoring_information/empty") {
  AuthoringInformation info;
  REQUIRE(info.get<ReferenceLayouts>().empty());
  REQUIRE(info.get<Renderers>().empty());
}

TEST_CASE("authoring_information/add_reference_layout") {
  AuthoringInformation info;
  info.add(ReferenceLayout{parseAudioPackFormatId("AP_00010003")});
  info.add(ReferenceLayout{parseAudioPackFormatId("AP_00010017")});
  REQUIRE(info.get<ReferenceLayouts>().size() == 2);
}

TEST_CASE("authoring_information/add_renderer") {
  AuthoringInformation info;
  Renderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
                    CoordinateMode("polar")};
  auto packA = AudioPackFormat::create(AudioPackFormatName("packA"),
                                       TypeDefinition::OBJECTS);
  auto packB = AudioPackFormat::create(AudioPackFormatName("packB"),
                                       TypeDefinition::OBJECTS);
  RendererPackFormatIdRefs packs{packA, packB};
  renderer.set(packs);
  info.add(renderer);
  auto renderers = info.get<Renderers>();
  REQUIRE(renderers.size() == 1);
  auto const& r = renderers.at(0);
  REQUIRE(r.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(r.get<CoordinateMode>() == std::string{"polar"});
  REQUIRE(r.get<RendererPackFormatIdRefs>().size() == 2);
}
