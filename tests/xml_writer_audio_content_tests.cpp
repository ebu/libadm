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
  audioContent->set(LoudnessMetadata(
      LoudnessMethod("ITU-R BS.1770"), LoudnessRecType("EBU R128"),
      LoudnessCorrectionType("File-based"), IntegratedLoudness(-23.f),
      LoudnessRange(10.f), MaxTruePeak(-2.3f), MaxMomentary(-19.f),
      MaxShortTerm(-21.2), DialogueLoudness(-24.f)));
  audioContent->set(DialogueContent::AUDIO_DESCRIPTION);
  audioContent->add(
      AudioContentLabel(LabelLanguage("en"), LabelValue("My Content")));
  audioContent->add(
      AudioContentLabel(LabelLanguage("deu"), LabelValue("Mein Inhalt")));

  auto document = Document::create();
  document->add(audioContent);

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("write_audio_content"));
}
