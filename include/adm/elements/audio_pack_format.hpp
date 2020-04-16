/// @file audio_pack_format.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_pack_format_id.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/helper/element_range.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::AudioPackFormatName
  struct AudioPackFormatNameTag {};
  /// @brief NamedType for the audioPackFormatName attribute
  using AudioPackFormatName =
      detail::NamedType<std::string, AudioPackFormatNameTag>;
  /// @brief Tag for NamedType ::AbsoluteDistance
  struct AbsoluteDistanceTag {};
  /// @brief NamedType for the absoluteDistance attribute
  using AbsoluteDistance = detail::NamedType<float, AbsoluteDistanceTag>;

  /// @brief Tag for AudioPackFormat
  struct AudioPackFormatTag {};
  /**
   * @brief Class representation of the audioPackFormat ADM element
   *
   * @headerfile audio_pack_format.hpp \
   * <adm/elements/audio_pack_format.hpp>
   */
  class AudioPackFormat : public std::enable_shared_from_this<AudioPackFormat> {
   public:
    typedef AudioPackFormatTag tag;
    /// Type that holds the id for this element;
    typedef AudioPackFormatId id_type;

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioPackFormat object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioPackFormat> create(
        AudioPackFormatName name, TypeDescriptor channelType,
        Parameters... optionalNamedArgs);

    ADM_EXPORT virtual ~AudioPackFormat() = default;
    /**
     * @brief Copy AudioPackFormat
     *
     * The actual copy constructor is private to ensure that an
     * AudioPackFormat can only be created as a `std::shared_ptr`. This is not a
     * deep copy! All referenced objects will be disconnected.
     */
    ADM_EXPORT std::shared_ptr<AudioPackFormat> copy() const;

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

    /// @brief AudioPackFormatId setter
    ADM_EXPORT void set(AudioPackFormatId id);
    /// @brief AudioPackFormatName setter
    ADM_EXPORT void set(AudioPackFormatName name);
    /// @brief Importance setter
    ADM_EXPORT void set(Importance importance);
    /// @brief AbsoluteDistance setter
    ADM_EXPORT void set(AbsoluteDistance distance);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    /// @brief Add reference to an AudioChannelFormat
    ADM_EXPORT bool addReference(
        std::shared_ptr<AudioChannelFormat> channelFormat);
    /// @brief Add reference to another AudioPackFormat
    ADM_EXPORT bool addReference(std::shared_ptr<AudioPackFormat> packFormat);

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns a set of all references to the ADM elements with the
     * specified type.
     */
    template <typename Element>
    ElementRange<const Element> getReferences() const;

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns a set of all references to the ADM elements with the
     * specified type.
     */
    template <typename Element>
    ElementRange<Element> getReferences();

    /// @brief Remove reference to an AudioChannelFormat
    ADM_EXPORT void removeReference(
        std::shared_ptr<AudioChannelFormat> channelFormat);
    /// @brief Remove reference to an AudioPackFormat
    ADM_EXPORT void removeReference(
        std::shared_ptr<AudioPackFormat> packFormat);

    /**
     * @brief Clear references to Elements template
     *
     * Templated clear method with the ADM parameter type as template
     * argument. Removes all references to the ADM elements with the specified
     * type.
     */
    template <typename Element>
    void clearReferences();

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT std::weak_ptr<Document> getParent() const;

   protected:
    friend class AudioPackFormatAttorney;

    ADM_EXPORT AudioPackFormat(AudioPackFormatName name,
                               TypeDescriptor channelType);
    ADM_EXPORT AudioPackFormat(const AudioPackFormat &) = default;
    ADM_EXPORT AudioPackFormat(AudioPackFormat &&) = default;

    ADM_EXPORT AudioPackFormatId
        get(detail::ParameterTraits<AudioPackFormatId>::tag) const;
    ADM_EXPORT AudioPackFormatName
        get(detail::ParameterTraits<AudioPackFormatName>::tag) const;
    ADM_EXPORT TypeDescriptor
        get(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT Importance get(detail::ParameterTraits<Importance>::tag) const;
    ADM_EXPORT AbsoluteDistance
        get(detail::ParameterTraits<AbsoluteDistance>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioPackFormatId>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioPackFormatName>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<TypeDescriptor>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Importance>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<AbsoluteDistance>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<Importance>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<AbsoluteDistance>::tag);

    bool isAudioPackFormatReferenceCycle(
        std::shared_ptr<AudioPackFormat> destinationPackFormat);

    ElementRange<const AudioChannelFormat> getReferences(
        detail::ParameterTraits<AudioChannelFormat>::tag) const;
    ElementRange<const AudioPackFormat> getReferences(
        detail::ParameterTraits<AudioPackFormat>::tag) const;

    ElementRange<AudioChannelFormat> getReferences(
        detail::ParameterTraits<AudioChannelFormat>::tag);
    ElementRange<AudioPackFormat> getReferences(
        detail::ParameterTraits<AudioPackFormat>::tag);

    ADM_EXPORT void clearReferences(
        detail::ParameterTraits<AudioChannelFormat>::tag);
    ADM_EXPORT void clearReferences(
        detail::ParameterTraits<AudioPackFormat>::tag);

    ADM_EXPORT void disconnectReferences();

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

   private:
    std::weak_ptr<Document> parent_;
    AudioPackFormatName name_;
    AudioPackFormatId id_;
    TypeDescriptor typeDescriptor_;
    boost::optional<Importance> importance_;
    boost::optional<AbsoluteDistance> absoluteDistance_;
    std::vector<std::shared_ptr<AudioChannelFormat>> audioChannelFormats_;
    std::vector<std::shared_ptr<AudioPackFormat>> audioPackFormats_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioPackFormat> AudioPackFormat::create(
      AudioPackFormatName name, TypeDescriptor channelType,
      Parameters... optionalNamedArgs) {
    if (channelType == adm::TypeDefinition::HOA) {
      throw std::invalid_argument(
          "For AudioPackFormat of type HOA use AudioPackFormatHoa::create() instead.");
    } else {

      std::shared_ptr<AudioPackFormat> pack(
          new AudioPackFormat(name, channelType));
      detail::setNamedOptionHelper(
          pack, std::forward<Parameters>(optionalNamedArgs)...);
      return pack;
    }
  }

  template <typename Parameter>
  Parameter AudioPackFormat::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioPackFormat::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioPackFormat::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioPackFormat::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename Element>
  ElementRange<const Element> AudioPackFormat::getReferences() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  template <typename Element>
  ElementRange<Element> AudioPackFormat::getReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  inline ElementRange<const AudioChannelFormat> AudioPackFormat::getReferences(
      detail::ParameterTraits<AudioChannelFormat>::tag) const {
    return detail::makeElementRange<AudioChannelFormat>(audioChannelFormats_);
  }

  inline ElementRange<const AudioPackFormat> AudioPackFormat::getReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) const {
    return detail::makeElementRange<AudioPackFormat>(audioPackFormats_);
  }

  inline ElementRange<AudioChannelFormat> AudioPackFormat::getReferences(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    return detail::makeElementRange<AudioChannelFormat>(audioChannelFormats_);
  }

  inline ElementRange<AudioPackFormat> AudioPackFormat::getReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return detail::makeElementRange<AudioPackFormat>(audioPackFormats_);
  }

  template <typename Element>
  void AudioPackFormat::clearReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    clearReferences(Tag());
  }

}  // namespace adm
