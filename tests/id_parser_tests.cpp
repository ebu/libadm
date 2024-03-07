#include <catch2/catch.hpp>
#include "adm/detail/id_parser.hpp"

using namespace adm::detail;

namespace adm {
  struct ExampleId {};
  namespace detail {
    template <>
    struct IdTraits<ExampleId> {
      static constexpr char const* name{"ExampleId"};
      static constexpr char const* prefix{"AI_"};
      static constexpr char const* format{"AI_xxxx_yyyy"};
      static std::size_t const underscore_position{7};
    };
  }  // namespace detail
}  // namespace adm

void parseExample(const std::string& id) {
  // AI_xxxx_yyyy
  IDParser parser{id};
  parser.check_size<adm::ExampleId>();
  parser.check_prefix<adm::ExampleId>();
  auto type = parser.parse_hex<adm::ExampleId>(3, 4);
  parser.check_underscore<adm::ExampleId>();
  auto value = parser.parse_hex<adm::ExampleId>(8, 4);

  REQUIRE(type == 0x12ab);
  REQUIRE(value == 0x89ef);
}

TEST_CASE("id_parse") {
  SECTION("lower") { parseExample("AI_12ab_89ef"); };
  SECTION("upper") { parseExample("AI_12AB_89EF"); }
  SECTION("too long") {
    REQUIRE_THROWS_AS(parseExample("AI_12ab_89ef "), std::runtime_error);
  }
  SECTION("bad char lower high") {
    REQUIRE_THROWS_AS(parseExample("AI_12ab_89eg"), std::runtime_error);
  }
  SECTION("bad char upper high") {
    REQUIRE_THROWS_AS(parseExample("AI_12ab_89eG"), std::runtime_error);
  }
  SECTION("bad char lower low") {
    REQUIRE_THROWS_AS(parseExample("AI_12ab_89e`"), std::runtime_error);
  }
  SECTION("bad char upper low") {
    REQUIRE_THROWS_AS(parseExample("AI_12ab_89e@"), std::runtime_error);
  }
  SECTION("underscore") {
    REQUIRE_THROWS_AS(parseExample("AI_12ab 89ef"), std::runtime_error);
  }
  SECTION("prefix end") {
    REQUIRE_THROWS_AS(parseExample("AI-12ab_89ef"), std::runtime_error);
  }
  SECTION("prefix start") {
    REQUIRE_THROWS_AS(parseExample("-I_12ab_89ef"), std::runtime_error);
  }
}

TEST_CASE("formatHex") {
  SECTION("padding") {
    std::string s("xxxxxx");
    formatHex(s, 1, 4, 0x89ef);
    REQUIRE(s == "x89efx");
  };

  SECTION("ends") {
    std::string s("xxxx");
    formatHex(s, 0, 4, 0x89ef);
    REQUIRE(s == "89ef");
  };

  SECTION("too big") {
    std::string s("xxxx");
    REQUIRE_THROWS_AS(formatHex(s, 0, 4, 0x10000), std::runtime_error);
  };
}
