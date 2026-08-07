#include <catch2/catch.hpp>
#include "adm/elements/audio_programme_ref_screen.hpp"
#include "adm/elements/coordinate_mode.hpp"

using namespace adm;

TEST_CASE("audio_programme_ref_screen/coordinate_mode") {
  AudioProgrammeReferenceScreen screen;
  REQUIRE(screen.has<CoordinateMode>() == false);
  screen.set(CoordinateMode("cartesian"));
  REQUIRE(screen.has<CoordinateMode>() == true);
  REQUIRE(screen.get<CoordinateMode>() == std::string{"cartesian"});
  screen.unset<CoordinateMode>();
  REQUIRE(screen.has<CoordinateMode>() == false);
}
