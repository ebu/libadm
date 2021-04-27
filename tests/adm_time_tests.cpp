#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/time.hpp"

using namespace adm;

namespace std {
  std::ostream& operator<<(std::ostream& stream, const FractionalTime& time) {
    return stream << "FractionalTime{" << time.numerator << ", "
                  << time.denominator << "}";
  }

  // required because catch2 stringify doesn't know about boost::variants
  std::ostream& operator<<(std::ostream& stream,
                           const std::chrono::nanoseconds& time) {
    return stream << "nanoseconds{" << time.count() << "}";
  }

  std::ostream& operator<<(std::ostream& stream, const Time& time) {
    return boost::apply_visitor(
        [&](const auto& t) -> std::ostream& { return stream << t; },
        time.asVariant());
  }
}  // namespace std

TEST_CASE("adm_time") {
  // different accuracy
  {
    parseFractionalTimecode("00:00:00.00000");
    parseFractionalTimecode("00:00:00.000000");
    parseFractionalTimecode("00:00:00.0000000");
    parseFractionalTimecode("00:00:00.00000000");
    parseFractionalTimecode("00:00:00.000000000");
  }
  // check hours, minutes, seconds, nanoseconds
  {
    REQUIRE(parseFractionalTimecode("01:00:00.00000") ==
            Time(std::chrono::nanoseconds(3600000000000)));
    REQUIRE(parseFractionalTimecode("00:01:00.00000") ==
            Time(std::chrono::nanoseconds(60000000000)));
    REQUIRE(parseFractionalTimecode("00:00:01.00000") ==
            Time(std::chrono::nanoseconds(1000000000)));
    REQUIRE(parseFractionalTimecode("00:00:00.10000") ==
            Time(std::chrono::nanoseconds(100000000)));
    REQUIRE(parseFractionalTimecode("00:00:00.000000001") ==
            Time(std::chrono::nanoseconds(1)));
  }

  // check fractional
  {
    REQUIRE(parseFractionalTimecode("01:00:00.0S1") ==
            Time(FractionalTime{3600, 1}));
    REQUIRE(parseFractionalTimecode("00:01:00.0S1") ==
            Time(FractionalTime{60, 1}));
    REQUIRE(parseFractionalTimecode("00:00:01.0S1") ==
            Time(FractionalTime{1, 1}));
    REQUIRE(parseFractionalTimecode("00:00:00.1S10") ==
            Time(FractionalTime{1, 10}));

    // test leading zeros
    REQUIRE(parseFractionalTimecode("00:00:00.01S010") ==
            Time(FractionalTime{1, 10}));
  }

  // reformat timecode
  {
    REQUIRE(formatFractionalTimecode(parseFractionalTimecode(
                "00:00:00.00000")) == "00:00:00.000000000");
    REQUIRE(formatFractionalTimecode(parseFractionalTimecode(
                "04:20:14.046079001")) == "04:20:14.046079001");
    REQUIRE(formatFractionalTimecode(parseFractionalTimecode(
                "23:59:59.999999999")) == "23:59:59.999999999");

    REQUIRE(formatFractionalTimecode(
                parseFractionalTimecode("00:00:00.000S001")) == "00:00:00.0S1");
    REQUIRE(formatFractionalTimecode(parseFractionalTimecode(
                "04:20:14.123S456")) == "04:20:14.123S456");
  }
}

TEST_CASE("Time Fractional") {
  Time t{FractionalTime{6, 4}};

  REQUIRE(t == FractionalTime{6, 4});
  REQUIRE(t.asFractional() == FractionalTime{6, 4});
  REQUIRE(t.asNanoseconds() == std::chrono::nanoseconds{1500000000});
  REQUIRE(t.isFractional());
  REQUIRE(!t.isNanoseconds());

  REQUIRE(t.asFractional().normalised() == FractionalTime{3, 2});
}

TEST_CASE("Time nanoseconds") {
  Time t{std::chrono::nanoseconds{1500000000}};

  REQUIRE(t == std::chrono::nanoseconds{1500000000});
  REQUIRE(t.asNanoseconds() == std::chrono::nanoseconds{1500000000});
  REQUIRE(t.asFractional() == FractionalTime{1500000000, 1000000000});
  REQUIRE(t.isNanoseconds());
  REQUIRE(!t.isFractional());
}
