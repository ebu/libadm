/// @file audio_track.hpp
#pragma once

#include "adm/elements/audio_track_uid.hpp"
#include "adm/elements/format_descriptor.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include <boost/optional.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <memory>
#include <vector>

namespace adm {
  /// @brief Tag for NamedType ::TrackId
  struct TrackIdTag {};
  /// @brief NamedType for a track id
  using TrackId = detail::NamedType<unsigned int, TrackIdTag>;

  struct AudioTrackUidRefsTag {};
  using AudioTrackUidRefs = std::vector<AudioTrackUidId>;

  namespace detail {
    template <>
    struct ParameterTraits<AudioTrackUidRefs> {
      using tag = AudioTrackUidRefsTag;
    };

    using AudioTrackBase = HasParameters<RequiredParameter<TrackId>,
                                         DefaultParameter<FormatDescriptor>,
                                         VectorParameter<AudioTrackUidRefs>>;
  }  // namespace detail

  /// @brief Tag for AudioTrack
  struct AudioTrackTag {};
  /**
   * @ingroup sadm
   * @brief Class representation of the audioTrack SADM element
   *
   * \rst
   * +----------------------+-------------------------------+------------------------------+
   * | ADM Parameter        | Parameter Type                | Pattern Type                 |
   * +======================+===============================+==============================+
   * | trackID              | :class:`TrackId`              | :class:`RequiredParameter`   |
   * +----------------------+-------------------------------+------------------------------+
   * | formatLabel          | :class:`FormatDescriptor`     | :class:`OptionalParameter`   |
   * +----------------------+-------------------------------+------------------------------+
   * | formatDefinition     | :class:`FormatDescriptor`     | :class:`OptionalParameter`   |
   * +----------------------+-------------------------------+------------------------------+
   * | audioTrackUIDRef     | :class:`AudioTrackUidRefs`    | :class:`VectorParameter`     |
   * +----------------------+-------------------------------+------------------------------+
   * \endrst
   */
  class AudioTrack : public detail::AudioTrackBase,
                     private detail::AddWrapperMethods<AudioTrack> {
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

    using detail::AddWrapperMethods<AudioTrack>::get;
    using detail::AddWrapperMethods<AudioTrack>::unset;
    using detail::AddWrapperMethods<AudioTrack>::isDefault;
    using detail::AddWrapperMethods<AudioTrack>::has;
    using detail::AudioTrackBase::add;
    using detail::AudioTrackBase::get;
    using detail::AudioTrackBase::has;
    using detail::AudioTrackBase::isDefault;
    using detail::AudioTrackBase::remove;
    using detail::AudioTrackBase::set;
    using detail::AudioTrackBase::unset;

    void print(std::ostream& os) const;

    friend bool operator==(AudioTrack const& lhs, AudioTrack const& rhs) {
      auto result = lhs.get<TrackId>() == rhs.get<TrackId>();
      result =
          result && lhs.get<FormatDescriptor>() == rhs.get<FormatDescriptor>();
      result = result &&
               lhs.get<AudioTrackUidRefs>() == rhs.get<AudioTrackUidRefs>();
      return result;
    }

    friend class detail::AddWrapperMethods<AudioTrack>;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioTrack::AudioTrack(TrackId trackId, Parameters... optionalNamedArgs) {
    set(trackId);
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }
}  // namespace adm
