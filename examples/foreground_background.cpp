#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;

  // create ADM elements
  auto admProgramme = AudioProgramme::create(
      AudioProgrammeName("Foreground Background Example"));
  auto backgroundContent = AudioContent::create(AudioContentName("Background"));
  auto foregroundContent_1 =
      AudioContent::create(AudioContentName("Foreground 1"));
  auto foregroundContent_2 =
      AudioContent::create(AudioContentName("Foreground 2"));
  auto backgroundGroupObject = AudioObject::create(
      AudioObjectName("BackgroundGroup"),
      AudioObjectInteraction(OnOffInteract(false), GainInteract(true),
                             GainInteractionRange(GainInteractionMin(0.5f),
                                                  GainInteractionMax(1.f))));
  auto backgroundHolder_1 = createSimpleObject("Background Object 1");
  auto backgroundHolder_2 = createSimpleObject("Background Object 2");
  auto foregroundHolder_1 = createSimpleObject("Foreground Object 1");
  auto foregroundHolder_2 = createSimpleObject("Foreground Object 2");

  backgroundHolder_1.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  backgroundHolder_2.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  foregroundHolder_1.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));
  foregroundHolder_2.audioChannelFormat->add(
      AudioBlockFormatObjects(SphericalPosition()));

  admProgramme->addReference(backgroundContent);
  admProgramme->addReference(foregroundContent_1);
  admProgramme->addReference(foregroundContent_2);

  backgroundContent->addReference(backgroundGroupObject);
  foregroundContent_1->addReference(foregroundHolder_1.audioObject);
  foregroundContent_2->addReference(foregroundHolder_2.audioObject);

  backgroundGroupObject->addReference(backgroundHolder_1.audioObject);
  backgroundGroupObject->addReference(backgroundHolder_2.audioObject);

  auto admDocument = Document::create();
  admDocument->add(admProgramme);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, admDocument);
  std::cout << xmlStream.str();
  return 0;
}
