#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements/loudness_renderer.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_object.hpp"

using namespace adm;

TEST_CASE("loudness_renderer/empty") {
  LoudnessRenderer renderer;
  REQUIRE(renderer.has<RendererUri>() == false);
  REQUIRE(renderer.has<RendererName>() == false);
  REQUIRE(renderer.has<RendererVersion>() == false);
  REQUIRE(renderer.has<CoordinateMode>() == false);
  REQUIRE(renderer.has<RendererPackFormatIdRef>() == false);
  REQUIRE(renderer.getReferences<AudioObject>().empty());
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
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  auto objectA = AudioObject::create(AudioObjectName("objectA"));
  auto objectB = AudioObject::create(AudioObjectName("objectB"));
  RendererPackFormatIdRef packRef{pack};
  renderer.set(packRef);
  REQUIRE(renderer.addReference(objectA));
  REQUIRE(renderer.addReference(objectB));

  REQUIRE(renderer.has<RendererPackFormatIdRef>());
  REQUIRE(renderer.get<RendererPackFormatIdRef>() == pack);
  REQUIRE(renderer.getReferences<AudioObject>().size() == 2);
}

TEST_CASE("loudness_renderer/named_args_constructor") {
  LoudnessRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
                            CoordinateMode("cartesian")};
  REQUIRE(renderer.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(renderer.get<CoordinateMode>() == std::string{"cartesian"});
}

TEST_CASE("loudness_renderer/references_different_document") {
  auto rendererDocument = Document::create();
  auto objectDocument = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto object = AudioObject::create(AudioObjectName("Object"));
  LoudnessRenderer renderer;
  LoudnessMetadata loudnessMetadata;
  loudnessMetadata.set(renderer);
  programme->set(LoudnessMetadatas{loudnessMetadata});

  rendererDocument->add(programme);
  objectDocument->add(object);

  auto storedRenderer =
      programme->get<LoudnessMetadatas>().at(0).get<LoudnessRenderer>();
  REQUIRE_THROWS_AS(storedRenderer.addReference(object), std::runtime_error);
}

TEST_CASE("loudness_renderer/adds_unparented_reference_to_parent_document") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto object = AudioObject::create(AudioObjectName("Object"));
  LoudnessRenderer renderer;
  LoudnessMetadata loudnessMetadata;
  loudnessMetadata.set(renderer);
  programme->set(LoudnessMetadatas{loudnessMetadata});

  document->add(programme);

  auto storedRenderer =
      programme->get<LoudnessMetadatas>().at(0).get<LoudnessRenderer>();
  REQUIRE(storedRenderer.addReference(object));
  REQUIRE(object->getParent().lock() == document);
}

TEST_CASE(
    "loudness_renderer/adds_unparented_reference_when_added_to_document") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto object = AudioObject::create(AudioObjectName("Object"));
  LoudnessRenderer renderer;
  renderer.addReference(object);
  LoudnessMetadata loudnessMetadata;
  loudnessMetadata.set(renderer);
  programme->set(LoudnessMetadatas{loudnessMetadata});

  document->add(programme);

  REQUIRE(object->getParent().lock() == document);
}

TEST_CASE("loudness_renderer/adds_reference_in_same_document") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto object = AudioObject::create(AudioObjectName("Object"));
  LoudnessRenderer renderer;
  LoudnessMetadata loudnessMetadata;
  loudnessMetadata.set(renderer);
  programme->set(LoudnessMetadatas{loudnessMetadata});

  document->add(object);
  document->add(programme);

  auto storedRenderer =
      programme->get<LoudnessMetadatas>().at(0).get<LoudnessRenderer>();
  REQUIRE(storedRenderer.addReference(object));
  REQUIRE(storedRenderer.getReferences<AudioObject>().size() == 1);
}
