/// @file audio_block_format_binaural.hpp
#pragma once

#include <boost/optional.hpp>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/common_parameters.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"

namespace adm {

  class Document;

  namespace detail {
    using AudioBlockFormatBinauralBase =
        HasParameters<DefaultParameter<Gain>, DefaultParameter<Importance>>;
  }  // namespace detail

  /// @brief Tag for AudioBlockFormatBinaural
  struct AudioBlockFormatBinauralTag {};
  /**
   * @brief Class representation for ADM element audioBlockFormat if
   * audioChannelFormat.typeDefinition == "Binaural"
   *
   * Supported parameters are as follows:
   *
   * \rst
   * +---------------------+------------------------------+----------------------------+
   * | ADM Parameter       | Parameter Type               | Pattern Type               |
   * +=====================+==============================+============================+
   * | audioBlockFormatId  | :class:`AudioBlockFormatId`  | :class:`RequiredParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | rtime               | :type:`Rtime`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | duration            | :type:`Duration`             | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | lstart              | :type:`Lstart`               | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | lduration           | :type:`Lduration`            | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | initializeBlock     | :type:`InitializeBlock`      | :class:`OptionalParameter` |
   * +---------------------+------------------------------+----------------------------+
   * | gain                | :class:`Gain`                | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * | importance          | :type:`Importance`           | :class:`DefaultParameter`  |
   * +---------------------+------------------------------+----------------------------+
   * \endrst
   */
  class AudioBlockFormatBinaural
      : private detail::AudioBlockFormatBinauralBase {
   public:
    typedef AudioBlockFormatBinauralTag tag;
    /// Type that holds the id for this element;
    typedef AudioBlockFormatId id_type;

    template <typename... Parameters>
    explicit AudioBlockFormatBinaural(Parameters... optionalNamedArgs);

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
    /// @brief lstart setter
    ADM_EXPORT void set(Lstart lstart);
    /// @brief Lduration setter
    ADM_EXPORT void set(Lduration lduration);
    /// @brief InitializeBlock setter
    ADM_EXPORT void set(InitializeBlock initializeBlock);

    using detail::AudioBlockFormatBinauralBase::set;

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
    using detail::AudioBlockFormatBinauralBase::get;
    using detail::AudioBlockFormatBinauralBase::has;
    using detail::AudioBlockFormatBinauralBase::isDefault;
    using detail::AudioBlockFormatBinauralBase::unset;

    ADM_EXPORT AudioBlockFormatId
        get(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT Rtime get(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT Lstart get(detail::ParameterTraits<Lstart>::tag) const;
    ADM_EXPORT Lduration get(detail::ParameterTraits<Lduration>::tag) const;
    ADM_EXPORT InitializeBlock
        get(detail::ParameterTraits<InitializeBlock>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioBlockFormatId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Rtime>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Lstart>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Lduration>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<InitializeBlock>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT bool isDefault(detail::ParameterTraits<Rtime>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Rtime>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Lstart>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Lduration>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<InitializeBlock>::tag);

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
    detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
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
