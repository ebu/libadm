#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/xml_writer.hpp"
#include <iostream>

#define BOOST_TEST_MODULE XmlWriter
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

std::shared_ptr<const adm::Document> createSimpleScene();

BOOST_AUTO_TEST_CASE(default_options) {
  using namespace adm;
  auto doc = createSimpleScene();
  // with ebu core wrapper structure
  boost::test_tools::output_test_stream outputEbu(
      "test_data/xml_writer_simple_scene_ebu.xml", true);
  writeXml(outputEbu, doc);

  BOOST_TEST(outputEbu.match_pattern(false));
}

BOOST_AUTO_TEST_CASE(itu_envelope) {
  using namespace adm;
  auto doc = createSimpleScene();

  // with ITU wrapper structure
  boost::test_tools::output_test_stream outputItu(
      "test_data/xml_writer_simple_scene_itu.xml", true);
  writeXml(outputItu, doc,
           xml::WriterOptions::write_default_values |
               xml::WriterOptions::itu_structure);

  BOOST_TEST(outputItu.match_pattern(false));
}

BOOST_AUTO_TEST_CASE(write_optional_defaults) {
  using namespace adm;
  auto doc = createSimpleScene();

  boost::test_tools::output_test_stream output(
      "test_data/xml_writer_simple_scene_defaults.xml", true);
  writeXml(output, doc, xml::WriterOptions::write_default_values);

  BOOST_TEST(output.match_pattern(false));
}

BOOST_AUTO_TEST_CASE(write_complementary_audio_objects) {
  using namespace adm;

  auto audioObjectDefault = AudioObject::create(AudioObjectName("Default"));
  auto audioObjectComplementary =
      AudioObject::create(AudioObjectName("Complementary"));

  audioObjectDefault->addComplementary(audioObjectComplementary);

  auto document = Document::create();
  document->add(audioObjectDefault);

  boost::test_tools::output_test_stream output(
      "test_data/xml_writer_complementary_audio_objects.xml", true);
  writeXml(output, document);

  BOOST_TEST(output.match_pattern(false));
}

std::shared_ptr<const adm::Document> createSimpleScene() {
  using namespace adm;
  auto doc = Document::create();
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

  doc->add(programme);
  reassignIds(doc);

  return doc;
}
