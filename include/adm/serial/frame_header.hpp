/// @file frame_header.hpp
#pragma once

#include "adm/serial/frame_format.hpp"
#include "adm/serial/transport_track_format.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/auto_base.hpp"
#include "adm/elements/profile_list.hpp"
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
  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<ProfileList>;

    using FrameHeaderBase = HasParameters<OptionalParameter<ProfileList>>;
  }  // namespace detail
  /**
   * @ingroup sadm
   * @brief Class representation of the frameHeader SADM element
   *
   * Supported elements:
   *   - FrameFormat
   *   - ProfileList
   *   - TransportTrackFormat
   */
  class FrameHeader : public detail::FrameHeaderBase,
                      private detail::AddWrapperMethods<FrameHeader> {
   public:
    typedef FrameHeaderTag tag;

    template <typename... Parameters>
    ADM_EXPORT explicit FrameHeader(FrameFormat format,
                                    Parameters&&... parameters);
    ADM_EXPORT FrameHeader(const FrameHeader&) = default;
    ADM_EXPORT FrameHeader(FrameHeader&&) = default;
    ADM_EXPORT FrameHeader& operator=(const FrameHeader&) = default;
    ADM_EXPORT FrameHeader& operator=(FrameHeader&&) = default;

    ADM_EXPORT void add(const TransportTrackFormat& transportTrackFormat);
    ADM_EXPORT void set(FrameFormat frameFormat);

    ADM_EXPORT TransportTrackFormatConstRange transportTrackFormats() const;
    ADM_EXPORT TransportTrackFormatRange transportTrackFormats();

    ADM_EXPORT void clearTransportTrackFormats();

    using detail::FrameHeaderBase::get;
    using detail::FrameHeaderBase::has;
    using detail::FrameHeaderBase::isDefault;
    using detail::FrameHeaderBase::set;
    using detail::FrameHeaderBase::unset;
    using detail::AddWrapperMethods<FrameHeader>::has;
    using detail::AddWrapperMethods<FrameHeader>::get;
    using detail::AddWrapperMethods<FrameHeader>::isDefault;
    using detail::AddWrapperMethods<FrameHeader>::unset;
    friend class detail::AddWrapperMethods<FrameHeader>;

   private:
    ADM_EXPORT inline FrameFormat get(FrameFormat::tag) const {
      return frameFormat_;
    }
    ADM_EXPORT inline bool has(FrameFormat::tag) const { return true; }
    ADM_EXPORT inline bool is_default(FrameFormat::tag) const { return false; }

    FrameFormat frameFormat_;
    std::vector<TransportTrackFormat> transportTrackFormats_;
  };

  template <typename... Parameters>
  FrameHeader::FrameHeader(FrameFormat format, Parameters&&... parameters)
      : frameFormat_{std::move(format)} {
    detail::setNamedOptionHelper(this, std::forward<Parameters>(parameters)...);
  }
}  // namespace adm
