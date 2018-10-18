#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;

  // create ADM elements
  auto admProgramme = AudioProgramme::create(
      AudioProgrammeName("Alice and Bob talking in the forrest"));
  auto speechContent = AudioContent::create(AudioContentName("Speech"));
  auto atmoContent = AudioContent::create(AudioContentName("Atmo"));

  admProgramme->addReference(speechContent);
  admProgramme->addReference(atmoContent);
  auto holder = createSimpleObject("Alice");
  speechContent->addReference(holder.audioObject);

  holder = createSimpleObject("Bob");
  speechContent->addReference(holder.audioObject);

  auto admDocument = Document::create();
  admDocument->add(admProgramme);
  reassignIds(admDocument);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
