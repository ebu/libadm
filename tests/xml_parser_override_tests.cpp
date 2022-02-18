#include <catch2/catch.hpp>
#include "adm/private/xml_parser.hpp"
#include "adm/private/xml_parser_helper.hpp"

using namespace adm;
using namespace adm::xml;

// example of overriding the block format parser to fix broken xml
class XmlParserDiffuseFix : public XmlParser {
 public:
  using XmlParser::XmlParser;

 protected:
  AudioBlockFormatObjects parseAudioBlockFormatObjects(NodePtr node) override {
    NodePtr diffuse = xml::detail::findElement(node, "diffuse");
    if (diffuse != nullptr) {
      if (std::string(diffuse->value()) == "true")
        diffuse->value("1.0");
      else if (std::string(diffuse->value()) == "false")
        diffuse->value("0.0");
    }

    return XmlParser::parseAudioBlockFormatObjects(node);
  }
};

TEST_CASE("test override") {
  std::string xml = R"(
    <audioFormatExtended>
      <audioChannelFormat audioChannelFormatID="AC_00031001"
                          audioChannelFormatName="MyChannelFormat"
                          typeLabel="0003"
                          typeDefinition="Objects">
        <audioBlockFormat audioBlockFormatID="AB_00031001_00000001">
          <position coordinate="azimuth">30.0</position>
          <position coordinate="elevation">0.0</position>
          <position coordinate="distance">1.0</position>
          <diffuse>true</difuse>
        </audioBlockFormat>
      </audioChannelFormat>
    </audioFormatExtended>
  )";

  std::shared_ptr<Document> doc = Document::create();
  XmlParserDiffuseFix parser(doc, ParserOptions::recursive_node_search);
  parser.parseString(xml);

  auto acf = doc->lookup(parseAudioChannelFormatId("AC_00031001"));
  auto abf = acf->getElements<AudioBlockFormatObjects>()[0];
  CHECK(abf.get<Diffuse>().get() == 1.0f);
}
