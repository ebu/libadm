/// @file audio_object_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioObjectIdValue
  struct AudioObjectIdValueTag {};
  /// @brief NamedType for the AudioObjectIdValue attribute
  using AudioObjectIdValue =
      detail::NamedType<unsigned int, AudioObjectIdValueTag>;

  /// @brief Tag for AudioObjectId
  struct AudioObjectIdTag {};
  /// @brief Representation of an AudioObjectId
  class AudioObjectId {
   public:
    typedef AudioObjectIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioObjectId(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(AudioObjectIdValue value);

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
     * Compares the string representation of the AudioObjectId.
     */
    ADM_EXPORT bool operator==(const AudioObjectId& other) const;
    ADM_EXPORT bool operator!=(const AudioObjectId& other) const;
    ADM_EXPORT bool operator<(const AudioObjectId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT AudioObjectIdValue
        get(detail::ParameterTraits<AudioObjectIdValue>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioObjectIdValue>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioObjectIdValue>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<AudioObjectIdValue>::tag);

    boost::optional<AudioObjectIdValue> value_;

    static const AudioObjectIdValue valueDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioObjectId string and convert it into an object
  ADM_EXPORT AudioObjectId parseAudioObjectId(const std::string& id);
  /// @brief Format an AudioObjectId object as string
  ADM_EXPORT std::string formatId(AudioObjectId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioObjectId::AudioObjectId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioObjectId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioObjectId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioObjectId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioObjectId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
