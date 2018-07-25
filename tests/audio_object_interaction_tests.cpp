#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_object_interaction.hpp"

TEST_CASE("audio_object_interaction") {
  using namespace adm;
  AudioObjectInteraction interaction(OnOffInteract(true));

  REQUIRE(interaction.has<OnOffInteract>() == true);
  REQUIRE(interaction.has<GainInteract>() == false);
  REQUIRE(interaction.has<PositionInteract>() == false);
  REQUIRE(interaction.has<GainInteractionRange>() == false);
  REQUIRE(interaction.has<PositionInteractionRange>() == false);

  REQUIRE(interaction.isDefault<OnOffInteract>() == false);
  REQUIRE(interaction.isDefault<GainInteract>() == false);
  REQUIRE(interaction.isDefault<PositionInteract>() == false);
  REQUIRE(interaction.isDefault<GainInteractionRange>() == false);
  REQUIRE(interaction.isDefault<PositionInteractionRange>() == false);

  REQUIRE(interaction.get<OnOffInteract>() == true);

  interaction.set(OnOffInteract(false));
  interaction.set(GainInteract(true));
  interaction.set(PositionInteract(true));
  interaction.set(GainInteractionRange());
  interaction.set(PositionInteractionRange());

  REQUIRE(interaction.has<OnOffInteract>() == true);
  REQUIRE(interaction.has<GainInteract>() == true);
  REQUIRE(interaction.has<PositionInteract>() == true);
  REQUIRE(interaction.has<GainInteractionRange>() == true);
  REQUIRE(interaction.has<PositionInteractionRange>() == true);

  interaction.unset<GainInteract>();
  interaction.unset<PositionInteract>();
  interaction.unset<GainInteractionRange>();
  interaction.unset<PositionInteractionRange>();

  REQUIRE(interaction.has<OnOffInteract>() == true);
  REQUIRE(interaction.has<GainInteract>() == false);
  REQUIRE(interaction.has<PositionInteract>() == false);
  REQUIRE(interaction.has<GainInteractionRange>() == false);
  REQUIRE(interaction.has<PositionInteractionRange>() == false);
}
