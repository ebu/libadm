/// @file frame_header.hpp
#pragma once

#include "adm/serial/frame_format.hpp"
#include "adm/serial/transport_track_format.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
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

    ADM_EXPORT FrameHeader(FrameStart start, FrameDuration duration,
                           FrameType frameType, FrameFormatId frameFormatId);
    ADM_EXPORT FrameHeader(const FrameHeader&) = default;
    ADM_EXPORT FrameHeader(FrameHeader&&) = default;
    ADM_EXPORT FrameHeader& operator=(const FrameHeader&) = default;
    ADM_EXPORT FrameHeader& operator=(FrameHeader&&) = default;

    ADM_EXPORT const FrameFormat& frameFormat() const;
    ADM_EXPORT FrameFormat& frameFormat();

    ADM_EXPORT void add(const TransportTrackFormat& transportTrackFormat);
    ADM_EXPORT void add(const FrameFormat& frameFormat);

    ADM_EXPORT TransportTrackFormatConstRange transportTrackFormats() const;
    ADM_EXPORT TransportTrackFormatRange transportTrackFormats();

    ADM_EXPORT void clearTransportTrackFormats();

    /**
     * @brief ADM parameter getter template
     *
     * Templated getter with the wanted ADM parameter type as template
     * argument. If currently no value is available trying to get the adm
     * parameter will result in an exception. Check with the has method before
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template
     *
     * Templated has method with the ADM parameter type as template argument.
     * Returns true if the ADM parameter is set or has a default value.
     */
    template <typename Parameter>
    bool has() const;

    /**
     * @brief ADM parameter isDefault template
     *
     * Templated isDefault method with the ADM parameter type as template
     * argument. Returns true if the ADM parameter is the default value.
     */
    template <typename Parameter>
    bool isDefault() const;

   private:
    FrameFormat frameFormat_;
    std::vector<TransportTrackFormat> transportTrackFormats_;
  };

}  // namespace adm
