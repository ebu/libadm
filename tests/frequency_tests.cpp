#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/frequency.hpp"

#define BOOST_TEST_MODULE Frequency
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(frequency, *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    Frequency frequency;
    BOOST_TEST(frequency.has<HighPass>() == false);
    BOOST_TEST(frequency.has<LowPass>() == false);

    BOOST_TEST(frequency.isDefault<HighPass>() == false);
    BOOST_TEST(frequency.isDefault<LowPass>() == false);

    frequency.set(HighPass(300.f));
    frequency.set(LowPass(3300.f));

    BOOST_TEST(frequency.get<HighPass>() == 300.f);
    BOOST_TEST(frequency.get<LowPass>() == 3300.f);

    BOOST_TEST(frequency.isDefault<HighPass>() == false);
    BOOST_TEST(frequency.isDefault<LowPass>() == false);

    frequency.unset<HighPass>();
    frequency.unset<LowPass>();
  }

  // lowpass
  {
    Frequency frequency(LowPass(120.f));
    BOOST_TEST(isLowPass(frequency) == true);
    BOOST_TEST(isHighPass(frequency) == false);
    BOOST_TEST(isBandPass(frequency) == false);
  }

  // highpass
  {
    Frequency frequency(HighPass(120.f));
    BOOST_TEST(isLowPass(frequency) == false);
    BOOST_TEST(isHighPass(frequency) == true);
    BOOST_TEST(isBandPass(frequency) == false);
  }

  // bandpass
  {
    Frequency frequency(HighPass(120.f), LowPass(3000.f));
    BOOST_TEST(isLowPass(frequency) == false);
    BOOST_TEST(isHighPass(frequency) == false);
    BOOST_TEST(isBandPass(frequency) == true);
  }
}
