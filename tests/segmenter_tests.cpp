#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/segmenter.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/write.hpp"
#include "adm/parse.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TrackUidList buildTrackList(std::shared_ptr<Document> document);

TEST_CASE("write_sadm_frames") {
  auto document = parseXml("segmenter_input.xml");
  std::chrono::nanoseconds filelength((uint64_t)(10 * 1000000000L)); // 10 seconds

  auto trackUidList = buildTrackList(document);

  Segmenter segmenter(document, Time(filelength));
  SegmentStart segment_start(std::chrono::milliseconds(0));
  SegmentDuration segment_size(std::chrono::milliseconds{1000}); // 1 second frame
  int fr = 0;
  while (segment_start < filelength) {
    segmenter.buildFrame(segment_start, segment_size, fr);
    auto transportTrackFormat = segmenter.generateTransportTrackFormat(trackUidList, 
                                                            segment_start, segment_size);
    auto frame = segmenter.getFrame();
    auto frameHeader = segmenter.getFrameHeader();

    std::stringstream xmlStream;
    writeXml(xmlStream, frame, *frameHeader);

    char c[256];
    sprintf(c, "segmenter_output_%04d", fr);
    std::string out_name(c);

    CHECK_THAT(xmlStream.str(), EqualsXmlFile(out_name));

    // Get ready for next frame
    xmlStream.str("");
    segment_start = SegmentStart(segment_start.get() + segment_size.get());
    fr++;
  }
}

// This would normally be generated from the chna chunk.
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
