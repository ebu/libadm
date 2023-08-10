#include <iostream>

#include "adm/frame.hpp"

#include <algorithm>
#include "adm/serial/frame_header.hpp"
#include "adm/serial/frame_format.hpp"
#include "adm/serial/transport_track_format.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/utilities/lookup.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/detail/id_assigner.hpp"

namespace adm {
  Frame::Frame(FrameStart start, FrameDuration duration, FrameType frameType, FrameFormatId frameFormatId)
      : frameHeader_(FrameHeader(start, duration, frameType, frameFormatId)) {
  }

  Frame::Frame(FrameHeader frameHeader) : frameHeader_(frameHeader) {
  std::shared_ptr<Frame> Frame::create(FrameStart start, FrameDuration duration,
                                       FrameType frameType, FrameFormatId frameFormatId) {
    return std::shared_ptr<Frame>(new Frame(start, duration, frameType, frameFormatId));
  }

  std::shared_ptr<Frame> Frame::create(FrameHeader frameHeader) {
    auto frame = std::shared_ptr<Frame>(new Frame(frameHeader));
    
    // Generate the ATU_00000000 audioTrackUID and add to frame
    auto audioTrackUid_zero = AudioTrackUid::create(AudioTrackUidIdValue(0));
    frame->add(audioTrackUid_zero);
    
    return frame;
  }

  FrameStart Frame::start() const {
    return frameHeader().frameFormat().get<adm::FrameStart>();
  }

  FrameDuration Frame::duration() const {
    return frameHeader().frameFormat().get<adm::FrameDuration>();
  }

  std::shared_ptr<Frame> Frame::deepCopy() const {
      return adm::deepCopy(std::static_pointer_cast<Frame const>(shared_from_this()));
  }

  const FrameHeader& Frame::frameHeader() const { return frameHeader_; }
  FrameHeader& Frame::frameHeader() { return frameHeader_; }
  void Frame::setFrameHeader(FrameHeader frameHeader) { frameHeader_ = frameHeader; }

    std::shared_ptr<Document> Frame::getDocument() { return shared_from_this(); }
}  // namespace adm
