#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/xml_reader.hpp"
#include <iostream>

#define BOOST_TEST_MODULE XmlWriter
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

BOOST_AUTO_TEST_CASE(write_audio_object_interaction) {
  using namespace adm;

  auto admDocument =
      adm::parseXml("test_data/xml_writer_audio_object_interaction.xml");

  auto audioObjects = admDocument->getElements<AudioObject>();
  BOOST_TEST(audioObjects.size() == 2);

  BOOST_TEST(audioObjects[0]->has<AudioObjectInteraction>() == true);
  auto interaction_0 = audioObjects[0]->get<AudioObjectInteraction>();
  BOOST_TEST(interaction_0.get<OnOffInteract>() == true);
  BOOST_TEST(interaction_0.has<GainInteract>() == true);
  BOOST_TEST(interaction_0.get<GainInteract>() == true);
  BOOST_TEST(interaction_0.has<PositionInteract>() == true);
  BOOST_TEST(interaction_0.get<PositionInteract>() == true);
  BOOST_TEST(interaction_0.has<GainInteractionRange>() == true);
  auto gain_interaction_0 = interaction_0.get<GainInteractionRange>();
  BOOST_TEST(gain_interaction_0.has<GainInteractionMin>() == true);
  BOOST_TEST(gain_interaction_0.has<GainInteractionMax>() == true);
  BOOST_TEST(gain_interaction_0.get<GainInteractionMin>() == 0.5f);
  BOOST_TEST(gain_interaction_0.get<GainInteractionMax>() == 1.5f);
  BOOST_TEST(interaction_0.has<PositionInteractionRange>() == true);
  auto position_interaction_0 = interaction_0.get<PositionInteractionRange>();
  BOOST_TEST(position_interaction_0.has<AzimuthInteractionMin>() == true);
  BOOST_TEST(position_interaction_0.has<AzimuthInteractionMax>() == true);
  BOOST_TEST(position_interaction_0.get<AzimuthInteractionMin>() == -30.f);
  BOOST_TEST(position_interaction_0.get<AzimuthInteractionMax>() == 30.f);
  BOOST_TEST(position_interaction_0.has<ElevationInteractionMin>() == true);
  BOOST_TEST(position_interaction_0.has<ElevationInteractionMax>() == true);
  BOOST_TEST(position_interaction_0.get<ElevationInteractionMin>() == -45.f);
  BOOST_TEST(position_interaction_0.get<ElevationInteractionMax>() == 45.f);
  BOOST_TEST(position_interaction_0.has<DistanceInteractionMin>() == true);
  BOOST_TEST(position_interaction_0.has<DistanceInteractionMax>() == true);
  BOOST_TEST(position_interaction_0.get<DistanceInteractionMin>() == 0.5f);
  BOOST_TEST(position_interaction_0.get<DistanceInteractionMax>() == 1.5f);

  BOOST_TEST(audioObjects[1]->has<AudioObjectInteraction>() == true);
  auto interaction_1 = audioObjects[1]->get<AudioObjectInteraction>();
  BOOST_TEST(interaction_1.get<OnOffInteract>() == true);
  BOOST_TEST(interaction_1.has<GainInteract>() == true);
  BOOST_TEST(interaction_1.get<GainInteract>() == true);
  BOOST_TEST(interaction_1.has<PositionInteract>() == true);
  BOOST_TEST(interaction_1.get<PositionInteract>() == true);
  BOOST_TEST(interaction_1.has<GainInteractionRange>() == true);
  auto gain_interaction_1 = interaction_1.get<GainInteractionRange>();
  BOOST_TEST(gain_interaction_1.has<GainInteractionMin>() == true);
  BOOST_TEST(gain_interaction_1.has<GainInteractionMax>() == true);
  BOOST_TEST(gain_interaction_1.get<GainInteractionMin>() == 0.5f);
  BOOST_TEST(gain_interaction_1.get<GainInteractionMax>() == 1.5f);
  BOOST_TEST(interaction_1.has<PositionInteractionRange>() == true);
  auto position_interaction_1 = interaction_1.get<PositionInteractionRange>();
  BOOST_TEST(position_interaction_1.has<XInteractionMin>() == true);
  BOOST_TEST(position_interaction_1.has<XInteractionMax>() == true);
  BOOST_TEST(position_interaction_1.get<XInteractionMin>() == -1.f);
  BOOST_TEST(position_interaction_1.get<XInteractionMax>() == 1.f);
  BOOST_TEST(position_interaction_1.has<YInteractionMin>() == true);
  BOOST_TEST(position_interaction_1.has<YInteractionMax>() == true);
  BOOST_TEST(position_interaction_1.get<YInteractionMin>() == -1.f);
  BOOST_TEST(position_interaction_1.get<YInteractionMax>() == 1.f);
  BOOST_TEST(position_interaction_1.has<ZInteractionMin>() == true);
  BOOST_TEST(position_interaction_1.has<ZInteractionMax>() == true);
  BOOST_TEST(position_interaction_1.get<ZInteractionMin>() == -1.f);
  BOOST_TEST(position_interaction_1.get<ZInteractionMax>() == 1.f);
}
