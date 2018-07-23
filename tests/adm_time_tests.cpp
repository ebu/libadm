#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/time.hpp"

#define BOOST_TEST_MODULE AdmTime
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(adm_time) {
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
    BOOST_CHECK(parseTimecode("01:00:00.00000") ==
                std::chrono::nanoseconds(3600000000000));
    BOOST_CHECK(parseTimecode("00:01:00.00000") ==
                std::chrono::nanoseconds(60000000000));
    BOOST_CHECK(parseTimecode("00:00:01.00000") ==
                std::chrono::nanoseconds(1000000000));
    BOOST_CHECK(parseTimecode("00:00:00.10000") ==
                std::chrono::nanoseconds(100000000));
    BOOST_CHECK(parseTimecode("00:00:00.000000001") ==
                std::chrono::nanoseconds(1));
  }
  // reformat timecode
  {
    BOOST_CHECK(formatTimecode(parseTimecode("00:00:00.00000")) ==
                "00:00:00.000000000");
    BOOST_CHECK(formatTimecode(parseTimecode("04:20:14.046079001")) ==
                "04:20:14.046079001");
    BOOST_CHECK(formatTimecode(parseTimecode("23:59:59.999999999")) ==
                "23:59:59.999999999");
  }
}
