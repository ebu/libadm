#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

TEST_CASE("write_audio_object_interaction") {
  using namespace adm;

  auto audioContent = AudioContent::create(AudioContentName("MyContent"));
  audioContent->set(AudioContentId(AudioContentIdValue(0x1001)));
  audioContent->set(AudioContentLanguage("de"));
  audioContent->set(DialogueContent::AUDIO_DESCRIPTION);
  audioContent->add(Label(LabelLanguage("en"), LabelValue("My Content")));
  audioContent->add(Label(LabelLanguage("deu"), LabelValue("Mein Inhalt")));

  auto document = Document::create();
  document->add(audioContent);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_audio_content"));
}
