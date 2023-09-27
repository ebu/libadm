#pragma once

#include <boost/optional.hpp>
#include <memory>

#include "adm/serial/transport_id.hpp"
#include "adm/serial/audio_track.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

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
  class TransportTrackFormat {
   public:
    typedef TransportTrackFormatTag tag;
    /// Type that holds the id for this element;
    typedef TransportId id_type;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    TransportTrackFormat(Parameters... optionalNamedArgs);

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

    /// @brief TransportId setter
    ADM_EXPORT void set(TransportId id);
    /// @brief TransportName setter
    ADM_EXPORT void set(TransportName name);
    /// @brief TransportId setter
    ADM_EXPORT void set(NumTracks numTracks);
    /// @brief TransportName setter
    ADM_EXPORT void set(NumIds numIds);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

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

   private:
    ADM_EXPORT TransportId get(detail::ParameterTraits<TransportId>::tag) const;
    ADM_EXPORT TransportName
        get(detail::ParameterTraits<TransportName>::tag) const;
    ADM_EXPORT NumTracks get(detail::ParameterTraits<NumTracks>::tag) const;
    ADM_EXPORT NumIds get(detail::ParameterTraits<NumIds>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<TransportId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<TransportName>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<NumTracks>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<NumIds>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<TransportName>::tag);

    TransportId transportId_;
    boost::optional<TransportName> transportName_;
    std::vector<AudioTrack> audioTracks_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  TransportTrackFormat::TransportTrackFormat(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter TransportTrackFormat::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool TransportTrackFormat::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool TransportTrackFormat::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void TransportTrackFormat::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
