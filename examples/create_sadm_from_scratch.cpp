#include <iostream>
#include <sstream>
#include "adm/elements.hpp"
#include "adm/serial.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"

int main() {
  using namespace adm;
  auto uuid = "1f399874-dfa9-4a9b-82cd-fedc483d1223";
  auto frameFormatId = parseFrameFormatId("FF_00000001");
  auto frameHeader = FrameHeader{FrameFormat{frameFormatId, Start(std::chrono::milliseconds(0)),
                  Duration(std::chrono::milliseconds(40)),
                  FrameType(FrameTypeValue::FULL), FlowId{uuid}}};
  auto document = Document::create();

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

  document->add(admProgramme);

  reassignIds(document);

  auto trackFormat = TransportTrackFormat(TransportId(TransportIdValue(1)));

  AudioTrack audioTrack1(TrackId(1));
  audioTrack1.add(holder1.audioTrackUid->get<AudioTrackUidId>());
  trackFormat.add(audioTrack1);

  AudioTrack audioTrack2(TrackId(2));
  audioTrack2.add(holder2.audioTrackUid->get<AudioTrackUidId>());
  trackFormat.add(audioTrack2);

  frameHeader.add(trackFormat);

  // write XML data to stdout
  std::stringstream xmlStream;
  writeXml(xmlStream, document, frameHeader);
  std::cout << xmlStream.str();
  return 0;
}
