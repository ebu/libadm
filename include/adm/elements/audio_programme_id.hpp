/// @file audio_programme_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioProgrammeIdValue
  struct AudioProgrammeIdValueTag {};
  /// @brief NamedType for the AudioProgrammeIdValue attribute
  using AudioProgrammeIdValue =
      detail::NamedType<unsigned int, AudioProgrammeIdValueTag>;

  /// @brief Tag for AudioProgrammeId
  struct AudioProgrammeIdTag {};
  /// @brief Representation of an AudioProgrammeId
  class AudioProgrammeId {
   public:
    typedef AudioProgrammeIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioProgrammeId(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(AudioProgrammeIdValue value);

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
     * Compares the string representation of the AudioProgrammeId.
     */
    ADM_EXPORT bool operator==(const AudioProgrammeId& other) const;
    ADM_EXPORT bool operator!=(const AudioProgrammeId& other) const;
    ADM_EXPORT bool operator<(const AudioProgrammeId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT AudioProgrammeIdValue
        get(detail::ParameterTraits<AudioProgrammeIdValue>::tag) const;

    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioProgrammeIdValue>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioProgrammeIdValue>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<AudioProgrammeIdValue>::tag);

    boost::optional<AudioProgrammeIdValue> value_;

    static const AudioProgrammeIdValue valueDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioProgrammeId string and convert it into an object
  ADM_EXPORT AudioProgrammeId parseAudioProgrammeId(const std::string& id);
  /// @brief Format an AudioProgrammeId object as string
  ADM_EXPORT std::string formatId(AudioProgrammeId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioProgrammeId::AudioProgrammeId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioProgrammeId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioProgrammeId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioProgrammeId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioProgrammeId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
