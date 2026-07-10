#include <catch2/catch.hpp>
#include "adm/elements/loudness_renderer.hpp"
#include "adm/elements/audio_pack_format_id.hpp"
#include "adm/elements/audio_object_id.hpp"

using namespace adm;

TEST_CASE("loudness_renderer/empty") {
  LoudnessRenderer renderer;
  REQUIRE(renderer.has<RendererUri>() == false);
  REQUIRE(renderer.has<RendererName>() == false);
  REQUIRE(renderer.has<RendererVersion>() == false);
  REQUIRE(renderer.has<CoordinateMode>() == false);
  REQUIRE(renderer.get<RendererPackFormatIdRefs>().empty());
  REQUIRE(renderer.get<RendererObjectIdRefs>().empty());
}

TEST_CASE("loudness_renderer/set_unset") {
  LoudnessRenderer renderer;
  renderer.set(RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"));
  renderer.set(RendererName("Rec. ITU-R BS.2127"));
  renderer.set(RendererVersion("1.0.0"));
  renderer.set(CoordinateMode("polar"));

  REQUIRE(renderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(renderer.get<RendererName>() == std::string{"Rec. ITU-R BS.2127"});
  REQUIRE(renderer.get<RendererVersion>() == std::string{"1.0.0"});
  REQUIRE(renderer.get<CoordinateMode>() == std::string{"polar"});

  renderer.unset<RendererUri>();
  renderer.unset<RendererName>();
  renderer.unset<RendererVersion>();
  renderer.unset<CoordinateMode>();

  REQUIRE(renderer.has<RendererUri>() == false);
  REQUIRE(renderer.has<RendererName>() == false);
  REQUIRE(renderer.has<RendererVersion>() == false);
  REQUIRE(renderer.has<CoordinateMode>() == false);
}

TEST_CASE("loudness_renderer/id_refs") {
  LoudnessRenderer renderer;
  RendererPackFormatIdRefs packs{parseAudioPackFormatId("AP_00010002")};
  RendererObjectIdRefs objects{parseAudioObjectId("AO_1001"),
                               parseAudioObjectId("AO_1002")};
  renderer.set(packs);
  renderer.set(objects);

  REQUIRE(renderer.get<RendererPackFormatIdRefs>().size() == 1);
  REQUIRE(renderer.get<RendererObjectIdRefs>().size() == 2);
}

TEST_CASE("loudness_renderer/named_args_constructor") {
  LoudnessRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
                            CoordinateMode("cartesian")};
  REQUIRE(renderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(renderer.get<CoordinateMode>() == std::string{"cartesian"});
}
