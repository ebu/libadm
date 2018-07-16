/// @file frame_header.hpp
#pragma once

#include "adm/serialized/frame_format.hpp"
#include "adm/serialized/transport_track_format.hpp"
#include "adm/serialized_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/libadm_export.h"
#include <boost/range/iterator_range_core.hpp>
#include <memory>
#include <vector>

namespace adm {

  class Frame;

  using TransportTrackFormatConstRange =
      boost::iterator_range<std::vector<TransportTrackFormat>::const_iterator>;
  using TransportTrackFormatRange =
      boost::iterator_range<std::vector<TransportTrackFormat>::iterator>;

  /// @brief Tag for FrameHeader
  struct FrameHeaderTag {};
  /**
   * @ingroup sadm
   * @brief Class representation of the frameHeader SADM element
   *
   * Supported elements:
   *   - FrameFormat
   *   - TransportTrackFormat
   */

  class FrameHeader {
   public:
    typedef FrameHeaderTag tag;

    LIBADM_EXPORT FrameHeader(FrameStart start, FrameDuration duration,
                              FrameType frameType);
    LIBADM_EXPORT FrameHeader(const FrameHeader&) = default;
    LIBADM_EXPORT FrameHeader(FrameHeader&&) = default;
    LIBADM_EXPORT FrameHeader& operator=(const FrameHeader&) = default;
    LIBADM_EXPORT FrameHeader& operator=(FrameHeader&&) = default;

    LIBADM_EXPORT const FrameFormat& frameFormat() const;
    LIBADM_EXPORT FrameFormat& frameFormat();

    LIBADM_EXPORT void add(const TransportTrackFormat& transportTrackFormat);

    LIBADM_EXPORT TransportTrackFormatConstRange transportTrackFormats() const;
    LIBADM_EXPORT TransportTrackFormatRange transportTrackFormats();

    LIBADM_EXPORT void clearTransportTrackFormats();

   private:
    FrameFormat frameFormat_;
    std::vector<TransportTrackFormat> transportTrackFormats_;
  };

}  // namespace adm
