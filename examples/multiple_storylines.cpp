#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;

  // create ADM elements
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"),
                                          AudioProgrammeLanguage("en"));
  auto lesleyStorylineContent =
      AudioContent::create(AudioContentName("Lesley"));
  auto billStorylineContent = AudioContent::create(AudioContentName("Bill"));
  auto introNarratorContent =
      AudioContent::create(AudioContentName("Intro Narrator"));
  auto introMusicContent =
      AudioContent::create(AudioContentName("Intro Music"));

  auto introNarrator = createSimpleObject("Intro Narrator");
  auto introMusic = createSimpleObject("Intro Music");
  auto speechLesley = createSimpleObject("Speech");
  auto ambienceLesley = createSimpleObject("Ambience");
  auto speechBill = createSimpleObject("Speech");
  auto ambienceBill = createSimpleObject("Ambience");

  introNarrator.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  introMusic.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  speechLesley.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  speechBill.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  ambienceLesley.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  ambienceBill.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));

  programme->addReference(lesleyStorylineContent);
  programme->addReference(billStorylineContent);
  programme->addReference(introNarratorContent);
  programme->addReference(introMusicContent);

  auto lesleyGroup = AudioObject::create(AudioObjectName("Lesley"));
  auto billGroup = AudioObject::create(AudioObjectName("Bill"));
  lesleyGroup->addComplementary(billGroup);

  introNarratorContent->addReference(introNarrator.audioObject);
  introMusicContent->addReference(introMusic.audioObject);
  lesleyStorylineContent->addReference(lesleyGroup);
  billStorylineContent->addReference(billGroup);

  lesleyGroup->addReference(speechLesley.audioObject);
  lesleyGroup->addReference(ambienceLesley.audioObject);
  billGroup->addReference(speechBill.audioObject);
  billGroup->addReference(ambienceBill.audioObject);

  auto admDocument = Document::create();
  admDocument->add(programme);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
