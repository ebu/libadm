#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/label.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_content") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_content.xml");
  auto audioContent = document->lookup(parseAudioContentId("ACO_1001"));

  REQUIRE(audioContent->has<AudioContentName>() == true);
  REQUIRE(audioContent->has<AudioContentId>() == true);
  REQUIRE(audioContent->has<AudioContentLanguage>() == true);
  REQUIRE(audioContent->has<LoudnessMetadata>() == true);
  REQUIRE(audioContent->has<DialogueId>() == true);

  REQUIRE(audioContent->get<AudioContentName>() == "MyContent");
  REQUIRE(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
          0x1001u);
  REQUIRE(audioContent->get<AudioContentLanguage>() == "en");
  REQUIRE(audioContent->get<DialogueId>() == Dialogue::DIALOGUE);
  REQUIRE(audioContent->get<DialogueContentKind>() ==
          DialogueContent::VOICEOVER);

  auto labels = audioContent->getElements<AudioContentLabel>();
  REQUIRE(labels.size() == 2);
  REQUIRE(labels[0].get<LabelLanguage>() == "en");
  REQUIRE(labels[0].get<LabelValue>() == "My Content");
  REQUIRE(labels[1].get<LabelLanguage>() == "deu");
  REQUIRE(labels[1].get<LabelValue>() == "Mein Inhalt");
}

TEST_CASE("xml_parser/audio_content_duplicate_id") {
  REQUIRE_THROWS_AS(adm::parseXml("xml_parser/audio_content_duplicate_id.xml"),
                    adm::error::XmlParsingDuplicateId);
}
