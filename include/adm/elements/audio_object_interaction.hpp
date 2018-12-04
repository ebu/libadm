/// @file audio_object_interaction.hpp
#pragma once

#include "adm/elements/gain_interaction_range.hpp"
#include "adm/elements/position_interaction_range.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/named_option_helper.hpp"

#include <boost/optional.hpp>
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::OnOffInteract
  struct OnOffInteractTag {};
  /// @brief NamedType for channelLockFlag parameter
  using OnOffInteract = detail::NamedType<bool, OnOffInteractTag>;

  /// @brief Tag for NamedType ::GainInteract
  struct GainInteractTag {};
  /// @brief NamedType for channelLockFlag parameter
  using GainInteract = detail::NamedType<bool, GainInteractTag>;

  /// @brief Tag for NamedType ::PositionInteract
  struct PositionInteractTag {};
  /// @brief NamedType for channelLockFlag parameter
  using PositionInteract = detail::NamedType<bool, PositionInteractTag>;

  /// @brief Tag for AudioObjectInteraction class
  struct AudioObjectInteractionTag {};
  /**
   * @brief ADM parameter class to specify a channel lock
   */
  class AudioObjectInteraction {
   public:
    typedef AudioObjectInteractionTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioObjectInteraction(OnOffInteract onOffInteract,
                           Parameters... optionalNamedArgs);

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

    /// @brief OnOffInteract setter
    ADM_EXPORT void set(OnOffInteract);
    /// @brief GainInteract setter
    ADM_EXPORT void set(GainInteract);
    /// @brief PositionInteract setter
    ADM_EXPORT void set(PositionInteract);
    /// @brief GainInteractionRange setter
    ADM_EXPORT void set(GainInteractionRange);
    /// @brief PositionInteractionRange setter
    ADM_EXPORT void set(PositionInteractionRange);

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
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

   private:
    ADM_EXPORT OnOffInteract
        get(detail::ParameterTraits<OnOffInteract>::tag) const;
    ADM_EXPORT GainInteract
        get(detail::ParameterTraits<GainInteract>::tag) const;
    ADM_EXPORT PositionInteract
        get(detail::ParameterTraits<PositionInteract>::tag) const;
    ADM_EXPORT GainInteractionRange
        get(detail::ParameterTraits<GainInteractionRange>::tag) const;
    ADM_EXPORT PositionInteractionRange
        get(detail::ParameterTraits<PositionInteractionRange>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<OnOffInteract>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<GainInteract>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<PositionInteract>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<GainInteractionRange>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<PositionInteractionRange>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<GainInteract>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<PositionInteract>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<GainInteractionRange>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<PositionInteractionRange>::tag);

    OnOffInteract onOffInteract_;
    boost::optional<GainInteract> gainInteract_;
    boost::optional<PositionInteract> positionInteract_;
    boost::optional<GainInteractionRange> gainInteractionRange_;
    boost::optional<PositionInteractionRange> positionInteractionRange_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  AudioObjectInteraction::AudioObjectInteraction(
      OnOffInteract onOffInteract, Parameters... optionalNamedArgs)
      : onOffInteract_(onOffInteract) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioObjectInteraction::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioObjectInteraction::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioObjectInteraction::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioObjectInteraction::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
