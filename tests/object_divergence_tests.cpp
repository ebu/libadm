#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/object_divergence.hpp"

#define BOOST_TEST_MODULE ObjectDivergence
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(object_divergence, *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    ObjectDivergence objectDivergence;

    BOOST_TEST(objectDivergence.has<Divergence>() == true);
    BOOST_TEST(objectDivergence.has<AzimuthRange>() == true);
    BOOST_TEST(objectDivergence.has<PositionRange>() == true);

    BOOST_TEST(objectDivergence.isDefault<Divergence>() == true);
    BOOST_TEST(objectDivergence.isDefault<AzimuthRange>() == true);
    BOOST_TEST(objectDivergence.isDefault<PositionRange>() == true);

    // NOTE: double brackets are necessary for booost 1.59
    BOOST_TEST((objectDivergence.get<Divergence>().get() == 0.f));
    BOOST_TEST(objectDivergence.get<AzimuthRange>().get() == 45.f);
    BOOST_TEST((objectDivergence.get<PositionRange>().get() == 0.f));

    objectDivergence.set(Divergence(0.2f));
    objectDivergence.set(AzimuthRange(10.f));
    objectDivergence.set(PositionRange(0.1f));

    BOOST_TEST(objectDivergence.get<Divergence>().get() == 0.2f);
    BOOST_TEST(objectDivergence.get<AzimuthRange>().get() == 10.f);
    BOOST_TEST(objectDivergence.get<PositionRange>().get() == 0.1f);

    BOOST_TEST(objectDivergence.isDefault<Divergence>() == false);
    BOOST_TEST(objectDivergence.isDefault<AzimuthRange>() == false);
    BOOST_TEST(objectDivergence.isDefault<PositionRange>() == false);

    BOOST_CHECK_THROW(Divergence(2.f), OutOfRangeError);
    BOOST_CHECK_THROW(AzimuthRange(200.f), OutOfRangeError);
    BOOST_CHECK_THROW(PositionRange(2.f), OutOfRangeError);

    objectDivergence.unset<Divergence>();
    objectDivergence.unset<AzimuthRange>();
    objectDivergence.unset<PositionRange>();

    BOOST_TEST(objectDivergence.isDefault<Divergence>() == true);
    BOOST_TEST(objectDivergence.isDefault<AzimuthRange>() == true);
    BOOST_TEST(objectDivergence.isDefault<PositionRange>() == true);
  }
}
