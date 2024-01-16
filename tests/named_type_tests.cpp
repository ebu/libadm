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

class MoveOnly {
 public:
  MoveOnly() : value(0) {}
  MoveOnly(int value) : value(value) {}

  MoveOnly(MoveOnly const &) = delete;
  MoveOnly &operator=(MoveOnly const &) = delete;

  MoveOnly(MoveOnly &&) = default;
  MoveOnly &operator=(MoveOnly &&) = default;

  int get() const { return value; }

 private:
  int value;
};

struct MoveOnlyTag {};
using NamedMoveOnly = adm::detail::NamedType<MoveOnly, MoveOnlyTag>;

TEST_CASE("NamedType_move") {
  NamedMoveOnly value{MoveOnly{1}};

  MoveOnly moved = std::move(value).get();
  REQUIRE(moved.get() == 1);
  REQUIRE(value.get().get() == 0);

  MoveOnly moved_from_temporary = std::move(NamedMoveOnly{MoveOnly{2}}).get();
  REQUIRE(moved_from_temporary.get() == 2);

  value = MoveOnly{3};
  NamedMoveOnly moved_from_named(std::move(value));
  REQUIRE(moved_from_named.get().get() == 3);
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
