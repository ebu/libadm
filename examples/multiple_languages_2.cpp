#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;

  // create ADM elements
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
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
  englishNarrator.audioObject->addComplementary(germanNarrator.audioObject);
  germanNarrator.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  ambience.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  music.audioChannelFormat->add(AudioBlockFormatObjects(SphericalPosition()));

  programme->addReference(contentEn);
  programme->addReference(contentDe);
  programme->addReference(ambienceContent);
  programme->addReference(musicContent);

  contentEn->addReference(englishNarrator.audioObject);
  contentDe->addReference(germanNarrator.audioObject);
  ambienceContent->addReference(ambience.audioObject);
  musicContent->addReference(music.audioObject);

  auto admDocument = Document::create();
  admDocument->add(programme);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
