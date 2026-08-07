#include <catch2/catch.hpp>
#include "adm/document.hpp"
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
  auto packA = AudioPackFormat::create(AudioPackFormatName("packA"),
                                       TypeDefinition::OBJECTS);
  auto packB = AudioPackFormat::create(AudioPackFormatName("packB"),
                                       TypeDefinition::OBJECTS);
  info.add(ReferenceLayout{packA});
  info.add(ReferenceLayout{packB});
  auto layouts = info.get<ReferenceLayouts>();
  REQUIRE(layouts.size() == 2);
  REQUIRE(layouts.at(0).get() == packA);
  REQUIRE(layouts.at(1).get() == packB);
}

TEST_CASE(
    "authoring_information/reference_layout_adopts_unparented_reference") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  programme->set(AuthoringInformation{});
  document->add(programme);

  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  auto attachedInfo = programme->get<AuthoringInformation>();

  REQUIRE(attachedInfo.add(ReferenceLayout{pack}));
  REQUIRE(pack->getParent().lock() == document);
  REQUIRE(attachedInfo.get<ReferenceLayouts>().front().get() == pack);
}

TEST_CASE(
    "authoring_information/"
    "reference_layout_rejects_reference_from_another_document") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  programme->set(AuthoringInformation{});
  document->add(programme);

  auto otherDocument = Document::create();
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  otherDocument->add(pack);
  auto attachedInfo = programme->get<AuthoringInformation>();

  REQUIRE_THROWS_AS(attachedInfo.add(ReferenceLayout{pack}),
                    std::runtime_error);
}

TEST_CASE(
    "authoring_information/"
    "reference_layout_defers_reference_adoption_until_programme_add") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  AuthoringInformation info;

  REQUIRE(info.add(ReferenceLayout{pack}));
  programme->set(info);
  document->add(programme);

  REQUIRE(pack->getParent().lock() == document);
}

TEST_CASE("authoring_information/add_renderer") {
  AuthoringInformation info;
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer"),
                             CoordinateMode("polar")};
  auto packA = AudioPackFormat::create(AudioPackFormatName("packA"),
                                       TypeDefinition::OBJECTS);
  auto packB = AudioPackFormat::create(AudioPackFormatName("packB"),
                                       TypeDefinition::OBJECTS);
  renderer.addReference(packA);
  renderer.addReference(packB);
  info.add(renderer);
  auto renderers = info.get<Renderers>();
  REQUIRE(renderers.size() == 1);
  auto const& r = renderers.at(0);
  REQUIRE(r.get<RendererUri>() ==
          std::string{"urn:itu:bs:2127:0:itu_adm_renderer"});
  REQUIRE(r.get<CoordinateMode>() == std::string{"polar"});
  REQUIRE(r.getReferences<AudioPackFormat>().size() == 2);
}

TEST_CASE("authoring_renderer/references") {
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer")};
  auto packA = AudioPackFormat::create(AudioPackFormatName("packA"),
                                       TypeDefinition::OBJECTS);
  auto packB = AudioPackFormat::create(AudioPackFormatName("packB"),
                                       TypeDefinition::OBJECTS);

  REQUIRE(renderer.addReference(packA));
  REQUIRE(renderer.addReference(packB));
  REQUIRE(!renderer.addReference(packA));
  REQUIRE(renderer.getReferences<AudioPackFormat>().size() == 2);

  renderer.removeReference(packA);
  REQUIRE(renderer.getReferences<AudioPackFormat>().size() == 1);
  renderer.clearReferences<AudioPackFormat>();
  REQUIRE(renderer.getReferences<AudioPackFormat>().empty());
}

TEST_CASE("authoring_renderer/adopts_unparented_reference") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  AuthoringInformation info;
  programme->set(info);
  document->add(programme);

  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer")};
  renderer.addReference(pack);
  auto attachedInfo = programme->get<AuthoringInformation>();

  REQUIRE(attachedInfo.add(renderer));
  REQUIRE(pack->getParent().lock() == document);
  auto storedRenderer = attachedInfo.get<Renderers>().front();
  REQUIRE(storedRenderer.getParent().lock() == document);
}

TEST_CASE("authoring_renderer/rejects_reference_from_another_document") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  AuthoringInformation info;
  programme->set(info);
  document->add(programme);

  auto otherDocument = Document::create();
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  otherDocument->add(pack);
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer")};
  renderer.addReference(pack);
  auto attachedInfo = programme->get<AuthoringInformation>();

  REQUIRE_THROWS_AS(attachedInfo.add(renderer), std::runtime_error);
}

TEST_CASE("authoring_renderer/defers_reference_adoption_until_programme_add") {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto pack = AudioPackFormat::create(AudioPackFormatName("pack"),
                                      TypeDefinition::OBJECTS);
  AuthoringRenderer renderer{RendererUri("urn:itu:bs:2127:0:itu_adm_renderer")};
  renderer.addReference(pack);
  AuthoringInformation info;
  REQUIRE(info.add(renderer));
  programme->set(info);

  document->add(programme);

  REQUIRE(pack->getParent().lock() == document);
}
