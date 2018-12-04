/// @file audio_channel_format_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioChannelFormatIdValue
  struct AudioChannelFormatIdValueTag {};
  /// @brief NamedType for the AudioChannelFormatIdValue attribute
  using AudioChannelFormatIdValue =
      detail::NamedType<unsigned int, AudioChannelFormatIdValueTag>;

  /// @brief Tag for AudioChannelFormatId
  struct AudioChannelFormatIdTag {};
  /// @brief Representation of an AudioChannelFormatId
  class AudioChannelFormatId {
   public:
    typedef AudioChannelFormatIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioChannelFormatId(Parameters... optionalNamedArgs);

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

    /// @brief Set channel type
    ADM_EXPORT void set(TypeDescriptor channelType);
    /// @brief Set value
    ADM_EXPORT void set(AudioChannelFormatIdValue value);

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
     * Compares the string representation of the AudioChannelFormatId.
     */
    ADM_EXPORT bool operator==(const AudioChannelFormatId& other) const;
    ADM_EXPORT bool operator!=(const AudioChannelFormatId& other) const;
    ADM_EXPORT bool operator<(const AudioChannelFormatId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT TypeDescriptor
        get(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT AudioChannelFormatIdValue
        get(detail::ParameterTraits<AudioChannelFormatIdValue>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioChannelFormatIdValue>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioChannelFormatIdValue>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<TypeDescriptor>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioChannelFormatIdValue>::tag);

    boost::optional<TypeDescriptor> channelType_;
    boost::optional<AudioChannelFormatIdValue> value_;

    static const TypeDescriptor channelTypeDefault_;
    static const AudioChannelFormatIdValue valueDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioChannelFormatId string and convert it into an
  /// object
  ADM_EXPORT AudioChannelFormatId
  parseAudioChannelFormatId(const std::string& id);
  /// @brief Format an AudioChannelFormatId object as string
  ADM_EXPORT std::string formatId(AudioChannelFormatId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioChannelFormatId::AudioChannelFormatId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioChannelFormatId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioChannelFormatId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioChannelFormatId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioChannelFormatId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
