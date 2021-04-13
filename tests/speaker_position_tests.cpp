#include <catch2/catch.hpp>
#include "adm/elements/speaker_position.hpp"

TEST_CASE("spherical_speaker_position") {
  using namespace adm;
  {
    auto speaker = SphericalSpeakerPosition(Azimuth(10), Elevation(45));

    REQUIRE(speaker.has<Azimuth>() == true);
    REQUIRE(speaker.has<Elevation>() == true);

    REQUIRE(speaker.get<Azimuth>() == 10);
    REQUIRE(speaker.get<Elevation>() == 45);
  }
  {
    auto speaker = SphericalSpeakerPosition(
        Azimuth(10), Elevation(45),
        ScreenEdgeLock(VerticalEdge("top"), HorizontalEdge("right")));

    REQUIRE(speaker.has<Azimuth>() == true);
    REQUIRE(speaker.has<AzimuthMin>() == false);
    REQUIRE(speaker.has<AzimuthMax>() == false);
    REQUIRE(speaker.has<Elevation>() == true);
    REQUIRE(speaker.has<ElevationMin>() == false);
    REQUIRE(speaker.has<ElevationMax>() == false);
    REQUIRE(speaker.has<Distance>() == false);
    REQUIRE(speaker.has<DistanceMin>() == false);
    REQUIRE(speaker.has<DistanceMax>() == false);
    REQUIRE(speaker.has<ScreenEdgeLock>() == true);

    REQUIRE(speaker.get<Azimuth>().get() == Approx(10.f));
    REQUIRE(speaker.get<Elevation>().get() == Approx(45.f));
    REQUIRE(speaker.get<ScreenEdgeLock>().get<HorizontalEdge>() == "right");
    REQUIRE(speaker.get<ScreenEdgeLock>().get<VerticalEdge>() == "top");

    speaker.unset<ScreenEdgeLock>();
    REQUIRE(speaker.has<ScreenEdgeLock>() == false);
  }
  {
    auto speaker = SphericalSpeakerPosition(
        Azimuth(10), AzimuthMin(5), AzimuthMax(15), Elevation(45),
        ElevationMin(35), ElevationMax(55), Distance(0.5f), DistanceMin(0.4f),
        DistanceMax(1.f));

    REQUIRE(speaker.has<Azimuth>() == true);
    REQUIRE(speaker.has<AzimuthMin>() == true);
    REQUIRE(speaker.has<AzimuthMax>() == true);
    REQUIRE(speaker.has<Elevation>() == true);
    REQUIRE(speaker.has<ElevationMin>() == true);
    REQUIRE(speaker.has<ElevationMax>() == true);
    REQUIRE(speaker.has<Distance>() == true);
    REQUIRE(speaker.has<DistanceMin>() == true);
    REQUIRE(speaker.has<DistanceMax>() == true);
    REQUIRE(speaker.has<ScreenEdgeLock>() == false);

    REQUIRE(speaker.get<Azimuth>().get() == Approx(10));
    REQUIRE(speaker.get<AzimuthMin>().get() == Approx(5));
    REQUIRE(speaker.get<AzimuthMax>().get() == Approx(15));
    REQUIRE(speaker.get<Elevation>().get() == Approx(45));
    REQUIRE(speaker.get<ElevationMin>().get() == Approx(35));
    REQUIRE(speaker.get<ElevationMax>().get() == Approx(55));
    REQUIRE(speaker.get<Distance>().get() == Approx(0.5));
    REQUIRE(speaker.get<DistanceMin>().get() == Approx(0.4));
    REQUIRE(speaker.get<DistanceMax>().get() == Approx(1.0));

    speaker.unset<AzimuthMin>();
    speaker.unset<AzimuthMax>();
    speaker.unset<ElevationMin>();
    speaker.unset<ElevationMax>();
    speaker.unset<Distance>();
    speaker.unset<DistanceMin>();
    speaker.unset<DistanceMax>();

    REQUIRE(speaker.has<AzimuthMin>() == false);
    REQUIRE(speaker.has<AzimuthMax>() == false);
    REQUIRE(speaker.has<ElevationMin>() == false);
    REQUIRE(speaker.has<ElevationMax>() == false);
    REQUIRE(speaker.has<Distance>() == false);
    REQUIRE(speaker.has<DistanceMin>() == false);
    REQUIRE(speaker.has<DistanceMax>() == false);
  }
}

