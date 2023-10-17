#include "adm/serial/frame_header.hpp"
#include "adm/utilities/element_io.hpp"

#include <iostream>

namespace adm {

  FrameHeader::FrameHeader(FrameFormat format)
      : frameFormat_{std::move(format)} {}

  const FrameFormat& FrameHeader::frameFormat() const { return frameFormat_; }
  FrameFormat& FrameHeader::frameFormat() { return frameFormat_; }

  void FrameHeader::add(const TransportTrackFormat& transportTrackFormat) {
    transportTrackFormats_.push_back(transportTrackFormat);
  }

  void FrameHeader::set(FrameFormat frameFormat) {
    frameFormat_ = std::move(frameFormat);
  }
  
  TransportTrackFormatConstRange FrameHeader::transportTrackFormats() const {
    return boost::make_iterator_range(transportTrackFormats_.cbegin(),
                                      transportTrackFormats_.cend());
  }

  TransportTrackFormatRange FrameHeader::transportTrackFormats() {
    return boost::make_iterator_range(transportTrackFormats_.begin(),
                                      transportTrackFormats_.end());
  }

  void FrameHeader::clearTransportTrackFormats() {
    transportTrackFormats_.clear();
  }

}  // namespace adm
