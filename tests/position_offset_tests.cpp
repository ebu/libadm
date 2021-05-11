#include <catch2/catch.hpp>
#include "adm/elements/position_offset.hpp"

TEST_CASE("position_offset") {
  using namespace adm;

  // Default contructor
  {
    SphericalPositionOffset offset;
    REQUIRE(offset.has<Azimuth>() == true);
    REQUIRE(offset.has<Elevation>() == true);
    REQUIRE(offset.has<Distance>() == true);

    REQUIRE(offset.isDefault<Azimuth>() == false);
    REQUIRE(offset.isDefault<Elevation>() == false);
    REQUIRE(offset.isDefault<Distance>() == false);

    REQUIRE(offset.get<Azimuth>() == Approx(0.f));
    REQUIRE(offset.get<Elevation>() == Approx(0.f));
    REQUIRE(offset.get<Distance>() == Approx(0.f));
  }
  {
    CartesianPosition offset;
    REQUIRE(offset.has<X>() == true);
    REQUIRE(offset.has<Y>() == true);
    REQUIRE(offset.has<Z>() == true);

    REQUIRE(offset.isDefault<X>() == false);
    REQUIRE(offset.isDefault<Y>() == false);
    REQUIRE(offset.isDefault<Z>() == false);

    REQUIRE(offset.get<X>() == Approx(0.f));
    REQUIRE(offset.get<Y>() == Approx(0.f));
    REQUIRE(offset.get<Z>() == Approx(0.f));
  }

  // Cartesian position constructor
  {
    auto cartesianPosition = CartesianPositionOffset(X(.5f), Y(-.5f), Z(.1f));

    REQUIRE(cartesianPositon.get<X>() == Approx(.5f));
    REQUIRE(cartesianPositon.get<Y>() == Approx(-.5f));
    REQUIRE(cartesianPositon.get<Z>() == Approx(.1f));
  }

  // Spherical position constructor
  {
    auto sphericalPosition =
        SphericalPosition(Azimuth(30.f), Elevation(45.f), Distance(.9f));

    REQUIRE(sphericalPositon.get<Azimuth>() == Approx(30.f));
    REQUIRE(sphericalPositon.get<Elevation>() == Approx(45.f));
    REQUIRE(sphericalPositon.get<Distance>() == Approx(.9f));
  }

  // PositionOffset variant
  {
    PositionOffset cartesianPositionOffset(
        CartesianPositionOffset(X(1.f), Y(-1.f)));
    PositionOffset sphericalPositionOffset(
        SphericalPositionOffset(Azimuth(30.f), Elevation(45.f)));
  }
}