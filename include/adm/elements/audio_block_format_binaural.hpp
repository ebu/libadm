/// @file audio_block_format_binaural.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/initialize_block.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/libadm_export.h"

namespace adm {

  class Document;

  /// @brief Tag for AudioBlockFormatBinaural
  struct AudioBlockFormatBinauralTag {};
  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "Binaural"
   */
  class AudioBlockFormatBinaural {
   public:
    typedef AudioBlockFormatBinauralTag tag;
    /// Type that holds the id for this element;
    typedef AudioBlockFormatId id_type;

    template <typename... Parameters>
    AudioBlockFormatBinaural(Parameters... optionalNamedArgs);

    LIBADM_EXPORT AudioBlockFormatBinaural(const AudioBlockFormatBinaural&) =
        default;
    LIBADM_EXPORT AudioBlockFormatBinaural(AudioBlockFormatBinaural&&) =
        default;
    LIBADM_EXPORT AudioBlockFormatBinaural& operator=(
        const AudioBlockFormatBinaural&) = default;
    LIBADM_EXPORT AudioBlockFormatBinaural& operator=(
        AudioBlockFormatBinaural&&) = default;

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

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

   private:
    LIBADM_EXPORT AudioBlockFormatId
        get(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    LIBADM_EXPORT Rtime get(detail::ParameterTraits<Rtime>::tag) const;
    LIBADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;
    LIBADM_EXPORT Lstart get(detail::ParameterTraits<Lstart>::tag) const;
    LIBADM_EXPORT Lduration get(detail::ParameterTraits<Lduration>::tag) const;
    LIBADM_EXPORT InitializeBlock
        get(detail::ParameterTraits<InitializeBlock>::tag) const;

    LIBADM_EXPORT bool has(
        detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Rtime>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Lstart>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<Lduration>::tag) const;
    LIBADM_EXPORT bool has(detail::ParameterTraits<InitializeBlock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    LIBADM_EXPORT bool isDefault(detail::ParameterTraits<Rtime>::tag) const;

    LIBADM_EXPORT void unset(detail::ParameterTraits<Rtime>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Lstart>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<Lduration>::tag);
    LIBADM_EXPORT void unset(detail::ParameterTraits<InitializeBlock>::tag);

    AudioBlockFormatId id_;
    boost::optional<Rtime> rtime_;
    boost::optional<Duration> duration_;
    boost::optional<Lstart> lstart_;
    boost::optional<Lduration> lduration_;
    boost::optional<InitializeBlock> initializeBlock_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioBlockFormatBinaural::AudioBlockFormatBinaural(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }

  template <typename Parameter>
  Parameter AudioBlockFormatBinaural::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatBinaural::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatBinaural::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioBlockFormatBinaural::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
