#include <catch2/catch.hpp>
#include "adm/private/rapidxml_utils.hpp"
#include "adm/private/rapidxml_wrapper.hpp"

TEST_CASE("line_number_calculation") {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>\n"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00031002\" "
        "audioChannelFormatName=\"MyChannelFormat\" "
        "typeLabel=\"0003\""
        ">\n"
        "<frequency \n typeDefinition=\"lowPass\">120</frequency>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");

    rapidxml::file<> xmlFile(admStream);
    rapidxml::xml_document<> xmlDocument;
    xmlDocument.parse<0>(xmlFile.data());
    auto root = xmlDocument.first_node();
    auto freq = root->first_node()->first_node();
    auto p = adm::xml::getDocumentLine(freq);
    REQUIRE(p == 2);
    p = adm::xml::getDocumentLine(freq->first_attribute());
    REQUIRE(p == 3);
  }
}
