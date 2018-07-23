#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/jump_position.hpp"

#define BOOST_TEST_MODULE JumpPosition
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(jump_position, *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    JumpPosition jumpPosition;

    BOOST_TEST(jumpPosition.has<JumpPositionFlag>() == true);
    BOOST_TEST(jumpPosition.has<InterpolationLength>() == true);

    BOOST_TEST(jumpPosition.isDefault<JumpPositionFlag>() == true);
    BOOST_TEST(jumpPosition.isDefault<InterpolationLength>() == true);

    BOOST_TEST(jumpPosition.get<JumpPositionFlag>() == false);
    BOOST_CHECK(jumpPosition.get<InterpolationLength>().get() ==
                std::chrono::seconds(0));

    jumpPosition.set(JumpPositionFlag(true));
    jumpPosition.set(InterpolationLength(std::chrono::milliseconds(1500)));

    BOOST_TEST(jumpPosition.get<JumpPositionFlag>() == true);
    BOOST_CHECK(jumpPosition.get<InterpolationLength>().get() ==
                std::chrono::milliseconds(1500));

    BOOST_TEST(jumpPosition.isDefault<JumpPositionFlag>() == false);
    BOOST_TEST(jumpPosition.isDefault<InterpolationLength>() == false);

    jumpPosition.unset<JumpPositionFlag>();
    jumpPosition.unset<InterpolationLength>();

    BOOST_TEST(jumpPosition.isDefault<JumpPositionFlag>() == true);
    BOOST_TEST(jumpPosition.isDefault<InterpolationLength>() == true);

    BOOST_TEST(isEnabled(jumpPosition) == false);
    enable(jumpPosition);
    BOOST_TEST(isEnabled(jumpPosition) == true);
    disable(jumpPosition);
    BOOST_TEST(isEnabled(jumpPosition) == false);
  }
}
