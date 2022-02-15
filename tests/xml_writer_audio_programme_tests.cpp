#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

TEST_CASE("write_audio_object_interaction") {
  using namespace adm;

  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  audioProgramme->set(AudioProgrammeId(AudioProgrammeIdValue(0x1001)));
  audioProgramme->set(AudioProgrammeLanguage("de"));
  audioProgramme->set(Start(std::chrono::seconds(0)));
  audioProgramme->set(End(std::chrono::seconds(10)));
  audioProgramme->set(MaxDuckingDepth(-30));
  audioProgramme->add(Label(LabelLanguage("en"), LabelValue("My Programme")));
  audioProgramme->add(Label(LabelLanguage("deu"), LabelValue("Mein Programm")));

  auto document = Document::create();
  document->add(audioProgramme);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_audio_programme"));
}
