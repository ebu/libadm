#include <catch2/catch.hpp>
#include "adm/elements/position_interaction_range.hpp"

TEST_CASE("positionRange_interaction_range") {
  using namespace adm;
  // Default contructor
  {
    PositionInteractionRange positionRange;
    REQUIRE(positionRange.has<AzimuthInteractionMin>() == false);
    REQUIRE(positionRange.has<ElevationInteractionMin>() == false);
    REQUIRE(positionRange.has<DistanceInteractionMin>() == false);
    REQUIRE(positionRange.has<AzimuthInteractionMax>() == false);
    REQUIRE(positionRange.has<ElevationInteractionMax>() == false);
    REQUIRE(positionRange.has<DistanceInteractionMax>() == false);
    REQUIRE(positionRange.has<XInteractionMin>() == false);
    REQUIRE(positionRange.has<YInteractionMin>() == false);
    REQUIRE(positionRange.has<ZInteractionMin>() == false);
    REQUIRE(positionRange.has<XInteractionMax>() == false);
    REQUIRE(positionRange.has<YInteractionMax>() == false);
    REQUIRE(positionRange.has<ZInteractionMax>() == false);
  }
  // Full specification
  {
    PositionInteractionRange positionRange(
        AzimuthInteractionMin{-30.f}, ElevationInteractionMin{-45.f},
        DistanceInteractionMin{0.5f}, AzimuthInteractionMax{30.f},
        ElevationInteractionMax{45.f}, DistanceInteractionMax{1.5f});
    REQUIRE(positionRange.has<AzimuthInteractionMin>() == true);
    REQUIRE(positionRange.has<ElevationInteractionMin>() == true);
    REQUIRE(positionRange.has<DistanceInteractionMin>() == true);
    REQUIRE(positionRange.has<AzimuthInteractionMax>() == true);
    REQUIRE(positionRange.has<ElevationInteractionMax>() == true);
    REQUIRE(positionRange.has<DistanceInteractionMax>() == true);

    REQUIRE(positionRange.isDefault<AzimuthInteractionMin>() == false);
    REQUIRE(positionRange.isDefault<ElevationInteractionMin>() == false);
    REQUIRE(positionRange.isDefault<DistanceInteractionMin>() == false);
    REQUIRE(positionRange.isDefault<AzimuthInteractionMax>() == false);
    REQUIRE(positionRange.isDefault<ElevationInteractionMax>() == false);
    REQUIRE(positionRange.isDefault<DistanceInteractionMax>() == false);

    REQUIRE(positionRange.get<AzimuthInteractionMin>() == Approx(-30.f));
    REQUIRE(positionRange.get<ElevationInteractionMin>() == Approx(-45.f));
    REQUIRE(positionRange.get<DistanceInteractionMin>() == Approx(0.5f));
    REQUIRE(positionRange.get<AzimuthInteractionMax>() == Approx(30.f));
    REQUIRE(positionRange.get<ElevationInteractionMax>() == Approx(45.f));
    REQUIRE(positionRange.get<DistanceInteractionMax>() == Approx(1.5f));
  }
  {
    PositionInteractionRange positionRange(
        XInteractionMin{-1.f}, YInteractionMin{-1.f}, ZInteractionMin{-1.f},
        XInteractionMax{1.f}, YInteractionMax{1.f}, ZInteractionMax{1.f});
    REQUIRE(positionRange.has<XInteractionMin>() == true);
    REQUIRE(positionRange.has<YInteractionMin>() == true);
    REQUIRE(positionRange.has<ZInteractionMin>() == true);
    REQUIRE(positionRange.has<XInteractionMax>() == true);
    REQUIRE(positionRange.has<YInteractionMax>() == true);
    REQUIRE(positionRange.has<ZInteractionMax>() == true);

    REQUIRE(positionRange.isDefault<XInteractionMin>() == false);
    REQUIRE(positionRange.isDefault<YInteractionMin>() == false);
    REQUIRE(positionRange.isDefault<ZInteractionMin>() == false);
    REQUIRE(positionRange.isDefault<XInteractionMax>() == false);
    REQUIRE(positionRange.isDefault<YInteractionMax>() == false);
    REQUIRE(positionRange.isDefault<ZInteractionMax>() == false);

    REQUIRE(positionRange.get<XInteractionMin>() == Approx(-1.f));
    REQUIRE(positionRange.get<YInteractionMin>() == Approx(-1.f));
    REQUIRE(positionRange.get<ZInteractionMin>() == Approx(-1.f));
    REQUIRE(positionRange.get<XInteractionMax>() == Approx(1.f));
    REQUIRE(positionRange.get<YInteractionMax>() == Approx(1.f));
    REQUIRE(positionRange.get<ZInteractionMax>() == Approx(1.f));
  }
}
