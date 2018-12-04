/// @file audio_block_format_binaural.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

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

    ADM_EXPORT AudioBlockFormatBinaural(const AudioBlockFormatBinaural&) =
        default;
    ADM_EXPORT AudioBlockFormatBinaural(AudioBlockFormatBinaural&&) = default;
    ADM_EXPORT AudioBlockFormatBinaural& operator=(
        const AudioBlockFormatBinaural&) = default;
    ADM_EXPORT AudioBlockFormatBinaural& operator=(AudioBlockFormatBinaural&&) =
        default;

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
    ADM_EXPORT void set(AudioBlockFormatId id);
    /// @brief Rtime setter
    ADM_EXPORT void set(Rtime rtime);
    /// @brief Duration setter
    ADM_EXPORT void set(Duration duration);

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
    ADM_EXPORT AudioBlockFormatId
        get(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT Rtime get(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT bool isDefault(detail::ParameterTraits<Rtime>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Rtime>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);

    AudioBlockFormatId id_;
    boost::optional<Rtime> rtime_;
    boost::optional<Duration> duration_;
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
