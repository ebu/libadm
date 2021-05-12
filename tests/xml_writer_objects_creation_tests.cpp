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

TEST_CASE("write_objects_with_position_offset") {
  using namespace adm;
  auto document = Document::create();

  auto cartesianOffset = CartesianPositionOffset(X(0.0), Y(0.1), Z(-.2));
  auto cartesianOffsetObject = AudioObject::create(
      AudioObjectName("CartesianOffsetObject"), cartesianOffset);
  document->add(cartesianOffsetObject);

  auto sphericalOffset = SphericalPositionOffset(Azimuth(30.0), Elevation(0.0),
                                                 DistanceOffset(-0.5));
  auto sphericalOffsetObject = AudioObject::create(
      AudioObjectName("CartesianOffsetObject"), sphericalOffset);
  document->add(sphericalOffsetObject);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_objects_with_position_offset"));
}