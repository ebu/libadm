#include <iostream>
#include <fstream>
#include <sstream>
#include "adm/common_definitions.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "adm/segmenter.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"

using namespace adm;

std::string genSadmFilename(std::string pref, uint32_t ind);
TrackUidList buildTrackList(std::shared_ptr<Document> document);

int main(int argc, char const *argv[]) {
  std::string fout_pre = "/tmp/segmenter_output";
  int size = 1000;
  int max_fr = 10;

  auto document = parseXml("../tests/test_data/segmenter_input.xml");

  // Get file length
  std::chrono::nanoseconds filelength((uint64_t)(max_fr * size) * 1000000L);
  for (auto programme : document->getElements<AudioProgramme>()) {
    if (programme->has<Start>() && programme->has<End>()) {
      filelength = programme->get<End>().get().asNanoseconds() - programme->get<Start>().get().asNanoseconds();
      break;
    } else if (programme->has<End>()) {
      filelength = programme->get<End>().get().asNanoseconds();
      break;
    }
  }
    
  auto trackUidList = buildTrackList(document);

  Segmenter segmenter(document, Time(filelength));
  SegmentStart segment_start(std::chrono::milliseconds(0));
  SegmentDuration segment_size(std::chrono::milliseconds{size});
  int fr = 0;
  while (segment_start < filelength && fr < max_fr) {
    segmenter.buildFrame(segment_start, segment_size, fr);
    auto transportTrackFormat = segmenter.generateTransportTrackFormat(trackUidList, 
                                                            segment_start, segment_size);
    auto frame = segmenter.getFrame();
    auto frameHeader = segmenter.getFrameHeader();

    std::stringstream xmlStream;
    writeXml(xmlStream, frame, *frameHeader);

    std::string fname = genSadmFilename(fout_pre, fr);
    std::ofstream sfile(fname);

    sfile << xmlStream.str();
    
    // Get ready for next frame
    xmlStream.str("");
    segment_start = SegmentStart(segment_start.get() + segment_size.get());
    fr++;
  }

  return 0;
}


std::string genSadmFilename(std::string pref, uint32_t ind) {
  char c[256];
  sprintf(c, "%s_%04d.xml", pref.c_str(), ind);
  std::string s(c);
  return s;
}

TrackUidList buildTrackList(std::shared_ptr<Document> document) {
  TrackUidList trackUidList;

  auto atus = document->getElements<AudioTrackUid>();
  uint16_t track_idx = 1;
  for (auto atu : atus) {
    TrackUid track_uid;
    track_uid.uid = formatId(atu->get<AudioTrackUidId>());
    track_uid.trackIndex = track_idx;
    trackUidList.trackUid.push_back(track_uid);
    track_idx++;
  } 
  return trackUidList;
}