/// @file audio_track.hpp
#pragma once

#include "adm/elements/audio_track_uid.hpp"
#include "adm/elements/format_descriptor.hpp"
#include "adm/elements_fwd.hpp"
//#include "adm/serialized_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include <boost/optional.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <memory>
#include <vector>

namespace adm {

  /// @brief AudioTrackUidId Container proxy (const)
  using AudioTrackUidIdConstRange =
      boost::iterator_range<std::vector<AudioTrackUidId>::const_iterator>;

  /// @brief AudioTrackUidId Container proxy
  using AudioTrackUidIdRange =
      boost::iterator_range<std::vector<AudioTrackUidId>::iterator>;

  /// @brief Tag for NamedType ::TrackId
  struct TrackIdTag {};
  /// @brief NamedType for a track id
  using TrackId = detail::NamedType<unsigned int, TrackIdTag>;

  /// @brief Tag for AudioTrack
  struct AudioTrackTag {};
  /**
   * @ingroup sadm
   * @brief Class representation for SADM element audioTrack
   *
   * Supported parameters:
   *   - ::TrackId
   *   - \link #FormatDescriptor FormatLabel\endlink
   *   - \link #FormatDescriptor FormatDefintion\endlink
   */
  class AudioTrack {
   public:
    typedef AudioTrackTag tag;
    /// Type that holds the id for this element;
    typedef TrackId id_type;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioTrack(TrackId trackId, Parameters... optionalNamedArgs);

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

    /// @brief TrackId setter
    ADM_EXPORT void set(TrackId trackId);
    /// @brief FormatDescriptor setter
    ADM_EXPORT void set(FormatDescriptor format);

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
     * @brief Add AudioTrackUidId
     */
    ADM_EXPORT void add(const AudioTrackUidId &audioTrackUidId);

    /**
     * @brief AudioTrackUidId container getter
     *
     * @returns ContainerProxy containing all AudioTrackUidIds.
     */
    ADM_EXPORT AudioTrackUidIdConstRange audioTrackUidIds() const;

    /**
     * @brief AudioTrackUidId container getter
     *
     * @returns ContainerProxy containing all AudioTrackUidIds.
     */
    ADM_EXPORT AudioTrackUidIdRange audioTrackUidIds();

    /**
     * @brief Clear AudioTrackUidIds
     *
     * Removes all audioTrackUidIds from the AudioTrack
     */
    ADM_EXPORT void clearAudioTrackUidIds();
    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

   private:
    ADM_EXPORT TrackId get(detail::ParameterTraits<TrackId>::tag) const;
    ADM_EXPORT FormatDescriptor
        get(detail::ParameterTraits<FormatDescriptor>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<FormatDescriptor>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<TrackId>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<FormatDescriptor>::tag);

    TrackId trackId_;
    boost::optional<FormatDescriptor> format_;

    std::vector<AudioTrackUidId> audioTrackUidIds_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioTrack::AudioTrack(TrackId trackId, Parameters... optionalNamedArgs) {
    set(trackId);
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter AudioTrack::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioTrack::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioTrack::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioTrack::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
