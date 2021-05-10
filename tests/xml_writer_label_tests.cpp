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

  audioContent->add(
      AudioContentLabel(LabelLanguage("en"), LabelValue("My Content")));
  audioContent->add(AudioContentLabel(LabelValue("My Content")));
  audioContent->add(AudioContentLabel(LabelLanguage("en")));
  audioContent->add(AudioContentLabel());
  auto document = Document::create();
  document->add(audioContent);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_labels"));
}