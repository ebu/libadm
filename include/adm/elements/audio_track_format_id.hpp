/// @file audio_pack_format_id.hpp
#pragma once

#include <boost/optional.hpp>
#include <string>
#include "adm/elements/type_descriptor.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for AudioTrackFormatIdValue
  struct AudioTrackFormatIdValueTag {};
  /// @brief NamedType for the AudioTrackFormatIdValue attribute
  using AudioTrackFormatIdValue =
      detail::NamedType<unsigned int, AudioTrackFormatIdValueTag>;

  /// @brief Tag for AudioTrackFormatIdCounter
  struct AudioTrackFormatIdCounterTag {};
  /// @brief NamedType for the AudioTrackFormatIdCounter attribute
  using AudioTrackFormatIdCounter =
      detail::NamedType<unsigned int, AudioTrackFormatIdCounterTag>;

  /// @brief Tag for AudioTrackFormatId
  struct AudioTrackFormatIdTag {};
  /// @brief Representation of an AudioTrackFormatId
  class AudioTrackFormatId {
   public:
    typedef AudioTrackFormatIdTag tag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    AudioTrackFormatId(Parameters... optionalNamedArgs);

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
    ADM_EXPORT void set(AudioTrackFormatIdValue value);
    /// @brief Set counter
    ADM_EXPORT void set(AudioTrackFormatIdCounter counter);

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
     * Compares the string representation of the AudioTrackFormatId.
     */
    ADM_EXPORT bool operator==(const AudioTrackFormatId& other) const;
    ADM_EXPORT bool operator!=(const AudioTrackFormatId& other) const;
    ADM_EXPORT bool operator<(const AudioTrackFormatId& other) const;
    ///@}

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    ADM_EXPORT TypeDescriptor
        get(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT AudioTrackFormatIdValue
        get(detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const;
    ADM_EXPORT AudioTrackFormatIdCounter
        get(detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const;

    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioTrackFormatIdValue>::tag) const;
    ADM_EXPORT bool isDefault(
        detail::ParameterTraits<AudioTrackFormatIdCounter>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<TypeDescriptor>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioTrackFormatIdValue>::tag);
    ADM_EXPORT void unset(
        detail::ParameterTraits<AudioTrackFormatIdCounter>::tag);

    boost::optional<TypeDescriptor> channelType_;
    boost::optional<AudioTrackFormatIdValue> value_;
    boost::optional<AudioTrackFormatIdCounter> counter_;

    static const TypeDescriptor channelTypeDefault_;
    static const AudioTrackFormatIdValue valueDefault_;
    static const AudioTrackFormatIdCounter counterDefault_;
  };

  // ---- Free functions ---- //

  /// @brief Parse an AudioTrackFormatId string and convert it into an object
  ADM_EXPORT AudioTrackFormatId parseAudioTrackFormatId(const std::string& id);
  /// @brief Format an AudioTrackFormatId object as string
  ADM_EXPORT std::string formatId(AudioTrackFormatId id);

  // ---- Implementation ---- //
  template <typename... Parameters>
  AudioTrackFormatId::AudioTrackFormatId(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  };

  template <typename Parameter>
  Parameter AudioTrackFormatId::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioTrackFormatId::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioTrackFormatId::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioTrackFormatId::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

}  // namespace adm
