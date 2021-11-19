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
  audioContent->add(Label(LabelLanguage("en"), LabelValue("My Content")));
  audioContent->add(Label(LabelValue("My Content")));
  audioContent->add(Label(LabelLanguage("en")));
  audioContent->add(Label());
  document->add(audioContent);

  // audioProgramme
  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  audioProgramme->add(Label(LabelLanguage("en"), LabelValue("My Programme")));
  audioProgramme->add(Label(LabelValue("My Programme")));
  audioProgramme->add(Label(LabelLanguage("en")));
  audioProgramme->add(Label());
  document->add(audioProgramme);

  // audioObject including complementaryObjectGroup
  auto audioObject = AudioObject::create(AudioObjectName("MyObject"));
  audioObject->add(Label(LabelLanguage("en"), LabelValue("My Object")));
  audioObject->add(Label(LabelValue("My Object")));
  audioObject->add(Label(LabelLanguage("en")));
  audioObject->add(Label());

  audioObject->add(AudioComplementaryObjectGroupLabel(
      Label(LabelLanguage("en"), LabelValue("My ComplementaryObjectGroup"))));
  audioObject->add(AudioComplementaryObjectGroupLabel(
      Label(LabelValue("My ComplementaryObjectGroup"))));
  audioObject->add(
      AudioComplementaryObjectGroupLabel(Label(LabelLanguage("en"))));
  audioObject->add(AudioComplementaryObjectGroupLabel(Label()));

  document->add(audioObject);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_labels"));
}
