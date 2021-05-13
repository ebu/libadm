#include <catch2/catch.hpp>
#include "adm/elements/position_offset.hpp"

TEST_CASE("position_offset") {
  using namespace adm;

  // Default contructor
  {
    SphericalPositionOffset offset;
    REQUIRE(offset.has<AzimuthOffset>() == true);
    REQUIRE(offset.has<ElevationOffset>() == true);
    REQUIRE(offset.has<DistanceOffset>() == true);

    REQUIRE(offset.isDefault<AzimuthOffset>() == true);
    REQUIRE(offset.isDefault<ElevationOffset>() == true);
    REQUIRE(offset.isDefault<DistanceOffset>() == true);

    REQUIRE(offset.get<AzimuthOffset>() == Approx(0.f));
    REQUIRE(offset.get<ElevationOffset>() == Approx(0.f));
    REQUIRE(offset.get<DistanceOffset>() == Approx(0.f));
  }
  {
    CartesianPositionOffset offset;
    REQUIRE(offset.has<XOffset>() == true);
    REQUIRE(offset.has<YOffset>() == true);
    REQUIRE(offset.has<ZOffset>() == true);

    REQUIRE(offset.isDefault<XOffset>() == true);
    REQUIRE(offset.isDefault<YOffset>() == true);
    REQUIRE(offset.isDefault<ZOffset>() == true);

    REQUIRE(offset.get<XOffset>() == Approx(0.f));
    REQUIRE(offset.get<YOffset>() == Approx(0.f));
    REQUIRE(offset.get<ZOffset>() == Approx(0.f));
  }

  // Cartesian position constructor
  {
    auto offset =
        CartesianPositionOffset(XOffset(.5f), YOffset(-.5f), ZOffset(.1f));

    REQUIRE(offset.get<XOffset>() == Approx(.5f));
    REQUIRE(offset.get<YOffset>() == Approx(-.5f));
    REQUIRE(offset.get<ZOffset>() == Approx(.1f));
  }

  // Spherical position constructor
  {
    auto offset = SphericalPositionOffset(
        AzimuthOffset(30.f), ElevationOffset(45.f), DistanceOffset(.9f));

    REQUIRE(offset.get<AzimuthOffset>() == Approx(30.f));
    REQUIRE(offset.get<ElevationOffset>() == Approx(45.f));
    REQUIRE(offset.get<DistanceOffset>() == Approx(.9f));
  }

  // PositionOffset variant
  {
    PositionOffset cartesianPositionOffset(
        CartesianPositionOffset(XOffset(1.f), YOffset(-1.f), ZOffset(0.0f)));
    PositionOffset sphericalPositionOffset(SphericalPositionOffset(
        AzimuthOffset(30.f), ElevationOffset(45.f), DistanceOffset(-0.2)));
  }
}