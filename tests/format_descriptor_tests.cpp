#include <catch2/catch.hpp>
#include "adm/elements/format_descriptor.hpp"

TEST_CASE("format_descriptor") {
  using namespace adm;

  // parseFormatLabel
  {
    REQUIRE(parseFormatLabel("0000") == FormatDefinition::UNDEFINED);
    REQUIRE(parseFormatLabel("0001") == FormatDefinition::PCM);
  }
  // parseFormatDefinition
  {
    REQUIRE(parseFormatDefinition("Undefined") == FormatDefinition::UNDEFINED);
    REQUIRE(parseFormatDefinition("PCM") == FormatDefinition::PCM);
  }
  // formatFormatDefinition
  {
    REQUIRE(formatFormatDefinition(FormatDefinition::UNDEFINED) == "Undefined");
    REQUIRE(formatFormatDefinition(FormatDefinition::PCM) == "PCM");
  }
  // formatFormatDefinition
  {
    REQUIRE(formatFormatLabel(FormatDefinition::UNDEFINED) == "0000");
    REQUIRE(formatFormatLabel(FormatDefinition::PCM) == "0001");
  }
}
