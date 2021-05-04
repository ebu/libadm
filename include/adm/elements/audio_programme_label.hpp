#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioProgrammeLabelValue
  struct AudioProgrammeLabelValueTag {};
  /// @brief NamedType for the AudioProgrammeLabelValue attribute
  using AudioProgrammeLabelValue =
      detail::NamedType<std::string, AudioProgrammeLabelValueTag>;

  /// @brief Tag for AudioProgrammeLabelLanguage
  struct AudioProgrammeLabelLanguageTag {};
  /// @brief NamedType for AudioProgrammeLabelLanguage attribute
  using AudioProgrammeLabelLanguage =
      detail::NamedType<std::string, AudioProgrammeLabelLanguageTag>;

  /// @brief Tag for AudioProgrammeLabelId
  struct AudioProgrammeLabelTag {};
  /// @brief Representation of an AudioProgrammeLabel
  class AudioProgrammeLabel {
   public:
    typedef AudioProgrammeLabelTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioProgrammeLabel(Parameters... optionalNamedArgs);

    /**
     * @brief ADM parameter getter template
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template.
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

    /// @brief set value
    ADM_EXPORT void set(AudioProgrammeLabelValue value);

    /// @brief set language
    ADM_EXPORT void set(AudioProgrammeLabelLanguage language);

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
     * Compares the string representation of the AudioProgrammeLabel.
     */
    ADM_EXPORT bool operator==(const AudioProgrammeLabel& other) const;
    ADM_EXPORT bool operator!=(const AudioProgrammeLabel& other) const;
    ADM_EXPORT bool operator<(const AudioProgrammeLabel& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT AudioProgrammeLabelValue
        get(detail::ParameterTraits<AudioProgrammeLabelValue>::tag) const;
    ADM_EXPORT AudioProgrammeLabelLanguage
        get(detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag) const;

    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioProgrammeLabelValue>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioProgrammeLabelValue>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioProgrammeLabelValue>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioProgrammeLabelLanguage>::tag);

    boost::optional<std::string> value_;
    boost::optional<std::string> language_;

    static const AudioProgrammeLabelValue valueDefault_;
    static const AudioProgrammeLabelLanguage languageDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioProgrammeLabel string and convert it into an object
  ADM_EXPORT AudioProgrammeLabel
  parseAudioProgrammeLabel(const std::string& id);
  /// @brief Format an AudioProgrammeLabel object as string
  ADM_EXPORT std::string format(AudioProgrammeLabel id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioProgrammeLabel::AudioProgrammeLabel(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioProgrammeLabel::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioProgrammeLabel::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioProgrammeLabel::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioProgrammeLabel::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }
}  // namespace adm