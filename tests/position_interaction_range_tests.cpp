#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/position_interaction_range.hpp"

#define BOOST_TEST_MODULE PositionInteractionRange
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(positionRange_interaction_range,
                     *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  // Default contructor
  {
    PositionInteractionRange positionRange;
    BOOST_TEST(positionRange.has<AzimuthInteractionMin>() == false);
    BOOST_TEST(positionRange.has<ElevationInteractionMin>() == false);
    BOOST_TEST(positionRange.has<DistanceInteractionMin>() == false);
    BOOST_TEST(positionRange.has<AzimuthInteractionMax>() == false);
    BOOST_TEST(positionRange.has<ElevationInteractionMax>() == false);
    BOOST_TEST(positionRange.has<DistanceInteractionMax>() == false);
    BOOST_TEST(positionRange.has<XInteractionMin>() == false);
    BOOST_TEST(positionRange.has<YInteractionMin>() == false);
    BOOST_TEST(positionRange.has<ZInteractionMin>() == false);
    BOOST_TEST(positionRange.has<XInteractionMax>() == false);
    BOOST_TEST(positionRange.has<YInteractionMax>() == false);
    BOOST_TEST(positionRange.has<ZInteractionMax>() == false);
  }
  // Full specification
  {
    PositionInteractionRange positionRange(
        AzimuthInteractionMin{-30.f}, ElevationInteractionMin{-45.f},
        DistanceInteractionMin{0.5f}, AzimuthInteractionMax{30.f},
        ElevationInteractionMax{45.f}, DistanceInteractionMax{1.5f});
    BOOST_TEST(positionRange.has<AzimuthInteractionMin>() == true);
    BOOST_TEST(positionRange.has<ElevationInteractionMin>() == true);
    BOOST_TEST(positionRange.has<DistanceInteractionMin>() == true);
    BOOST_TEST(positionRange.has<AzimuthInteractionMax>() == true);
    BOOST_TEST(positionRange.has<ElevationInteractionMax>() == true);
    BOOST_TEST(positionRange.has<DistanceInteractionMax>() == true);

    BOOST_TEST(positionRange.isDefault<AzimuthInteractionMin>() == false);
    BOOST_TEST(positionRange.isDefault<ElevationInteractionMin>() == false);
    BOOST_TEST(positionRange.isDefault<DistanceInteractionMin>() == false);
    BOOST_TEST(positionRange.isDefault<AzimuthInteractionMax>() == false);
    BOOST_TEST(positionRange.isDefault<ElevationInteractionMax>() == false);
    BOOST_TEST(positionRange.isDefault<DistanceInteractionMax>() == false);

    BOOST_TEST(positionRange.get<AzimuthInteractionMin>() == -30.f);
    BOOST_TEST(positionRange.get<ElevationInteractionMin>() == -45.f);
    BOOST_TEST(positionRange.get<DistanceInteractionMin>() == 0.5f);
    BOOST_TEST(positionRange.get<AzimuthInteractionMax>() == 30.f);
    BOOST_TEST(positionRange.get<ElevationInteractionMax>() == 45.f);
    BOOST_TEST(positionRange.get<DistanceInteractionMax>() == 1.5f);
  }
  {
    PositionInteractionRange positionRange(
        XInteractionMin{-1.f}, YInteractionMin{-1.f}, ZInteractionMin{-1.f},
        XInteractionMax{1.f}, YInteractionMax{1.f}, ZInteractionMax{1.f});
    BOOST_TEST(positionRange.has<XInteractionMin>() == true);
    BOOST_TEST(positionRange.has<YInteractionMin>() == true);
    BOOST_TEST(positionRange.has<ZInteractionMin>() == true);
    BOOST_TEST(positionRange.has<XInteractionMax>() == true);
    BOOST_TEST(positionRange.has<YInteractionMax>() == true);
    BOOST_TEST(positionRange.has<ZInteractionMax>() == true);

    BOOST_TEST(positionRange.isDefault<XInteractionMin>() == false);
    BOOST_TEST(positionRange.isDefault<YInteractionMin>() == false);
    BOOST_TEST(positionRange.isDefault<ZInteractionMin>() == false);
    BOOST_TEST(positionRange.isDefault<XInteractionMax>() == false);
    BOOST_TEST(positionRange.isDefault<YInteractionMax>() == false);
    BOOST_TEST(positionRange.isDefault<ZInteractionMax>() == false);

    BOOST_TEST(positionRange.get<XInteractionMin>() == -1.f);
    BOOST_TEST(positionRange.get<YInteractionMin>() == -1.f);
    BOOST_TEST(positionRange.get<ZInteractionMin>() == -1.f);
    BOOST_TEST(positionRange.get<XInteractionMax>() == 1.f);
    BOOST_TEST(positionRange.get<YInteractionMax>() == 1.f);
    BOOST_TEST(positionRange.get<ZInteractionMax>() == 1.f);
  }
}
