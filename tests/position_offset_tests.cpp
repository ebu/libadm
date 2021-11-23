#include <catch2/catch.hpp>
#include "adm/elements/position_offset.hpp"
#include "helper/parameter_checks.hpp"

using namespace adm;

TEST_CASE("position_offset") {
  using namespace adm_test;

  SECTION("SphericalPositionOffset defaults") {
    SphericalPositionOffset offset;

    check_defaulted_param<AzimuthOffset>(offset,
                                         hasDefaultOf(AzimuthOffset(0.0f)),
                                         canBeSetTo(AzimuthOffset(1.0f)));
    check_defaulted_param<ElevationOffset>(offset,
                                           hasDefaultOf(ElevationOffset(0.0f)),
                                           canBeSetTo(ElevationOffset(1.0f)));
    check_defaulted_param<DistanceOffset>(offset,
                                          hasDefaultOf(DistanceOffset(0.0f)),
                                          canBeSetTo(DistanceOffset(1.0f)));
  }

  SECTION("CartesianPositionOffset defaults") {
    CartesianPositionOffset offset;

    check_defaulted_param<XOffset>(offset, hasDefaultOf(XOffset(0.0f)),
                                   canBeSetTo(XOffset(1.0f)));
    check_defaulted_param<YOffset>(offset, hasDefaultOf(YOffset(0.0f)),
                                   canBeSetTo(YOffset(1.0f)));
    check_defaulted_param<ZOffset>(offset, hasDefaultOf(ZOffset(0.0f)),
                                   canBeSetTo(ZOffset(1.0f)));
  }

  SECTION("CartesianPositionOffset constructor") {
    auto offset =
        CartesianPositionOffset(XOffset(.5f), YOffset(-.5f), ZOffset(.1f));

    REQUIRE(offset.get<XOffset>() == Approx(.5f));
    REQUIRE(offset.get<YOffset>() == Approx(-.5f));
    REQUIRE(offset.get<ZOffset>() == Approx(.1f));
  }

  SECTION("SphericalPositionOffset constructor") {
    auto offset = SphericalPositionOffset(
        AzimuthOffset(30.f), ElevationOffset(45.f), DistanceOffset(.9f));

    REQUIRE(offset.get<AzimuthOffset>() == Approx(30.f));
    REQUIRE(offset.get<ElevationOffset>() == Approx(45.f));
    REQUIRE(offset.get<DistanceOffset>() == Approx(.9f));
  }

  SECTION("PositionOffset variant") {
    PositionOffset cartesianPositionOffset(
        CartesianPositionOffset(XOffset(1.f), YOffset(-1.f), ZOffset(0.0f)));
    PositionOffset sphericalPositionOffset(SphericalPositionOffset(
        AzimuthOffset(30.f), ElevationOffset(45.f), DistanceOffset(-0.2)));
  }

  SECTION("print") {
    SECTION("SphericalPositionOffset") {
      SECTION("default") {
        auto sphericalOffset = SphericalPositionOffset();
        std::stringstream ss;
        sphericalOffset.print(ss);
        REQUIRE(ss.str() == "(azimuth=0, elevation=0, distance=0)");
      }

      SECTION("full") {
        auto sphericalOffset = SphericalPositionOffset(
            AzimuthOffset(1.0f), ElevationOffset(2.0f), DistanceOffset(0.3f));
        std::stringstream ss;
        sphericalOffset.print(ss);
        REQUIRE(ss.str() == "(azimuth=1, elevation=2, distance=0.3)");
      }
    }

    SECTION("CartesianPositionOffset") {
      SECTION("default") {
        auto cartesianOffset = CartesianPositionOffset();
        std::stringstream ss;
        cartesianOffset.print(ss);
        REQUIRE(ss.str() == "(X=0, Y=0, Z=0)");
      }

      SECTION("full") {
        auto cartesianOffset = CartesianPositionOffset(
            XOffset(0.1f), YOffset(0.2f), ZOffset(0.3f));
        std::stringstream ss;
        cartesianOffset.print(ss);
        REQUIRE(ss.str() == "(X=0.1, Y=0.2, Z=0.3)");
      }
    }
  }
}
