#include <iostream>
#include <sstream>
#include "adm/adm.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "adm/frame.hpp"

int main() {
  using namespace adm;

  auto frameFormatId = parseFrameFormatId("FF_00000000001");
  auto frame = Frame::create(FrameStart(std::chrono::milliseconds(0)),
                             FrameDuration(std::chrono::milliseconds(40)),
                             FrameType("full"),
                             frameFormatId);

  // create ADM elements
  auto admProgramme = AudioProgramme::create(
      AudioProgrammeName("Alice and Bob talking in the forrest"));
  auto speechContent = AudioContent::create(AudioContentName("Speech"));
  auto atmoContent = AudioContent::create(AudioContentName("Atmo"));

  admProgramme->addReference(speechContent);
  admProgramme->addReference(atmoContent);
  auto holder1 = createSimpleObject("Alice");
  speechContent->addReference(holder1.audioObject);

  auto holder2 = createSimpleObject("Bob");
  speechContent->addReference(holder2.audioObject);

  frame->add(admProgramme);

  reassignIds(frame);

  auto trackFormat = TransportTrackFormat();

  AudioTrack audioTrack1(TrackId(1));
  audioTrack1.add(holder1.audioTrackUid->get<AudioTrackUidId>());
  trackFormat.add(audioTrack1);

  AudioTrack audioTrack2(TrackId(2));
  audioTrack2.add(holder2.audioTrackUid->get<AudioTrackUidId>());
  trackFormat.add(audioTrack2);

  frame->frameHeader().add(trackFormat);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXmlSadm(xmlStream, frame);
  std::cout << xmlStream.str();
  return 0;
}
