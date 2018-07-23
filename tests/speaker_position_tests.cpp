#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/speaker_position.hpp"

#define BOOST_TEST_MODULE SpeakerPosition
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(speaker_position, *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    auto speaker = SpeakerPosition(Azimuth(10), Elevation(45));

    BOOST_TEST(speaker.has<Azimuth>() == true);
    BOOST_TEST(speaker.has<Elevation>() == true);

    BOOST_TEST(speaker.get<Azimuth>() == 10);
    BOOST_TEST(speaker.get<Elevation>() == 45);
  }
  {
    auto speaker = SpeakerPosition(
        Azimuth(10), Elevation(45),
        ScreenEdgeLock(VerticalEdge("top"), HorizontalEdge("right")));

    BOOST_TEST(speaker.has<Azimuth>() == true);
    BOOST_TEST(speaker.has<AzimuthMin>() == false);
    BOOST_TEST(speaker.has<AzimuthMax>() == false);
    BOOST_TEST(speaker.has<Elevation>() == true);
    BOOST_TEST(speaker.has<ElevationMin>() == false);
    BOOST_TEST(speaker.has<ElevationMax>() == false);
    BOOST_TEST(speaker.has<Distance>() == false);
    BOOST_TEST(speaker.has<DistanceMin>() == false);
    BOOST_TEST(speaker.has<DistanceMax>() == false);
    BOOST_TEST(speaker.has<ScreenEdgeLock>() == true);

    BOOST_TEST(speaker.get<Azimuth>().get() == 10);
    BOOST_TEST(speaker.get<Elevation>().get() == 45);
    BOOST_TEST(speaker.get<ScreenEdgeLock>().get<HorizontalEdge>() == "right");
    BOOST_TEST(speaker.get<ScreenEdgeLock>().get<VerticalEdge>() == "top");

    speaker.unset<ScreenEdgeLock>();
    BOOST_TEST(speaker.has<ScreenEdgeLock>() == false);
  }
  {
    auto speaker =
        SpeakerPosition(Azimuth(10), AzimuthMin(5), AzimuthMax(15),
                        Elevation(45), ElevationMin(35), ElevationMax(55),
                        Distance(0.5f), DistanceMin(0.4f), DistanceMax(1.f));

    BOOST_TEST(speaker.has<Azimuth>() == true);
    BOOST_TEST(speaker.has<AzimuthMin>() == true);
    BOOST_TEST(speaker.has<AzimuthMax>() == true);
    BOOST_TEST(speaker.has<Elevation>() == true);
    BOOST_TEST(speaker.has<ElevationMin>() == true);
    BOOST_TEST(speaker.has<ElevationMax>() == true);
    BOOST_TEST(speaker.has<Distance>() == true);
    BOOST_TEST(speaker.has<DistanceMin>() == true);
    BOOST_TEST(speaker.has<DistanceMax>() == true);
    BOOST_TEST(speaker.has<ScreenEdgeLock>() == false);

    BOOST_TEST(speaker.get<Azimuth>().get() == 10);
    BOOST_TEST(speaker.get<AzimuthMin>().get() == 5);
    BOOST_TEST(speaker.get<AzimuthMax>().get() == 15);
    BOOST_TEST(speaker.get<Elevation>().get() == 45);
    BOOST_TEST(speaker.get<ElevationMin>().get() == 35);
    BOOST_TEST(speaker.get<ElevationMax>().get() == 55);
    BOOST_TEST(speaker.get<Distance>().get() == 0.5);
    BOOST_TEST(speaker.get<DistanceMin>().get() == 0.4);
    BOOST_TEST(speaker.get<DistanceMax>().get() == 1.0);

    speaker.unset<AzimuthMin>();
    speaker.unset<AzimuthMax>();
    speaker.unset<ElevationMin>();
    speaker.unset<ElevationMax>();
    speaker.unset<Distance>();
    speaker.unset<DistanceMin>();
    speaker.unset<DistanceMax>();

    BOOST_TEST(speaker.has<AzimuthMin>() == false);
    BOOST_TEST(speaker.has<AzimuthMax>() == false);
    BOOST_TEST(speaker.has<ElevationMin>() == false);
    BOOST_TEST(speaker.has<ElevationMax>() == false);
    BOOST_TEST(speaker.has<Distance>() == false);
    BOOST_TEST(speaker.has<DistanceMin>() == false);
    BOOST_TEST(speaker.has<DistanceMax>() == false);
  }
}
