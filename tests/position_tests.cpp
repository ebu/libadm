#include <catch2/catch.hpp>
#include "adm/elements/position.hpp"

TEST_CASE("position") {
  using namespace adm;
  // Default contructor
  {
    SphericalPosition position;
    REQUIRE(position.has<Azimuth>() == true);
    REQUIRE(position.has<Elevation>() == true);
    REQUIRE(position.has<Distance>() == true);
    REQUIRE(position.has<ScreenEdgeLock>() == false);

    REQUIRE(position.isDefault<Azimuth>() == false);
    REQUIRE(position.isDefault<Elevation>() == false);
    REQUIRE(position.isDefault<Distance>() == true);
    REQUIRE(position.isDefault<ScreenEdgeLock>() == false);

    REQUIRE(position.get<Azimuth>() == Approx(0.f));
    REQUIRE(position.get<Elevation>() == Approx(0.f));
    REQUIRE(position.get<Distance>() == Approx(1.f));
  }
  {
    CartesianPosition position;
    REQUIRE(position.has<X>() == true);
    REQUIRE(position.has<Y>() == true);
    REQUIRE(position.has<Z>() == true);
    REQUIRE(position.has<ScreenEdgeLock>() == false);

    REQUIRE(position.isDefault<X>() == false);
    REQUIRE(position.isDefault<Y>() == false);
    REQUIRE(position.isDefault<Z>() == true);
    REQUIRE(position.isDefault<ScreenEdgeLock>() == false);

    REQUIRE(position.get<X>() == Approx(0.f));
    REQUIRE(position.get<Y>() == Approx(1.f));
    REQUIRE(position.get<Z>() == Approx(0.f));
  }
  // Full specification
  {
    SphericalPosition position(Azimuth(30.f), Elevation(45.f), Distance(0.5f),
                               ScreenEdgeLock(ScreenEdge("left")));
    REQUIRE(position.has<Azimuth>() == true);
    REQUIRE(position.has<Elevation>() == true);
    REQUIRE(position.has<Distance>() == true);
    REQUIRE(position.has<ScreenEdgeLock>() == true);

    REQUIRE(position.isDefault<Azimuth>() == false);
    REQUIRE(position.isDefault<Elevation>() == false);
    REQUIRE(position.isDefault<Distance>() == false);
    REQUIRE(position.isDefault<ScreenEdgeLock>() == false);

    REQUIRE(position.get<Azimuth>() == Approx(30.f));
    REQUIRE(position.get<Elevation>() == Approx(45.f));
    REQUIRE(position.get<Distance>() == Approx(0.5f));
    REQUIRE(position.get<ScreenEdgeLock>().get<HorizontalEdge>() == "left");
  }
  {
    CartesianPosition position(X(1.f), Y(-1.f), Z(0.f),
                               ScreenEdgeLock(ScreenEdge("left")));
    REQUIRE(position.has<X>() == true);
    REQUIRE(position.has<Y>() == true);
    REQUIRE(position.has<Z>() == true);
    REQUIRE(position.has<ScreenEdgeLock>() == true);

    REQUIRE(position.isDefault<X>() == false);
    REQUIRE(position.isDefault<Y>() == false);
    REQUIRE(position.isDefault<Z>() == false);
    REQUIRE(position.isDefault<ScreenEdgeLock>() == false);

    REQUIRE(position.get<X>() == Approx(1.f));
    REQUIRE(position.get<Y>() == Approx(-1.f));
    REQUIRE(position.get<Z>() == Approx(0.f));
    REQUIRE(position.get<ScreenEdgeLock>().get<HorizontalEdge>() == "left");
  }
  // Position variant
  {
    Position cartesianPosition(CartesianPosition(X(1.f), Y(-1.f)));
    Position sphericalPosition(
        SphericalPosition(Azimuth(30.f), Elevation(45.f)));
  }
}
