#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/position.hpp"

#define BOOST_TEST_MODULE Position
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(position, *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  // Default contructor
  {
    SphericalPosition position;
    BOOST_TEST(position.has<Azimuth>() == true);
    BOOST_TEST(position.has<Elevation>() == true);
    BOOST_TEST(position.has<Distance>() == true);
    BOOST_TEST(position.has<ScreenEdgeLock>() == false);

    BOOST_TEST(position.isDefault<Azimuth>() == false);
    BOOST_TEST(position.isDefault<Elevation>() == false);
    BOOST_TEST(position.isDefault<Distance>() == true);
    BOOST_TEST(position.isDefault<ScreenEdgeLock>() == false);

    BOOST_TEST(position.get<Azimuth>() == 0.f);
    BOOST_TEST(position.get<Elevation>() == 0.f);
    BOOST_TEST(position.get<Distance>() == 1.f);
  }
  {
    CartesianPosition position;
    BOOST_TEST(position.has<X>() == true);
    BOOST_TEST(position.has<Y>() == true);
    BOOST_TEST(position.has<Z>() == true);
    BOOST_TEST(position.has<ScreenEdgeLock>() == false);

    BOOST_TEST(position.isDefault<X>() == false);
    BOOST_TEST(position.isDefault<Y>() == false);
    BOOST_TEST(position.isDefault<Z>() == true);
    BOOST_TEST(position.isDefault<ScreenEdgeLock>() == false);

    BOOST_TEST(position.get<X>() == 0.f);
    BOOST_TEST(position.get<Y>() == 1.f);
    BOOST_TEST(position.get<Z>() == 0.f);
  }
  // Full specification
  {
    SphericalPosition position(Azimuth(30.f), Elevation(45.f), Distance(0.5f),
                               ScreenEdgeLock(ScreenEdge("left")));
    BOOST_TEST(position.has<Azimuth>() == true);
    BOOST_TEST(position.has<Elevation>() == true);
    BOOST_TEST(position.has<Distance>() == true);
    BOOST_TEST(position.has<ScreenEdgeLock>() == true);

    BOOST_TEST(position.isDefault<Azimuth>() == false);
    BOOST_TEST(position.isDefault<Elevation>() == false);
    BOOST_TEST(position.isDefault<Distance>() == false);
    BOOST_TEST(position.isDefault<ScreenEdgeLock>() == false);

    BOOST_TEST(position.get<Azimuth>() == 30.f);
    BOOST_TEST(position.get<Elevation>() == 45.f);
    BOOST_TEST(position.get<Distance>() == 0.5f);
    BOOST_TEST(position.get<ScreenEdgeLock>().get<HorizontalEdge>() == "left");
  }
  {
    CartesianPosition position(X(1.f), Y(-1.f), Z(0.f),
                               ScreenEdgeLock(ScreenEdge("left")));
    BOOST_TEST(position.has<X>() == true);
    BOOST_TEST(position.has<Y>() == true);
    BOOST_TEST(position.has<Z>() == true);
    BOOST_TEST(position.has<ScreenEdgeLock>() == true);

    BOOST_TEST(position.isDefault<X>() == false);
    BOOST_TEST(position.isDefault<Y>() == false);
    BOOST_TEST(position.isDefault<Z>() == false);
    BOOST_TEST(position.isDefault<ScreenEdgeLock>() == false);

    BOOST_TEST(position.get<X>() == 1.f);
    BOOST_TEST(position.get<Y>() == -1.f);
    BOOST_TEST(position.get<Z>() == 0.f);
    BOOST_TEST(position.get<ScreenEdgeLock>().get<HorizontalEdge>() == "left");
  }
  // Position variant
  {
    Position cartesianPosition(CartesianPosition(X(1.f), Y(-1.f)));
    Position sphericalPosition(
        SphericalPosition(Azimuth(30.f), Elevation(45.f)));
  }
}
