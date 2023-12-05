#include <catch2/catch.hpp>
#include "adm/common_definitions.hpp"
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "adm/errors.hpp"
#include "helper/file_comparator.hpp"

TEST_CASE("write_simple_object") {
  using namespace adm;

  auto document = Document::create();
  addSimpleObjectTo(document, "My Simple Object");
  reassignIds(document);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_simple_object"));
}

TEST_CASE("write_simple_object_short_structure") {
  using namespace adm;

  auto document = Document::create();
  addSimpleObjectShortStructureTo(document,
                                  "My Simple Short Structured Object");
  reassignIds(document);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_simple_object_short_structure"));
}

TEST_CASE("write_simple_common_definitions_object") {
  using namespace adm;

  auto document = Document::create();
  REQUIRE_THROWS_AS(
      addSimpleCommonDefinitionsObjectTo(document, "My 5.1 Object", "0+5+0"),
      error::AdmException);
  addCommonDefinitionsTo(document);
  REQUIRE_THROWS_AS(
      addSimpleCommonDefinitionsObjectTo(
          document, "My Unsupported Speaker Layout", "unsupported"),
      error::AdmException);
  addSimpleCommonDefinitionsObjectTo(document, "My 5.1 Object", "0+5+0");
  reassignIds(document);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(),
             EqualsXmlFile("write_simple_common_definitions_object"));
}

TEST_CASE("write_mixed_objects_and_structures") {
  using namespace adm;

  auto document = Document::create();
  addCommonDefinitionsTo(document);
  addSimpleObjectTo(document, "Simple Object 1");
  addSimpleObjectShortStructureTo(document,
                                  "Simple Object 2 (short structured)");
  addSimpleCommonDefinitionsObjectTo(document, "5.1 Object 3", "0+5+0");
  addSimpleObjectTo(document, "Simple Object 4");
  addSimpleObjectShortStructureTo(document,
                                  "Simple Object 5 (short structured)");
  reassignIds(document);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_mixed_objects_and_structures"));
}
