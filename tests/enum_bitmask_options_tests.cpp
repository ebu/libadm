#include <catch2/catch.hpp>
#include <sstream>
#include "adm/detail/enum_bitmask.hpp"

namespace adm {

  enum class TestOptions : unsigned {
    none = 0x0,
    secure = 0x1,
    relaxed = 0x2,
    verbose = 0x4
  };

}  // namespace adm

ENABLE_ENUM_BITMASK_OPERATORS(adm::TestOptions);

inline bool isSet(adm::TestOptions options, adm::TestOptions flag) {
  return static_cast<bool>(options & flag);
}

// stream operator required for boost unit test
std::ostream& operator<<(std::ostream& stream, adm::TestOptions opt) {
  stream << static_cast<std::underlying_type<adm::TestOptions>::type>(opt);
  return stream;
}

TEST_CASE("bitmask_operators") {
  using adm::TestOptions;
  {
    TestOptions opt = TestOptions::secure | TestOptions::verbose;
    REQUIRE(unsigned(opt) == (0x1 | 0x4));
  }
  {
    TestOptions opt = TestOptions::secure;
    opt |= TestOptions::verbose;
    REQUIRE(unsigned(opt) == (0x1 | 0x4));
  }
  {
    TestOptions opt =
        TestOptions::verbose & (TestOptions::secure | TestOptions::verbose);
    REQUIRE(unsigned(opt) == (0x4 & (0x1 | 0x4)));
  }
  {
    TestOptions opt = (TestOptions::secure | TestOptions::verbose);
    opt &= TestOptions::verbose;
    REQUIRE(unsigned(opt) == (0x4 & (0x1 | 0x4)));
  }
  {
    TestOptions opt = (TestOptions::secure | TestOptions::verbose);
    opt ^= TestOptions::verbose;
    REQUIRE(unsigned(opt) == (0x4 ^ (0x1 | 0x4)));
  }
  {
    TestOptions opt = ~TestOptions::secure;
    REQUIRE(unsigned(opt) == (~0x1));
  }
}

std::string options_to_string(adm::TestOptions opt = adm::TestOptions::none) {
  using adm::TestOptions;
  std::stringstream ss;

  if (TestOptions::none != (opt & TestOptions::secure)) {
    ss << "secure";
  }
  if (TestOptions::none != (opt & TestOptions::relaxed)) {
    ss << "relaxed";
  }
  if (TestOptions::none != (opt & TestOptions::verbose)) {
    ss << "verbose";
  }
  return ss.str();
}

TEST_CASE("options_as_parameter") {
  using adm::TestOptions;
  // this test case evaluates the primary use case these bitmask enum options:
  // pass a combination of enum values to a function and check which bits are
  // set
  REQUIRE(options_to_string() == "");
  REQUIRE(options_to_string(TestOptions::secure) == "secure");
  REQUIRE(options_to_string(TestOptions::relaxed) == "relaxed");
  REQUIRE(options_to_string(TestOptions::verbose) == "verbose");
  REQUIRE(options_to_string(TestOptions::relaxed | TestOptions::secure) ==
          "securerelaxed");
  REQUIRE(options_to_string(TestOptions::relaxed | TestOptions::secure |
                            TestOptions::verbose) == "securerelaxedverbose");

  REQUIRE(options_to_string((TestOptions::relaxed | TestOptions::secure |
                             TestOptions::verbose) &
                            TestOptions::secure) == "secure");

  REQUIRE(options_to_string((TestOptions::relaxed | TestOptions::verbose) &
                            TestOptions::secure) == "");
  REQUIRE(options_to_string(~(TestOptions::relaxed | TestOptions::verbose)) ==
          "secure");
  REQUIRE(options_to_string((TestOptions::relaxed | TestOptions::secure |
                             TestOptions::verbose) ^
                            TestOptions::secure) == "relaxedverbose");
}

TEST_CASE("options_manipulation") {
  using adm::TestOptions;

  TestOptions options = TestOptions::none;

  // check if a flag/option is set using bitwise AND
  REQUIRE(!isSet(options, TestOptions::secure));
  REQUIRE(!isSet(options, TestOptions::relaxed));
  REQUIRE(!isSet(options, TestOptions::verbose));

  // set a flag/option using bitwise OR
  options |= TestOptions::relaxed;
  options |= TestOptions::verbose;

  REQUIRE(!isSet(options, TestOptions::secure));
  REQUIRE(isSet(options, TestOptions::relaxed));
  REQUIRE(isSet(options, TestOptions::verbose));

  // unset a flag using bitwise NOT + AND
  options &= ~TestOptions::relaxed;

  REQUIRE(!isSet(options, TestOptions::secure));
  REQUIRE(!isSet(options, TestOptions::relaxed));
  REQUIRE(isSet(options, TestOptions::verbose));
}
