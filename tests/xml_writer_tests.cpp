#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

std::shared_ptr<const adm::Document> createSimpleScene();

TEST_CASE("simple_scene_default") {
  using namespace adm;
  auto document = createSimpleScene();
  // with ebu core wrapper structure
  std::stringstream xml;
  writeXml(xml, document);
  CHECK_THAT(xml.str(), EqualsXmlFile("simple_scene_default"));
}

TEST_CASE("simple_scene_itu") {
  using namespace adm;
  auto document = createSimpleScene();

  std::stringstream xml;
  writeXml(xml, document,
           xml::WriterOptions::write_default_values |
               xml::WriterOptions::itu_structure);

  CHECK_THAT(xml.str(), EqualsXmlFile("simple_scene_itu"));
}

TEST_CASE("write_optional_defaults") {
  using namespace adm;
  auto document = createSimpleScene();

  std::stringstream xml;
  writeXml(xml, document, xml::WriterOptions::write_default_values);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_optional_defaults"));
}

TEST_CASE("write_complementary_audio_objects") {
  using namespace adm;

  auto audioObjectDefault = AudioObject::create(AudioObjectName("Default"));
  auto audioObjectComplementary =
      AudioObject::create(AudioObjectName("Complementary"));

  audioObjectDefault->addComplementary(audioObjectComplementary);

  auto document = Document::create();
  document->add(audioObjectDefault);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_complementary_audio_objects"));
}

TEST_CASE("write_object_attributes") {
  using namespace adm;

  auto audioObject = AudioObject::create(AudioObjectName("Default"));
  audioObject->set(Gain::fromLinear(0.5));

  auto document = Document::create();
  document->add(audioObject);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_object_attributes"));
}

std::shared_ptr<const adm::Document> createSimpleScene() {
  using namespace adm;
  auto document = Document::create();
  auto start = Start(parseTimecode("10:00:00.0"));
  auto end = End(parseTimecode("10:00:10.0"));
  auto programme =
      AudioProgramme::create(AudioProgrammeName("Main"), start, end);
  auto content = AudioContent::create(AudioContentName("Main"));
  programme->addReference(content);

  auto result = createSimpleObject("MainObject");
  content->addReference(result.audioObject);

  auto channel = result.audioChannelFormat;
  channel->add(AudioBlockFormatObjects(SphericalPosition(Azimuth(30)),
                                       Rtime(std::chrono::seconds(0)),
                                       JumpPosition(JumpPositionFlag(true))));
  channel->add(AudioBlockFormatObjects(
      SphericalPosition(Azimuth(-30)), Rtime(std::chrono::seconds(3)),
      JumpPosition(JumpPositionFlag(true),
                   InterpolationLength(std::chrono::seconds(1)))));
  channel->add(AudioBlockFormatObjects(SphericalPosition(Azimuth(0)),
                                       Rtime(std::chrono::seconds(6))));
  channel->add(AudioBlockFormatObjects(SphericalPosition(Azimuth(30)),
                                       Rtime(std::chrono::seconds(9))));

  document->add(programme);
  reassignIds(document);

  return document;
}
