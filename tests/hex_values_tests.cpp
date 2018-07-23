#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/detail/hex_values.hpp"

#define BOOST_TEST_MODULE HexValues
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(hex_values) {
  using namespace adm;
  BOOST_TEST(detail::parseHexValue("0000") == 0);
  BOOST_TEST(detail::parseHexValue("FFFF") == 65535);
  BOOST_TEST(detail::parseHexValue("0a0F") == 2575);
  BOOST_TEST(detail::parseHexValue("00ff") == 255);
  BOOST_TEST(detail::parseHexValue("ff") == 255);
  BOOST_CHECK_THROW(detail::parseHexValue("ffff0"), std::runtime_error);
  BOOST_TEST(detail::formatHexValue(255) == "00ff");
  BOOST_TEST(detail::formatHexValue(255, 2) == "ff");
  BOOST_TEST(detail::formatHexValue(65535, 8) == "0000ffff");
  BOOST_CHECK_THROW(detail::formatHexValue(1000000), std::runtime_error);
}
