#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

TEST_CASE("write_labels") {
  using namespace adm;
  auto document = Document::create();

  // audioContent
  auto audioContent = AudioContent::create(AudioContentName("MyContent"));
  audioContent->add(
      AudioContentLabel(LabelLanguage("en"), LabelValue("My Content")));
  audioContent->add(AudioContentLabel(LabelValue("My Content")));
  audioContent->add(AudioContentLabel(LabelLanguage("en")));
  audioContent->add(AudioContentLabel());
  document->add(audioContent);

  // audioProgramme
  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  audioProgramme->add(
      AudioProgrammeLabel(LabelLanguage("en"), LabelValue("My Programme")));
  audioProgramme->add(AudioProgrammeLabel(LabelValue("My Programme")));
  audioProgramme->add(AudioProgrammeLabel(LabelLanguage("en")));
  audioProgramme->add(AudioProgrammeLabel());
  document->add(audioProgramme);

  // audioObject including complementaryObjectGroup
  auto audioObject = AudioObject::create(AudioObjectName("MyObject"));
  audioObject->add(
      AudioObjectLabel(LabelLanguage("en"), LabelValue("My Object")));
  audioObject->add(AudioObjectLabel(LabelValue("My Object")));
  audioObject->add(AudioObjectLabel(LabelLanguage("en")));
  audioObject->add(AudioObjectLabel());

  audioObject->add(AudioComplementaryObjectGroupLabel(
      LabelLanguage("en"), LabelValue("My ComplementaryObjectGroup")));
  audioObject->add(AudioComplementaryObjectGroupLabel(
      LabelValue("My ComplementaryObjectGroup")));
  audioObject->add(AudioComplementaryObjectGroupLabel(LabelLanguage("en")));
  audioObject->add(AudioComplementaryObjectGroupLabel());

  document->add(audioObject);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_labels"));
}