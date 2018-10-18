#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;

  // create ADM elements
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto programmeVoiceOver =
      AudioProgramme::create(AudioProgrammeName("Programme - Voice Over"));

  auto speechContent = AudioContent::create(AudioContentName("Speech"));
  auto speechDuckedContent =
      AudioContent::create(AudioContentName("Speech Ducked"));
  auto voiceOverContent = AudioContent::create(AudioContentName("Voice Over"));

  auto speech = createSimpleObject("Speech");
  auto speechDucked = createSimpleObject("Speech Ducked");
  auto voiceOver = createSimpleObject("Voice Over");

  speech.audioChannelFormat->add(AudioBlockFormatObjects(SphericalPosition()));
  speechDucked.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  voiceOver.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));

  programme->addReference(speechContent);
  programmeVoiceOver->addReference(speechDuckedContent);
  programmeVoiceOver->addReference(voiceOverContent);

  speechContent->addReference(speech.audioObject);
  speechDuckedContent->addReference(speechDucked.audioObject);
  voiceOverContent->addReference(voiceOver.audioObject);

  auto admDocument = Document::create();
  admDocument->add(programme);
  admDocument->add(programmeVoiceOver);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
