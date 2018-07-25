#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/xml_reader.hpp"
#include <iostream>

TEST_CASE("write_audio_object_interaction") {
  using namespace adm;

  auto admDocument =
      adm::parseXml("write_audio_object_interaction.accepted.xml");

  auto audioObjects = admDocument->getElements<AudioObject>();
  REQUIRE(audioObjects.size() == 2);

  REQUIRE(audioObjects[0]->has<AudioObjectInteraction>() == true);
  auto interaction_0 = audioObjects[0]->get<AudioObjectInteraction>();
  REQUIRE(interaction_0.get<OnOffInteract>() == true);
  REQUIRE(interaction_0.has<GainInteract>() == true);
  REQUIRE(interaction_0.get<GainInteract>() == true);
  REQUIRE(interaction_0.has<PositionInteract>() == true);
  REQUIRE(interaction_0.get<PositionInteract>() == true);
  REQUIRE(interaction_0.has<GainInteractionRange>() == true);
  auto gain_interaction_0 = interaction_0.get<GainInteractionRange>();
  REQUIRE(gain_interaction_0.has<GainInteractionMin>() == true);
  REQUIRE(gain_interaction_0.has<GainInteractionMax>() == true);
  REQUIRE(gain_interaction_0.get<GainInteractionMin>() == Approx(0.5f));
  REQUIRE(gain_interaction_0.get<GainInteractionMax>() == Approx(1.5f));
  REQUIRE(interaction_0.has<PositionInteractionRange>() == true);
  auto position_interaction_0 = interaction_0.get<PositionInteractionRange>();
  REQUIRE(position_interaction_0.has<AzimuthInteractionMin>() == true);
  REQUIRE(position_interaction_0.has<AzimuthInteractionMax>() == true);
  REQUIRE(position_interaction_0.get<AzimuthInteractionMin>() == Approx(-30.f));
  REQUIRE(position_interaction_0.get<AzimuthInteractionMax>() == Approx(30.f));
  REQUIRE(position_interaction_0.has<ElevationInteractionMin>() == true);
  REQUIRE(position_interaction_0.has<ElevationInteractionMax>() == true);
  REQUIRE(position_interaction_0.get<ElevationInteractionMin>() ==
          Approx(-45.f));
  REQUIRE(position_interaction_0.get<ElevationInteractionMax>() ==
          Approx(45.f));
  REQUIRE(position_interaction_0.has<DistanceInteractionMin>() == true);
  REQUIRE(position_interaction_0.has<DistanceInteractionMax>() == true);
  REQUIRE(position_interaction_0.get<DistanceInteractionMin>() == Approx(0.5f));
  REQUIRE(position_interaction_0.get<DistanceInteractionMax>() == Approx(1.5f));

  REQUIRE(audioObjects[1]->has<AudioObjectInteraction>() == true);
  auto interaction_1 = audioObjects[1]->get<AudioObjectInteraction>();
  REQUIRE(interaction_1.get<OnOffInteract>() == true);
  REQUIRE(interaction_1.has<GainInteract>() == true);
  REQUIRE(interaction_1.get<GainInteract>() == true);
  REQUIRE(interaction_1.has<PositionInteract>() == true);
  REQUIRE(interaction_1.get<PositionInteract>() == true);
  REQUIRE(interaction_1.has<GainInteractionRange>() == true);
  auto gain_interaction_1 = interaction_1.get<GainInteractionRange>();
  REQUIRE(gain_interaction_1.has<GainInteractionMin>() == true);
  REQUIRE(gain_interaction_1.has<GainInteractionMax>() == true);
  REQUIRE(gain_interaction_1.get<GainInteractionMin>() == Approx(0.5f));
  REQUIRE(gain_interaction_1.get<GainInteractionMax>() == Approx(1.5f));
  REQUIRE(interaction_1.has<PositionInteractionRange>() == true);
  auto position_interaction_1 = interaction_1.get<PositionInteractionRange>();
  REQUIRE(position_interaction_1.has<XInteractionMin>() == true);
  REQUIRE(position_interaction_1.has<XInteractionMax>() == true);
  REQUIRE(position_interaction_1.get<XInteractionMin>() == Approx(-1.f));
  REQUIRE(position_interaction_1.get<XInteractionMax>() == Approx(1.f));
  REQUIRE(position_interaction_1.has<YInteractionMin>() == true);
  REQUIRE(position_interaction_1.has<YInteractionMax>() == true);
  REQUIRE(position_interaction_1.get<YInteractionMin>() == Approx(-1.f));
  REQUIRE(position_interaction_1.get<YInteractionMax>() == Approx(1.f));
  REQUIRE(position_interaction_1.has<ZInteractionMin>() == true);
  REQUIRE(position_interaction_1.has<ZInteractionMax>() == true);
  REQUIRE(position_interaction_1.get<ZInteractionMin>() == Approx(-1.f));
  REQUIRE(position_interaction_1.get<ZInteractionMax>() == Approx(1.f));
}
