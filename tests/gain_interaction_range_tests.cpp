#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_object_interaction.hpp"

#define BOOST_TEST_MODULE GainInteractionRange
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(gain_interaction_range,
                     *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;

  GainInteractionRange gainRange;
  BOOST_TEST(gainRange.has<GainInteractionMin>() == false);
  BOOST_TEST(gainRange.has<GainInteractionMax>() == false);

  gainRange.set(GainInteractionMin(0.5f));
  gainRange.set(GainInteractionMax(1.5f));

  BOOST_TEST(gainRange.has<GainInteractionMin>() == true);
  BOOST_TEST(gainRange.has<GainInteractionMax>() == true);
  BOOST_TEST(gainRange.get<GainInteractionMin>() == 0.5f);
  BOOST_TEST(gainRange.get<GainInteractionMax>() == 1.5f);

  gainRange.unset<GainInteractionMin>();
  gainRange.unset<GainInteractionMax>();

  BOOST_TEST(gainRange.has<GainInteractionMin>() == false);
  BOOST_TEST(gainRange.has<GainInteractionMax>() == false);
}
