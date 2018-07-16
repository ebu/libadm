/// @file audio_block_format_direct_speakers.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/initialize_block.hpp"
#include "adm/elements/speaker_position.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/libadm_export.h"

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::SpeakerLabel
  struct SpeakerLabelTag {};
  /// @brief NamedType for a speaker label
  using SpeakerLabel = detail::NamedType<std::string, SpeakerLabelTag>;
  /// @brief NamedType for all speaker labels of an AudioBlockFormat
  using SpeakerLabels = std::vector<SpeakerLabel>;
  ADD_TRAIT(SpeakerLabels, SpeakerLabelsTag);

  /// @brief Tag for AudioBlockFormatDirectSpeakers
  struct AudioBlockFormatDirectSpeakersTag {};
  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "DirectSpeakers"
   */
  class AudioBlockFormatDirectSpeakers {
   public:
    typedef AudioBlockFormatDirectSpeakersTag tag;

    template <typename... Parameters>
    AudioBlockFormatDirectSpeakers(Parameters... optionalNamedArgs);

    LIBADM_EXPORT AudioBlockFormatDirectSpeakers(
        const AudioBlockFormatDirectSpeakers&) = default;
    LIBADM_EXPORT AudioBlockFormatDirectSpeakers(
        AudioBlockFormatDirectSpeakers&&) = default;
    LIBADM_EXPORT AudioBlockFormatDirectSpeakers& operator=(
        const AudioBlockFormatDirectSpeakers&) = default;
    LIBADM_EXPORT AudioBlockFormatDirectSpeakers& operator=(
        AudioBlockFormatDirectSpeakers&&) = default;
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

    LIBADM_EXPORT bool isDefault(detail::ParameterTraits<Rtime>::tag) const;

    /// @brief AudioBlockFormatId setter
    LIBADM_EXPORT void set(AudioBlockFormatId id);
    /// @brief Rtime setter
    LIBADM_EXPORT void set(Rtime rtime);
    /// @brief Duration setter
    LIBADM_EXPORT void set(Duration duration);
    /// @brief lstart setter
    LIBADM_EXPORT void set(Lstart lstart);
    /// @brief Lduration setter
    LIBADM_EXPORT void set(Lduration lduration);
    /// @brief InitializeBlock setter
    LIBADM_EXPORT void set(InitializeBlock initializeBlock);
    /// @brief SpeakerPosition setter
    LIBADM_EXPORT void set(SpeakerPosition speakerPosition);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    /// @brief Add a SpeakerLabel
    LIBADM_EXPORT bool add(SpeakerLabel label);
    /// @brief remove a SpeakerLabel
    LIBADM_EXPORT void remove(SpeakerLabel label);

   private:
    LIBADM_EXPORT AudioBlockFormatId
        get(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    LIBADM_EXPORT Rtime get(detail::ParameterTraits<Rtime>::tag) const;
    LIBADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;
    LIBADM_EXPORT Lstart get(detail::ParameterTraits<Lstart>::tag) const;
    LIBADM_EXPORT Lduration get(detail::ParameterTraits<Lduration>::tag) const;
    LIBADM_EXPORT InitializeBlock
        get(detail::ParameterTraits<InitializeBlock>::tag) const;
    LIBADM_EXPORT SpeakerLabels
        get(detail::ParameterTraits<SpeakerLabels>::tag) const;
    LIBADM_EXPORT SpeakerPosition
        get(detail::ParameterTraits<SpeakerPosition>::tag) const;

    LIBADM_EXPORT bool has(
        detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Rtime>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Lstart>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Lduration>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<InitializeBlock>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<SpeakerLabels>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<SpeakerPosition>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    LIBADM_EXPORT void unset(detail::ParameterTraits<Rtime>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Lstart>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Lduration>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<InitializeBlock>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<SpeakerLabels>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<SpeakerPosition>::tag);

    AudioBlockFormatId id_;
    boost::optional<Rtime> rtime_;
    boost::optional<Duration> duration_;
    boost::optional<Lstart> lstart_;
    boost::optional<Lduration> lduration_;
    boost::optional<InitializeBlock> initializeBlock_;
    SpeakerLabels speakerLabels_;
    boost::optional<SpeakerPosition> speakerPosition_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioBlockFormatDirectSpeakers::AudioBlockFormatDirectSpeakers(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter AudioBlockFormatDirectSpeakers::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatDirectSpeakers::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatDirectSpeakers::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioBlockFormatDirectSpeakers::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
