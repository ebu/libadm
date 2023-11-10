#pragma once

#include <boost/optional.hpp>
#include <memory>

#include "adm/serial/transport_id.hpp"
#include "adm/serial/audio_track.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"

namespace adm {

  using AudioTrackConstRange =
      boost::iterator_range<std::vector<AudioTrack>::const_iterator>;
  using AudioTrackRange =
      boost::iterator_range<std::vector<AudioTrack>::iterator>;

  /// @brief Tag for NamedType ::TransportName
  struct TransportNameTag {};
  /// @brief NamedType for the TransportName attribute
  using TransportName = detail::NamedType<std::string, TransportNameTag>;
  /// @brief Tag for NamedType ::NumTracks
  struct NumTracksTag {};
  /// @brief NamedType for the NumTracks attribute
  using NumTracks = detail::NamedType<unsigned int, NumTracksTag>;
  /// @brief Tag for NamedType ::NumIds
  struct NumIdsTag {};
  /// @brief NamedType for the NumIds attribute
  using NumIds = detail::NamedType<unsigned int, NumIdsTag>;

  namespace detail {
    using TransportTrackFormatBase =
        HasParameters<RequiredParameter<TransportId>,
                      OptionalParameter<TransportName>,
                      OptionalParameter<NumTracks>, OptionalParameter<NumIds>>;
  }

  /// @brief Tag for TransportTrackFormat
  struct TransportTrackFormatTag {};
  /**
   * @ingroup sadm
   * @brief Class representation of the transportTrackFormat SADM element
   *
   * Supported parameters:
   *   - TransportId
   *   - ::TransportName
   *   - ::NumTracks
   *   - ::NumIds
   */
  class TransportTrackFormat
      : public detail::TransportTrackFormatBase,
        private detail::AddWrapperMethods<TransportTrackFormat> {
   public:
    using tag = TransportTrackFormatTag;
    /// Type that holds the id for this element;
    using id_type = TransportId;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    explicit TransportTrackFormat(TransportId id,
                                  Parameters... optionalNamedArgs);

    using detail::AddWrapperMethods<TransportTrackFormat>::has;
    using detail::AddWrapperMethods<TransportTrackFormat>::get;
    using detail::AddWrapperMethods<TransportTrackFormat>::unset;
    using detail::AddWrapperMethods<TransportTrackFormat>::isDefault;
    using detail::TransportTrackFormatBase::get;
    using detail::TransportTrackFormatBase::has;
    using detail::TransportTrackFormatBase::isDefault;
    using detail::TransportTrackFormatBase::set;
    using detail::TransportTrackFormatBase::unset;

    /**
     * @brief Add AudioTrack
     */
    ADM_EXPORT void add(const AudioTrack &audioTrack);

    /**
     * @brief AudioTrack container getter
     *
     * @returns ContainerProxy containing all AudioTracks.
     */
    ADM_EXPORT AudioTrackConstRange audioTracks() const;

    /**
     * @brief AudioTrack container getter
     *
     * @returns ContainerProxy containing all AudioTracks.
     */
    ADM_EXPORT AudioTrackRange audioTracks();

    /**
     * @brief Clear AudioTracks
     *
     * Removes all audioTracks from the TransportTrackFormat
     */
    ADM_EXPORT void clearAudioTracks();

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;
    friend class detail::AddWrapperMethods<TransportTrackFormat>;

   private:
    std::vector<AudioTrack> audioTracks_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  TransportTrackFormat::TransportTrackFormat(TransportId id,
                                             Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(this, std::move(id),
                                 std::move(optionalNamedArgs)...);
  }
}  // namespace adm
