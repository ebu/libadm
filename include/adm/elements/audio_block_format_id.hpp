/// @file audio_block_format_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioBlockFormatIdValue
  struct AudioBlockFormatIdValueTag {};
  /// @brief NamedType for the AudioBlockFormatIdValue attribute
  using AudioBlockFormatIdValue =
      detail::NamedType<unsigned int, AudioBlockFormatIdValueTag>;

  /// @brief Tag for AudioBlockFormatIdCounter
  struct AudioBlockFormatIdCounterTag {};
  /// @brief NamedType for the AudioBlockFormatIdCounter attribute
  using AudioBlockFormatIdCounter =
      detail::NamedType<unsigned int, AudioBlockFormatIdCounterTag>;

  /// @brief Tag for AudioBlockFormatId
  struct AudioBlockFormatIdTag {};
  /// @brief Representation of an AudioBlockFormatId
  class AudioBlockFormatId {
   public:
    typedef AudioBlockFormatIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioBlockFormatId(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(AudioBlockFormatIdValue value);
    /// @brief Set counter
    ADM_EXPORT void set(AudioBlockFormatIdCounter counter);

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
     * Compares the string representation of the AudioBlockFormatId.
     */
    ADM_EXPORT bool operator==(const AudioBlockFormatId& other) const;
    ADM_EXPORT bool operator!=(const AudioBlockFormatId& other) const;
    ADM_EXPORT bool operator<(const AudioBlockFormatId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT TypeDescriptor
        get(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT AudioBlockFormatIdValue
        get(detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const;
    ADM_EXPORT AudioBlockFormatIdCounter
        get(detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<TypeDescriptor>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioBlockFormatIdValue>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioBlockFormatIdCounter>::tag);

    boost::optional<TypeDescriptor> channelType_;
    boost::optional<AudioBlockFormatIdValue> value_;
    boost::optional<AudioBlockFormatIdCounter> counter_;

    static const TypeDescriptor channelTypeDefault_;
    static const AudioBlockFormatIdValue valueDefault_;
    static const AudioBlockFormatIdCounter counterDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioBlockFormatId string and convert it into an object
  ADM_EXPORT AudioBlockFormatId parseAudioBlockFormatId(const std::string& id);
  /// @brief Format an AudioBlockFormatId object as string
  ADM_EXPORT std::string formatId(AudioBlockFormatId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioBlockFormatId::AudioBlockFormatId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioBlockFormatId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioBlockFormatId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioBlockFormatId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