namespace {
  template <typename Attrib, typename SpeakerT>
  void testSetUnsetFloating(Attrib const& desiredAttrib, SpeakerT speakerPos) {
    REQUIRE(!speakerPos.template has<Attrib>());
    speakerPos.set(desiredAttrib);
    REQUIRE(speakerPos.template has<Attrib>());
    REQUIRE(speakerPos.template get<Attrib>().get() ==
            Approx(desiredAttrib.get()));
    speakerPos.template unset<Attrib>();
    REQUIRE(!speakerPos.template has<Attrib>());
  }
}  // namespace

TEST_CASE("cartesian_speaker_position") {
  using namespace adm;
  SECTION("default") {
    auto speaker = CartesianSpeakerPosition{};
    SECTION("coordinates") {
      REQUIRE(speaker.has<X>());
      REQUIRE(speaker.has<Y>());
      REQUIRE(!speaker.has<Z>());
    }
    SECTION("coordinate ranges") {
      REQUIRE(!speaker.has<XMin>());
      REQUIRE(!speaker.has<YMin>());
      REQUIRE(!speaker.has<ZMin>());
      REQUIRE(!speaker.has<XMax>());
      REQUIRE(!speaker.has<YMax>());
      REQUIRE(!speaker.has<ZMax>());
    }
    SECTION("screen edge lock") { REQUIRE(!speaker.has<ScreenEdgeLock>()); }
  }
  SECTION("pass parameters via ctor") {
    auto x = X{1.0f};
    auto y = Y{-1.0f};
    auto z = Z{0.0f};
    auto xMin = XMin{-1.0f};
    auto xMax = XMax{1.0f};
    auto yMin = YMin{-1.0f};
    auto yMax = YMax{1.0f};
    auto zMin = ZMin{-1.0f};
    auto zMax = ZMax{1.0f};
    auto edgeLock =
        ScreenEdgeLock(VerticalEdge("top"), HorizontalEdge("right"));
    auto speaker = CartesianSpeakerPosition(x, y, z, xMin, xMax, yMin, yMax,
                                            zMin, zMax, edgeLock);
    REQUIRE(speaker.get<X>().get() == Approx(x.get()));
    REQUIRE(speaker.get<Y>().get() == Approx(y.get()));
    REQUIRE(speaker.get<Z>().get() == Approx(z.get()));
    REQUIRE(speaker.has<XMin>());
    REQUIRE(speaker.has<XMax>());
    REQUIRE(speaker.has<YMin>());
    REQUIRE(speaker.has<YMax>());
    REQUIRE(speaker.has<ZMin>());
    REQUIRE(speaker.has<ZMax>());
    REQUIRE(speaker.has<ScreenEdgeLock>());
  }
  SECTION("set") {
    SECTION("coordinates") {
      auto speaker = CartesianSpeakerPosition();
      speaker.set(X{1.0f});
      speaker.set(Y{-1.0f});
      speaker.set(Z{0.f});

      REQUIRE(speaker.has<X>());
      REQUIRE(speaker.has<Y>());
      REQUIRE(speaker.has<Z>());

      REQUIRE(speaker.get<X>() == Approx(1));
      REQUIRE(speaker.get<Y>() == Approx(-1));
      REQUIRE(speaker.get<Z>() == Approx(0));
    }

    auto speaker = CartesianSpeakerPosition{};
    SECTION("and unset") {
      SECTION("ScreenEdgeLock") {
        auto edgeLock =
            ScreenEdgeLock(VerticalEdge("top"), HorizontalEdge("right"));
        speaker.set(edgeLock);
        REQUIRE(speaker.has<ScreenEdgeLock>());
        REQUIRE(speaker.get<ScreenEdgeLock>().get<VerticalEdge>() ==
                edgeLock.get<VerticalEdge>());
        REQUIRE(speaker.get<ScreenEdgeLock>().get<HorizontalEdge>() ==
                edgeLock.get<HorizontalEdge>());
        speaker.unset<ScreenEdgeLock>();
        REQUIRE(!speaker.has<ScreenEdgeLock>());
      }
      SECTION("MinMax") {
        auto min = -1.0f;
        auto max = 1.0f;
        testSetUnsetFloating(XMin{min}, speaker);
        testSetUnsetFloating(XMax{max}, speaker);
        testSetUnsetFloating(YMin{min}, speaker);
        testSetUnsetFloating(YMax{max}, speaker);
        testSetUnsetFloating(ZMin{min}, speaker);
        testSetUnsetFloating(ZMax{max}, speaker);
      }
    }
  }
}