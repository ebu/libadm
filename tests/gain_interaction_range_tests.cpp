#include <catch2/catch.hpp>
#include "adm/elements/audio_object_interaction.hpp"

TEST_CASE("gain_interaction_range") {
  using namespace adm;

  GainInteractionRange gainRange;
  REQUIRE(gainRange.has<GainInteractionMin>() == false);
  REQUIRE(gainRange.has<GainInteractionMax>() == false);

  gainRange.set(GainInteractionMin(0.5f));
  gainRange.set(GainInteractionMax(1.5f));

  REQUIRE(gainRange.has<GainInteractionMin>() == true);
  REQUIRE(gainRange.has<GainInteractionMax>() == true);
  REQUIRE(gainRange.get<GainInteractionMin>() == Approx(0.5f));
  REQUIRE(gainRange.get<GainInteractionMax>() == Approx(1.5f));

  gainRange.unset<GainInteractionMin>();
  gainRange.unset<GainInteractionMax>();

  REQUIRE(gainRange.has<GainInteractionMin>() == false);
  REQUIRE(gainRange.has<GainInteractionMax>() == false);
}
