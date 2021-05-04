#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/time.hpp"
#include "adm/utilities/time_conversion.hpp"

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
    parseTimecode("00:00:00.00000");
    parseTimecode("00:00:00.000000");
    parseTimecode("00:00:00.0000000");
    parseTimecode("00:00:00.00000000");
    parseTimecode("00:00:00.000000000");
  }
  // check hours, minutes, seconds, nanoseconds
  {
    REQUIRE(parseTimecode("01:00:00.00000") ==
            Time(std::chrono::nanoseconds(3600000000000)));
    REQUIRE(parseTimecode("00:01:00.00000") ==
            Time(std::chrono::nanoseconds(60000000000)));
    REQUIRE(parseTimecode("00:00:01.00000") ==
            Time(std::chrono::nanoseconds(1000000000)));
    REQUIRE(parseTimecode("00:00:00.10000") ==
            Time(std::chrono::nanoseconds(100000000)));
    REQUIRE(parseTimecode("00:00:00.000000001") ==
            Time(std::chrono::nanoseconds(1)));
  }

  // check fractional
  {
    REQUIRE(parseTimecode("01:00:00.0S1") == Time(FractionalTime{3600, 1}));
    REQUIRE(parseTimecode("00:01:00.0S1") == Time(FractionalTime{60, 1}));
    REQUIRE(parseTimecode("00:00:01.0S1") == Time(FractionalTime{1, 1}));
    REQUIRE(parseTimecode("00:00:00.1S10") == Time(FractionalTime{1, 10}));

    // test leading zeros
    REQUIRE(parseTimecode("00:00:00.01S010") == Time(FractionalTime{1, 10}));
  }

  // reformat timecode
  {
    REQUIRE(formatTimecode(parseTimecode("00:00:00.00000")) ==
            "00:00:00.000000000");
    REQUIRE(formatTimecode(parseTimecode("04:20:14.046079001")) ==
            "04:20:14.046079001");
    REQUIRE(formatTimecode(parseTimecode("23:59:59.999999999")) ==
            "23:59:59.999999999");

    REQUIRE(formatTimecode(parseTimecode("00:00:00.000S001")) ==
            "00:00:00.0S1");
    REQUIRE(formatTimecode(parseTimecode("04:20:14.123S456")) ==
            "04:20:14.123S456");
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

TEST_CASE("construction") {
  REQUIRE(Time{std::chrono::seconds(1)} ==
          std::chrono::nanoseconds{1000000000});
}

TEST_CASE("Exceptions") {
  REQUIRE_THROWS_WITH(parseTimecode("foo"),
                      Catch::Contains("invalid timecode"));
  REQUIRE_THROWS_WITH(parseTimecode("00:00:00.000S001YYY"),
                      Catch::Contains("invalid timecode"));
  REQUIRE_THROWS_WITH(parseTimecode("YYY00:00:00.000S001"),
                      Catch::Contains("invalid timecode"));
}

TEST_CASE("rational conversion") {
  REQUIRE(asRational(FractionalTime{4, 8}) == RationalTime{1, 2});

  REQUIRE(asRational(std::chrono::nanoseconds{1000}) ==
          RationalTime{1, 1000000});

  REQUIRE(asFractionalTime(RationalTime{1, 2}) == FractionalTime{1, 2});

  REQUIRE(asTime(RationalTime{1, 2}) == FractionalTime{1, 2});
}
