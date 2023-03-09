#include <catch2/catch.hpp>
#include "adm/elements/profile_list.hpp"
#include "helper/parameter_checks.hpp"
#include "adm/document.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;
using namespace adm_test;

TEST_CASE("version") {
  auto document = Document::create();

  check_optional_param<Version>(document,
                                canBeSetTo(Version{"ITU-R_BS.2076-2"}));
}

TEST_CASE("xml/version") {
  auto document = parseXml("xml_parser/version.xml");

  REQUIRE(document->has<Version>());
  REQUIRE(document->get<Version>().get() == "ITU-R_BS.2076-2");

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("version"));
}
