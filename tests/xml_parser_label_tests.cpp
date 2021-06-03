#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_programme_label") {
  using namespace adm;
  auto document = parseXml("xml_parser/labels.xml");
  auto audioProgramme = document->lookup(parseAudioProgrammeId("APR_1001"));

  auto labels = audioProgramme->get<Labels>();
  REQUIRE(labels.size() == 6);

  REQUIRE(labels[0].has<LabelLanguage>() == true);
  REQUIRE(labels[0].get<LabelLanguage>() == "en");
  REQUIRE(labels[0].has<LabelValue>() == true);
  REQUIRE(labels[0].get<LabelValue>() == "My Programme");

  REQUIRE(labels[1].has<LabelLanguage>() == true);
  REQUIRE(labels[1].get<LabelLanguage>() == "deu");
  REQUIRE(labels[1].has<LabelValue>() == true);
  REQUIRE(labels[1].get<LabelValue>() == "Mein Programm");

  REQUIRE(labels[2].has<LabelLanguage>() == true);
  REQUIRE(labels[2].get<LabelLanguage>() == "fr");
  REQUIRE(labels[2].has<LabelValue>() == true);
  REQUIRE(labels[2].get<LabelValue>() == "");

  REQUIRE(labels[3].has<LabelLanguage>() == true);
  REQUIRE(labels[3].get<LabelLanguage>() == "es");
  REQUIRE(labels[3].has<LabelValue>() == true);
  REQUIRE(labels[3].get<LabelValue>() == "");

  REQUIRE(labels[4].has<LabelLanguage>() == false);
  REQUIRE(labels[4].has<LabelValue>() == true);
  REQUIRE(labels[4].get<LabelValue>() == "Undefined Language");

  REQUIRE(labels[5].has<LabelLanguage>() == false);
  REQUIRE(labels[5].has<LabelValue>() == true);
  REQUIRE(labels[5].get<LabelValue>() == "");
}

TEST_CASE("xml_parser/audio_content_label") {
  using namespace adm;
  auto document = parseXml("xml_parser/labels.xml");
  auto audioContent = document->lookup(parseAudioContentId("ACO_1001"));

  auto labels = audioContent->get<Labels>();
  REQUIRE(labels.size() == 6);

  REQUIRE(labels[0].has<LabelLanguage>() == true);
  REQUIRE(labels[0].get<LabelLanguage>() == "en");
  REQUIRE(labels[0].has<LabelValue>() == true);
  REQUIRE(labels[0].get<LabelValue>() == "My Content");

  REQUIRE(labels[1].has<LabelLanguage>() == true);
  REQUIRE(labels[1].get<LabelLanguage>() == "deu");
  REQUIRE(labels[1].has<LabelValue>() == true);
  REQUIRE(labels[1].get<LabelValue>() == "Mein Inhalt");

  REQUIRE(labels[2].has<LabelLanguage>() == true);
  REQUIRE(labels[2].get<LabelLanguage>() == "fr");
  REQUIRE(labels[2].has<LabelValue>() == true);
  REQUIRE(labels[2].get<LabelValue>() == "");

  REQUIRE(labels[3].has<LabelLanguage>() == true);
  REQUIRE(labels[3].get<LabelLanguage>() == "es");
  REQUIRE(labels[3].has<LabelValue>() == true);
  REQUIRE(labels[3].get<LabelValue>() == "");

  REQUIRE(labels[4].has<LabelLanguage>() == false);
  REQUIRE(labels[4].has<LabelValue>() == true);
  REQUIRE(labels[4].get<LabelValue>() == "Undefined Language");

  REQUIRE(labels[5].has<LabelLanguage>() == false);
  REQUIRE(labels[5].has<LabelValue>() == true);
  REQUIRE(labels[5].get<LabelValue>() == "");
}

TEST_CASE("xml_parser/audio_object_label") {
  using namespace adm;
  auto document = parseXml("xml_parser/labels.xml");
  auto audioObject = document->lookup(parseAudioObjectId("AO_1001"));

  auto labels = audioObject->get<Labels>();
  REQUIRE(labels.size() == 6);

  REQUIRE(labels[0].has<LabelLanguage>() == true);
  REQUIRE(labels[0].get<LabelLanguage>() == "en");
  REQUIRE(labels[0].has<LabelValue>() == true);
  REQUIRE(labels[0].get<LabelValue>() == "My Object");

  REQUIRE(labels[1].has<LabelLanguage>() == true);
  REQUIRE(labels[1].get<LabelLanguage>() == "deu");
  REQUIRE(labels[1].has<LabelValue>() == true);
  REQUIRE(labels[1].get<LabelValue>() == "Mein Objekt");

  REQUIRE(labels[2].has<LabelLanguage>() == true);
  REQUIRE(labels[2].get<LabelLanguage>() == "fr");
  REQUIRE(labels[2].has<LabelValue>() == true);
  REQUIRE(labels[2].get<LabelValue>() == "");

  REQUIRE(labels[3].has<LabelLanguage>() == true);
  REQUIRE(labels[3].get<LabelLanguage>() == "es");
  REQUIRE(labels[3].has<LabelValue>() == true);
  REQUIRE(labels[3].get<LabelValue>() == "");

  REQUIRE(labels[4].has<LabelLanguage>() == false);
  REQUIRE(labels[4].has<LabelValue>() == true);
  REQUIRE(labels[4].get<LabelValue>() == "Undefined Language");

  REQUIRE(labels[5].has<LabelLanguage>() == false);
  REQUIRE(labels[5].has<LabelValue>() == true);
  REQUIRE(labels[5].get<LabelValue>() == "");
}

TEST_CASE("xml_parser/audio_complementary_object_group_label") {
  using namespace adm;
  auto document = parseXml("xml_parser/labels.xml");
  auto audioObject = document->lookup(parseAudioObjectId("AO_1001"));

  auto labels = audioObject->get<AudioComplementaryObjectGroupLabels>();
  REQUIRE(labels.size() == 6);

  REQUIRE(labels[0].get().has<LabelLanguage>() == true);
  REQUIRE(labels[0].get().get<LabelLanguage>() == "en");
  REQUIRE(labels[0].get().has<LabelValue>() == true);
  REQUIRE(labels[0].get().get<LabelValue>() == "My Object");

  REQUIRE(labels[1].get().has<LabelLanguage>() == true);
  REQUIRE(labels[1].get().get<LabelLanguage>() == "deu");
  REQUIRE(labels[1].get().has<LabelValue>() == true);
  REQUIRE(labels[1].get().get<LabelValue>() == "Mein Objekt");

  REQUIRE(labels[2].get().has<LabelLanguage>() == true);
  REQUIRE(labels[2].get().get<LabelLanguage>() == "fr");
  REQUIRE(labels[2].get().has<LabelValue>() == true);
  REQUIRE(labels[2].get().get<LabelValue>() == "");

  REQUIRE(labels[3].get().has<LabelLanguage>() == true);
  REQUIRE(labels[3].get().get<LabelLanguage>() == "es");
  REQUIRE(labels[3].get().has<LabelValue>() == true);
  REQUIRE(labels[3].get().get<LabelValue>() == "");

  REQUIRE(labels[4].get().has<LabelLanguage>() == false);
  REQUIRE(labels[4].get().has<LabelValue>() == true);
  REQUIRE(labels[4].get().get<LabelValue>() == "Undefined Language");

  REQUIRE(labels[5].get().has<LabelLanguage>() == false);
  REQUIRE(labels[5].get().has<LabelValue>() == true);
  REQUIRE(labels[5].get().get<LabelValue>() == "");
}
