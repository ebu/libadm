/// @file audio_content_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioContentIdValue
  struct AudioContentIdValueTag {};
  /// @brief NamedType for the AudioContentIdValue attribute
  using AudioContentIdValue =
      detail::NamedType<unsigned int, AudioContentIdValueTag>;

  /// @brief Tag for AudioContentId
  struct AudioContentIdTag {};
  /// @brief Representation of an AudioContentId
  class AudioContentId {
   public:
    typedef AudioContentIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioContentId(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(AudioContentIdValue value);

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
     * Compares the string representation of the AudioContentId.
     */
    ADM_EXPORT bool operator==(const AudioContentId& other) const;
    ADM_EXPORT bool operator!=(const AudioContentId& other) const;
    ADM_EXPORT bool operator<(const AudioContentId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT AudioContentIdValue
        get(detail::ParameterTraits<AudioContentIdValue>::tag) const;

    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioContentIdValue>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioContentIdValue>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<AudioContentIdValue>::tag);

    boost::optional<AudioContentIdValue> value_;

    static const AudioContentIdValue valueDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioContentId string and convert it into an object
  ADM_EXPORT AudioContentId parseAudioContentId(const std::string& id);
  /// @brief Format an AudioContentId object as string
  ADM_EXPORT std::string formatId(AudioContentId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioContentId::AudioContentId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioContentId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioContentId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioContentId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioContentId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
