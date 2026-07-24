#include <catch2/catch.hpp>

#include "adm/elements/authoring_information.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/loudness_renderer.hpp"

using namespace adm;

TEST_CASE("renderer_interop/renderer_to_loudness_renderer") {
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
                             RendererName("Rec. ITU-R BS.2127"),
                             RendererVersion("1.0.0"),
                             CoordinateMode("cartesian")};
  auto packA = AudioPackFormat::create(AudioPackFormatName("packA"),
                                       TypeDefinition::OBJECTS);
  auto packB = AudioPackFormat::create(AudioPackFormatName("packB"),
                                       TypeDefinition::OBJECTS);
  RendererPackFormatIdRefs packs{packA, packB};
  renderer.set(packs);

  auto loudnessRenderer = renderer.toLoudnessRenderer();

  REQUIRE(loudnessRenderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(loudnessRenderer.get<RendererName>() ==
          std::string{"Rec. ITU-R BS.2127"});
  REQUIRE(loudnessRenderer.get<RendererVersion>() == std::string{"1.0.0"});
  REQUIRE(loudnessRenderer.get<CoordinateMode>() == std::string{"cartesian"});
  REQUIRE(loudnessRenderer.has<RendererPackFormatIdRef>());
  REQUIRE(loudnessRenderer.get<RendererPackFormatIdRef>() == packA);
  REQUIRE(loudnessRenderer.get<RendererObjectIdRefs>().empty());
}

TEST_CASE(
    "renderer_interop/loudness_renderer_to_renderer_dropping_object_refs") {
  LoudnessRenderer loudnessRenderer{
      RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
      RendererName("Rec. ITU-R BS.2127"), RendererVersion("1.0.0"),
      CoordinateMode("polar")};
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  auto objectA = AudioObject::create(AudioObjectName("objectA"));
  auto objectB = AudioObject::create(AudioObjectName("objectB"));
  RendererPackFormatIdRef packRef{pack};
  RendererObjectIdRefs objects{objectA, objectB};
  loudnessRenderer.set(packRef);
  loudnessRenderer.set(objects);

  auto renderer = loudnessRenderer.toRendererDroppingObjectRefs(
      loudnessRenderer.get<RendererUri>());

  REQUIRE(renderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(renderer.get<RendererName>() == std::string{"Rec. ITU-R BS.2127"});
  REQUIRE(renderer.get<RendererVersion>() == std::string{"1.0.0"});
  REQUIRE(renderer.get<CoordinateMode>() == std::string{"polar"});
  REQUIRE(renderer.get<RendererPackFormatIdRefs>().size() == 1);

  auto roundTrip = renderer.toLoudnessRenderer();
  REQUIRE(roundTrip.get<RendererObjectIdRefs>().empty());
  REQUIRE(roundTrip.has<RendererPackFormatIdRef>());
  REQUIRE(roundTrip.get<RendererPackFormatIdRef>() == pack);
}

TEST_CASE("renderer_interop/loudness_metadata_accepts_renderer") {
  LoudnessMetadata loudnessMetadata;
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
                             CoordinateMode("polar")};
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  RendererPackFormatIdRefs packs{pack};
  renderer.set(packs);

  loudnessMetadata.set(renderer);

  REQUIRE(loudnessMetadata.has<LoudnessRenderer>());
  auto loudnessRenderer = loudnessMetadata.get<LoudnessRenderer>();
  REQUIRE(loudnessRenderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(loudnessRenderer.get<CoordinateMode>() == std::string{"polar"});
  REQUIRE(loudnessRenderer.has<RendererPackFormatIdRef>());
  REQUIRE(loudnessRenderer.get<RendererPackFormatIdRef>() == pack);
  REQUIRE(loudnessRenderer.get<RendererObjectIdRefs>().empty());
}