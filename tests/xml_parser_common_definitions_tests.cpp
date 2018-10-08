#include <catch2/catch.hpp>
#include <sstream>
#include "adm/parse.hpp"

TEST_CASE("xml_parser/with_common_definitions") {
  adm::parseXml("xml_parser/with_common_definitions.xml");
}
