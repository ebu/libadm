#include <catch2/catch.hpp>
#include "adm/detail/named_type.hpp"

TEST_CASE("compare_named_types") {
  using namespace adm;
  // integers
  {
    using NamedInteger = detail::NamedType<int, struct NamedIntegerTag>;
    REQUIRE(NamedInteger(1) == 1);
    REQUIRE(NamedInteger(1) != 2);
    REQUIRE(NamedInteger(1) < 2);
    REQUIRE(NamedInteger(2) > 1);
    REQUIRE(NamedInteger(1) <= 2);
    REQUIRE(NamedInteger(2) >= 1);

    REQUIRE(NamedInteger(1) == NamedInteger(1));
    REQUIRE(NamedInteger(1) != NamedInteger(2));
    REQUIRE(NamedInteger(1) < NamedInteger(2));
    REQUIRE(NamedInteger(2) > NamedInteger(1));
    REQUIRE(NamedInteger(1) <= NamedInteger(2));
    REQUIRE(NamedInteger(2) >= NamedInteger(1));
  }
}

TEST_CASE("compare_checked_named_types") {
  using namespace adm;
  // integers
  {
    using NamedIntegerRange =
        detail::NamedType<int, struct NamedIntegerRangeTag,
                          detail::RangeValidator<0, 10> >;
    REQUIRE(NamedIntegerRange(1) == 1);
    REQUIRE(NamedIntegerRange(1) != 2);
    REQUIRE(NamedIntegerRange(1) < 2);
    REQUIRE(NamedIntegerRange(2) > 1);
    REQUIRE(NamedIntegerRange(1) <= 2);
    REQUIRE(NamedIntegerRange(2) >= 1);

    REQUIRE(NamedIntegerRange(1) == NamedIntegerRange(1));
    REQUIRE(NamedIntegerRange(1) != NamedIntegerRange(2));
    REQUIRE(NamedIntegerRange(1) < NamedIntegerRange(2));
    REQUIRE(NamedIntegerRange(2) > NamedIntegerRange(1));
    REQUIRE(NamedIntegerRange(1) <= NamedIntegerRange(2));
    REQUIRE(NamedIntegerRange(2) >= NamedIntegerRange(1));
  }
}

TEST_CASE("NamedType_range_check") {
  using namespace adm;
  using NamedIntegerRange = detail::NamedType<int, struct NamedIntegerRangeTag,
                                              detail::RangeValidator<0, 10> >;
  NamedIntegerRange goodValue(5);
  REQUIRE_THROWS_AS(NamedIntegerRange(12), OutOfRangeError);
  REQUIRE_THROWS_AS(NamedIntegerRange(-1), OutOfRangeError);

  NamedIntegerRange lower_limit(0);
  REQUIRE_THROWS_AS(lower_limit--, OutOfRangeError);
  REQUIRE_THROWS_AS(--lower_limit, OutOfRangeError);
  REQUIRE(lower_limit == 0);

  NamedIntegerRange upper_limit(10);
  REQUIRE_THROWS_AS(upper_limit++, OutOfRangeError);
  REQUIRE_THROWS_AS(++upper_limit, OutOfRangeError);
  REQUIRE(upper_limit == 10);
}

TEST_CASE("screen_edge_validator") {
  using namespace adm;
  struct ScreenEdgeLockTag {};
  using ScreenEdgeLock = detail::NamedType<std::string, ScreenEdgeLockTag,
                                           detail::ScreenEdgeValidator>;
  ScreenEdgeLock goodValue_1("left");
  ScreenEdgeLock goodValue_2("right");
  ScreenEdgeLock goodValue_3("top");
  ScreenEdgeLock goodValue_4("bottom");
  REQUIRE_THROWS_AS(ScreenEdgeLock("center"), InvalidStringError);
}
