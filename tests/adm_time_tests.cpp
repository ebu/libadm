#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/time.hpp"

TEST_CASE("adm_time") {
  using namespace adm;
  // different accuracy
  {
    parseTimecode("00:00:00.00000");
    parseTimecode("00:00:00.000000");
    parseTimecode("00:00:00.0000000");
    parseTimecode("00:00:00.00000000");
    parseTimecode("00:00:00.000000000");
  }
  // check hours, minutes, seconds, nanoseconds
  {
    REQUIRE(parseTimecode("01:00:00.00000") ==
            std::chrono::nanoseconds(3600000000000));
    REQUIRE(parseTimecode("00:01:00.00000") ==
            std::chrono::nanoseconds(60000000000));
    REQUIRE(parseTimecode("00:00:01.00000") ==
            std::chrono::nanoseconds(1000000000));
    REQUIRE(parseTimecode("00:00:00.10000") ==
            std::chrono::nanoseconds(100000000));
    REQUIRE(parseTimecode("00:00:00.000000001") == std::chrono::nanoseconds(1));
  }
  // reformat timecode
  {
    REQUIRE(formatTimecode(parseTimecode("00:00:00.00000")) ==
            "00:00:00.000000000");
    REQUIRE(formatTimecode(parseTimecode("04:20:14.046079001")) ==
            "04:20:14.046079001");
    REQUIRE(formatTimecode(parseTimecode("23:59:59.999999999")) ==
            "23:59:59.999999999");
  }
}
