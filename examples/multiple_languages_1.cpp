#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;

  // create ADM elements
  auto programmeEn = AudioProgramme::create(
      AudioProgrammeName("English Programme"), AudioProgrammeLanguage("en"));
  auto programmeDe = AudioProgramme::create(
      AudioProgrammeName("German Programme"), AudioProgrammeLanguage("de"));
  auto contentEn = AudioContent::create(AudioContentName("Englisch Content"),
                                        AudioContentLanguage("en"));
  auto contentDe = AudioContent::create(AudioContentName("German Content"),
                                        AudioContentLanguage("de"));
  auto ambienceContent = AudioContent::create(AudioContentName("Ambience"));
  auto musicContent = AudioContent::create(AudioContentName("Music"));

  auto englishNarrator = createSimpleObject("English Narrator");
  auto germanNarrator = createSimpleObject("German Narrator");
  auto ambience = createSimpleObject("Ambience");
  auto music = createSimpleObject("Music");

  englishNarrator.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  germanNarrator.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  ambience.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  music.audioChannelFormat->add(AudioBlockFormatObjects(SphericalPosition()));

  programmeEn->addReference(contentEn);
  programmeEn->addReference(ambienceContent);
  programmeEn->addReference(musicContent);
  programmeDe->addReference(contentDe);
  programmeDe->addReference(ambienceContent);
  programmeDe->addReference(musicContent);

  contentEn->addReference(englishNarrator.audioObject);
  contentDe->addReference(germanNarrator.audioObject);
  ambienceContent->addReference(ambience.audioObject);
  musicContent->addReference(music.audioObject);

  auto admDocument = Document::create();
  admDocument->add(programmeEn);
  admDocument->add(programmeDe);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
