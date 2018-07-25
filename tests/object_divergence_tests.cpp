#include <catch2/catch.hpp>
#include "adm/elements/object_divergence.hpp"

TEST_CASE("object_divergence") {
  using namespace adm;
  {
    ObjectDivergence objectDivergence;

    REQUIRE(objectDivergence.has<Divergence>() == true);
    REQUIRE(objectDivergence.has<AzimuthRange>() == true);
    REQUIRE(objectDivergence.has<PositionRange>() == true);

    REQUIRE(objectDivergence.isDefault<Divergence>() == true);
    REQUIRE(objectDivergence.isDefault<AzimuthRange>() == true);
    REQUIRE(objectDivergence.isDefault<PositionRange>() == true);

    // NOTE: double brackets are necessary for booost 1.59
    REQUIRE(objectDivergence.get<Divergence>().get() == Approx(0.f));
    REQUIRE(objectDivergence.get<AzimuthRange>().get() == Approx(45.f));
    REQUIRE(objectDivergence.get<PositionRange>().get() == Approx(0.f));

    objectDivergence.set(Divergence(0.2f));
    objectDivergence.set(AzimuthRange(10.f));
    objectDivergence.set(PositionRange(0.1f));

    REQUIRE(objectDivergence.get<Divergence>().get() == Approx(0.2f));
    REQUIRE(objectDivergence.get<AzimuthRange>().get() == Approx(10.f));
    REQUIRE(objectDivergence.get<PositionRange>().get() == Approx(0.1f));

    REQUIRE(objectDivergence.isDefault<Divergence>() == false);
    REQUIRE(objectDivergence.isDefault<AzimuthRange>() == false);
    REQUIRE(objectDivergence.isDefault<PositionRange>() == false);

    REQUIRE_THROWS_AS(Divergence(2.f), OutOfRangeError);
    REQUIRE_THROWS_AS(AzimuthRange(200.f), OutOfRangeError);
    REQUIRE_THROWS_AS(PositionRange(2.f), OutOfRangeError);

    objectDivergence.unset<Divergence>();
    objectDivergence.unset<AzimuthRange>();
    objectDivergence.unset<PositionRange>();

    REQUIRE(objectDivergence.isDefault<Divergence>() == true);
    REQUIRE(objectDivergence.isDefault<AzimuthRange>() == true);
    REQUIRE(objectDivergence.isDefault<PositionRange>() == true);
  }
}
