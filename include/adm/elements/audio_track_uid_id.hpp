/// @file audio_track_uid_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/libadm_export.h"

namespace adm {

  /// @brief Tag for AudioTrackUidIdValue
  struct AudioTrackUidIdValueTag {};
  /// @brief NamedType for the AudioTrackUidIdValue attribute
  using AudioTrackUidIdValue =
      detail::NamedType<unsigned int, AudioTrackUidIdValueTag>;

  /// @brief Tag for AudioTrackUidId
  struct AudioTrackUidIdTag {};
  /// @brief Representation of an AudioTrackUidId
  class AudioTrackUidId {
   public:
    typedef AudioTrackUidIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioTrackUidId(Parameters... optionalNamedArgs);

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

    /// @brief Set value
    LIBADM_EXPORT void set(AudioTrackUidIdValue value);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    ///@{
    /**
     * @brief Operator overload
     *
     * Compares the string representation of the AudioTrackUidId.
     */
    LIBADM_EXPORT bool operator==(const AudioTrackUidId& other) const;
    LIBADM_EXPORT bool operator!=(const AudioTrackUidId& other) const;
    LIBADM_EXPORT bool operator<(const AudioTrackUidId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    LIBADM_EXPORT AudioTrackUidIdValue
        get(detail::ParameterTraits<AudioTrackUidIdValue>::tag) const;

    LIBADM_EXPORT bool has(
        detail::ParameterTraits<AudioTrackUidIdValue>::tag) const;

    LIBADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioTrackUidIdValue>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    LIBADM_EXPORT void unset(
        detail::ParameterTraits<AudioTrackUidIdValue>::tag);

    boost::optional<AudioTrackUidIdValue> value_;

    static const AudioTrackUidIdValue valueDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioTrackUidId string and convert it into an object
  LIBADM_EXPORT AudioTrackUidId parseAudioTrackUidId(const std::string& id);
  /// @brief Format an AudioTrackUidId object as string
  LIBADM_EXPORT std::string formatId(AudioTrackUidId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioTrackUidId::AudioTrackUidId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioTrackUidId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioTrackUidId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioTrackUidId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioTrackUidId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
