#include <catch2/catch.hpp>
#include <sstream>
#include "adm/parse.hpp"
#include "test_config.hpp"

TEST_CASE("xml_parser/with_common_definitions") {
  adm::parseXml(data_file("xml_parser/with_common_definitions.xml"));
}
