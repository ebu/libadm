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

BOOST_AUTO_TEST_CASE(write_audio_object_interaction) {
  using namespace adm;

  auto audioObjectSpherical = AudioObject::create(AudioObjectName("Spherical"));
  audioObjectSpherical->set(Interact(true));
  audioObjectSpherical->set(AudioObjectInteraction(
      OnOffInteract(true), GainInteract(true), PositionInteract(true),
      GainInteractionRange(GainInteractionMin(0.5f), GainInteractionMax(1.5f)),
      PositionInteractionRange(
          AzimuthInteractionMin(-30.f), ElevationInteractionMin(-45.f),
          DistanceInteractionMin(0.5f), AzimuthInteractionMax(30.f),
          ElevationInteractionMax(45.f), DistanceInteractionMax(1.5f))));

  auto audioObjectCartesian = AudioObject::create(AudioObjectName("Cartesian"));
  audioObjectCartesian->set(Interact(true));
  audioObjectCartesian->set(AudioObjectInteraction(
      OnOffInteract(true), GainInteract(true), PositionInteract(true),
      GainInteractionRange(GainInteractionMin(0.5f), GainInteractionMax(1.5f)),
      PositionInteractionRange(XInteractionMin(-1.f), YInteractionMin(-1.f),
                               ZInteractionMin(-1.f), XInteractionMax(1.f),
                               YInteractionMax(1.f), ZInteractionMax(1.f))));

  auto document = Document::create();
  document->add(audioObjectSpherical);
  document->add(audioObjectCartesian);

  boost::test_tools::output_test_stream output(
      "test_data/xml_writer_audio_object_interaction.xml", true);
  writeXml(output, document);

  BOOST_TEST(output.match_pattern(false));
}
