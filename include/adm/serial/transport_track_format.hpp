/// @file transport_track_format.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>

#include "adm/serial/transport_id.hpp"
#include "adm/serial/audio_track.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

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

  struct AudioTracksTag {};
  using AudioTracks = std::vector<AudioTrack>;

  namespace detail {
    template <>
    struct ParameterTraits<AudioTracks> {
      using tag = AudioTracksTag;
    };
    using TransportTrackFormatBase =
        HasParameters<RequiredParameter<TransportId>,
                      OptionalParameter<TransportName>,
                      OptionalParameter<NumTracks>, OptionalParameter<NumIds>,
                      VectorParameter<AudioTracks>>;
  }  // namespace detail

  /// @brief Tag for TransportTrackFormat
  struct TransportTrackFormatTag {};

  /**
   * @ingroup sadm
   * @brief Class representation of the transportTrackFormat SADM element
   *
   * \rst
   * +----------------------+----------------------------+------------------------------+
   * | ADM Parameter        | Parameter Type             | Pattern Type                 |
   * +======================+============================+==============================+
   * | transportID          | :class:`TransportId`       | :class:`RequiredParameter`   |
   * +----------------------+----------------------------+------------------------------+
   * | transportName        | :type:`TransportName`      | :class:`OptionalParameter`   |
   * +----------------------+----------------------------+------------------------------+
   * | numTracks            | :type:`NumTracks`          | :class:`OptionalParameter`   |
   * +----------------------+----------------------------+------------------------------+
   * | numIDs               | :type:`NumIds`             | :class:`OptionalParameter`   |
   * +----------------------+----------------------------+------------------------------+
   * | audioTrack           | :type:`AudioTracks`        | :class:`VectorParameter`     |
   * +----------------------+----------------------------+------------------------------+
   * \endrst
   */
  class TransportTrackFormat
      : private detail::TransportTrackFormatBase,
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
    using detail::TransportTrackFormatBase::add;
    using detail::TransportTrackFormatBase::remove;
    using detail::TransportTrackFormatBase::set;

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;
    friend class detail::AddWrapperMethods<TransportTrackFormat>;

   private:
    using detail::TransportTrackFormatBase::get;
    using detail::TransportTrackFormatBase::has;
    using detail::TransportTrackFormatBase::isDefault;
    using detail::TransportTrackFormatBase::unset;
  };

  inline bool operator==(TransportTrackFormat const& lhs,
                         TransportTrackFormat const& rhs) {
    return detail::optionalsEqual<TransportId, TransportName, NumTracks, NumIds,
                                  AudioTracks>(lhs, rhs);
  }

  // ---- Implementation ---- //

  template <typename... Parameters>
  TransportTrackFormat::TransportTrackFormat(TransportId id,
                                             Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(this, std::move(id),
                                 std::move(optionalNamedArgs)...);
  }
}  // namespace adm
