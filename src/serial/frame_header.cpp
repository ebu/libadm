#include "adm/serial/frame_header.hpp"
#include "adm/utilities/element_io.hpp"

#include <iostream>

namespace adm {

  void FrameHeader::set(FrameFormat frameFormat) {
    frameFormat_ = std::move(frameFormat);
  }
}  // namespace adm
