#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/detail/named_type.hpp"

#define BOOST_TEST_MODULE detail::NamedType
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(compare_named_types) {
  using namespace adm;
  // integers
  {
    using NamedInteger = detail::NamedType<int, struct NamedIntegerTag>;
    BOOST_TEST(NamedInteger(1) == 1);
    BOOST_TEST(NamedInteger(1) != 2);
    BOOST_TEST(NamedInteger(1) < 2);
    BOOST_TEST(NamedInteger(2) > 1);
    BOOST_TEST(NamedInteger(1) <= 2);
    BOOST_TEST(NamedInteger(2) >= 1);

    BOOST_TEST(NamedInteger(1) == NamedInteger(1));
    BOOST_TEST(NamedInteger(1) != NamedInteger(2));
    BOOST_TEST(NamedInteger(1) < NamedInteger(2));
    BOOST_TEST(NamedInteger(2) > NamedInteger(1));
    BOOST_TEST(NamedInteger(1) <= NamedInteger(2));
    BOOST_TEST(NamedInteger(2) >= NamedInteger(1));
  }
}

BOOST_AUTO_TEST_CASE(compare_checked_named_types) {
  using namespace adm;
  // integers
  {
    using NamedIntegerRange =
        detail::NamedType<int, struct NamedIntegerRangeTag,
                          detail::RangeValidator<0, 10> >;
    BOOST_TEST(NamedIntegerRange(1) == 1);
    BOOST_TEST(NamedIntegerRange(1) != 2);
    BOOST_TEST(NamedIntegerRange(1) < 2);
    BOOST_TEST(NamedIntegerRange(2) > 1);
    BOOST_TEST(NamedIntegerRange(1) <= 2);
    BOOST_TEST(NamedIntegerRange(2) >= 1);

    BOOST_TEST(NamedIntegerRange(1) == NamedIntegerRange(1));
    BOOST_TEST(NamedIntegerRange(1) != NamedIntegerRange(2));
    BOOST_TEST(NamedIntegerRange(1) < NamedIntegerRange(2));
    BOOST_TEST(NamedIntegerRange(2) > NamedIntegerRange(1));
    BOOST_TEST(NamedIntegerRange(1) <= NamedIntegerRange(2));
    BOOST_TEST(NamedIntegerRange(2) >= NamedIntegerRange(1));
  }
}

BOOST_AUTO_TEST_CASE(NamedType_range_check) {
  using namespace adm;
  using NamedIntegerRange = detail::NamedType<int, struct NamedIntegerRangeTag,
                                              detail::RangeValidator<0, 10> >;
  NamedIntegerRange goodValue(5);
  BOOST_CHECK_THROW(NamedIntegerRange(12), OutOfRangeError);
  BOOST_CHECK_THROW(NamedIntegerRange(-1), OutOfRangeError);
}

BOOST_AUTO_TEST_CASE(screen_edge_validator) {
  using namespace adm;
  struct ScreenEdgeLockTag {};
  using ScreenEdgeLock = detail::NamedType<std::string, ScreenEdgeLockTag,
                                           detail::ScreenEdgeValidator>;
  ScreenEdgeLock goodValue_1("left");
  ScreenEdgeLock goodValue_2("right");
  ScreenEdgeLock goodValue_3("top");
  ScreenEdgeLock goodValue_4("bottom");
  BOOST_CHECK_THROW(ScreenEdgeLock("center"), InvalidStringError);
}
