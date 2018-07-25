#include <catch2/catch.hpp>
#include "adm/elements/speaker_position.hpp"

TEST_CASE("speaker_position") {
  using namespace adm;
  {
    auto speaker = SpeakerPosition(Azimuth(10), Elevation(45));

    REQUIRE(speaker.has<Azimuth>() == true);
    REQUIRE(speaker.has<Elevation>() == true);

    REQUIRE(speaker.get<Azimuth>() == 10);
    REQUIRE(speaker.get<Elevation>() == 45);
  }
  {
    auto speaker = SpeakerPosition(
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
    auto speaker =
        SpeakerPosition(Azimuth(10), AzimuthMin(5), AzimuthMax(15),
                        Elevation(45), ElevationMin(35), ElevationMax(55),
                        Distance(0.5f), DistanceMin(0.4f), DistanceMax(1.f));

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
