#include <catch2/catch.hpp>
#include "adm/detail/hex_values.hpp"

using namespace adm;

TEST_CASE("parseHexValue") {
  REQUIRE(detail::parseHexValue("0000") == 0);
  REQUIRE(detail::parseHexValue("FFFF") == 65535);
  REQUIRE(detail::parseHexValue("0a0F") == 2575);
  REQUIRE(detail::parseHexValue("00ff") == 255);
  REQUIRE(detail::parseHexValue("ff") == 255);
  REQUIRE_THROWS_AS(detail::parseHexValue("ffff0"), std::runtime_error);
  REQUIRE_THROWS_AS(detail::parseHexValue("fff "), std::runtime_error);
  REQUIRE(detail::parseHexValue("ffff0", 8) == 1048560);

  BENCHMARK("parse 4 digits") {
    return detail::parseHexValue("FFFF");
  };
}

TEST_CASE("formatHexValue") {
  REQUIRE(detail::formatHexValue(255) == "00ff");
  REQUIRE(detail::formatHexValue(255, 2) == "ff");
  REQUIRE(detail::formatHexValue(65535, 8) == "0000ffff");
  REQUIRE_THROWS_AS(detail::formatHexValue(1000000), std::runtime_error);
}
