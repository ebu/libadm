/// @file audio_stream_format.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_stream_format_id.hpp"
#include "adm/elements/format_descriptor.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/helper/element_range.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::AudioStreamFormatName
  struct AudioStreamFormatNameTag {};
  /// @brief NamedType for the audioStreamFormatName attribute
  using AudioStreamFormatName =
      detail::NamedType<std::string, AudioStreamFormatNameTag>;

  /// @brief Tag for AudioStreamFormat
  struct AudioStreamFormatTag {};
  /**
   * @brief Class representation of the audioStreamFormat ADM element
   *
   * ### Notes on AudioTrackFormat std::weak_ptr usage
   *
   * Please note that the references to `AudioTrackFormat`s are represented
   * using `std::weak_ptr`.
   * Consequently, the method to retrieve those references is
   * `AudioStreamFormat::getAudioTrackFormatReferences()`
   * to make the difference to other `getReferences<Element>()` explicit.
   *
   * The reason for using `std::weak_ptr` in the first place is because
   * there's a cyclic reference between `AudioStreamFormat` and
   * `AudioTrackFormat`.
   * As it turns out, using the `std::weak_ptr` for `AudioTrackFormat` is much
   * more convenient than the other way around. This is mostly due to the fact
   * that otherwise there's no direct connection from e.g. `AudioTrackUid` to
   * `AudioStreamFormat`, which would cause the latter to be removed if we
   * would use the `std::weak_ptr` connection the other way around.
   *
   * Furthermore, from the code we wrote so far, we can see that the
   * `AudioTrackFormat -> AudioStreamFormat`
   * connection is used quite often, while the `AudioStreamFormat ->
   * AudioTrackFormat` has been virtually unused so far.
   * Thus it was the obvious choice to make the first use case more convenient
   * for users of the library.
   *
   *
   * @headerfile audio_stream_format.hpp <adm/elements/audio_stream_format.hpp>
   */
  class AudioStreamFormat
      : public std::enable_shared_from_this<AudioStreamFormat> {
   public:
    typedef AudioStreamFormatTag tag;
    /// Type that holds the id for this element;
    typedef AudioStreamFormatId id_type;

    /**
     * @brief Options to change AudioStreamFormat/AudioTrackFormat reference
     * sync behaviour
     *
     * These options are to be used with `setReference` and `removeReference`
     * and controls if *this* AudioTrackFormat is automatically added/removed
     * as a reference to the AudioStreamFormat if it is referenced by *this*.
     *
     * The default (and only) behaviour is to keep AudioTrackFormat and
     * AudioStreamFormat in sync.
     *
     * Future applications might want to use a more relaxed policy by
     * providing (and implementing, and testing!) other options.
     */
    enum class ReferenceSyncOption : unsigned {
      sync_with_track_format =
          0x1,  ///< commit all changes to AudioStreamFormat
    };

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioStreamFormat object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioStreamFormat> create(
        AudioStreamFormatName name, FormatDescriptor format,
        Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioStreamFormat
     *
     * The actual copy constructor is private to ensure that an
     * AudioStreamFormat can only be created as a `std::shared_ptr`. This is not
     * deep copy! All referenced objects will be disconnected.
     *

     */
    ADM_EXPORT std::shared_ptr<AudioStreamFormat> copy() const;

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

    /// @brief AudioStreamFormatId setter
    ADM_EXPORT void set(AudioStreamFormatId id);
    /// @brief AudioStreamFormatName setter
    ADM_EXPORT void set(AudioStreamFormatName name);

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
     * @brief Set reference to an AudioChannelFormat
     *
     * A pending unresolved reference will be removed.
     */
    ADM_EXPORT void setReference(
        std::shared_ptr<AudioChannelFormat> channelFormat);
    /**
     * @brief Set reference to an AudioPackFormat
     *
     * A pending unresolved reference will be removed.
     */
    ADM_EXPORT void setReference(std::shared_ptr<AudioPackFormat> packFormat);
    /**
     * @brief Add reference to an AudioTrackFormat
     *
     * @param trackFormat which should be added.
     * @param sync controls if *this* will be automatically added to
     *`trackFormat` as a reference as well.
     */
    ADM_EXPORT bool addReference(
        std::weak_ptr<AudioTrackFormat> trackFormat,
        ReferenceSyncOption sync = ReferenceSyncOption::sync_with_track_format);

    /**
     * @brief Get reference to ADM element template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns the reference to the ADM element with the
     * specified type. If it is not set a nullptr will be returned.
     */
    template <typename Element>
    std::shared_ptr<const Element> getReference() const;

    /**
     * @brief Get reference to ADM element template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns the reference to the ADM element with the
     * specified type. If it is not set a nullptr will be returned.
     */
    template <typename Element>
    std::shared_ptr<Element> getReference();

    /**
     * @brief Get references to AudioTrackFormats
     *
     * Returns a range of std::weak_ptr<const AudioTrackFormats>.
     *
     * @note
     * Please read the class documentation of adm::AudioStreamFormat
     * for the rationale behind using std::weak_ptr.
     */
    ADM_EXPORT ElementWeakRange<const AudioTrackFormat>
    getAudioTrackFormatReferences() const;

    /**
     * @brief Get references to AudioTrackFormats
     *
     * Returns a range of std::weak_ptr<AudioTrackFormats>.
     *
     * @note
     * Please read the class documentation of adm::AudioStreamFormat
     * for the rationale behind using std::weak_ptr.
     */
    ADM_EXPORT ElementWeakRange<AudioTrackFormat>
    getAudioTrackFormatReferences();

    /**
     * @brief Remove reference to an AudioTrackFormat
     *
     * @param trackFormat reference which should be removed.
     * @param sync controls if *this* will be automatically added to
     *`trackFormat` as a reference as well.
     */
    ADM_EXPORT void removeReference(
        std::weak_ptr<AudioTrackFormat> trackFormat,
        ReferenceSyncOption sync = ReferenceSyncOption::sync_with_track_format);

    /**
     * @brief Remove reference to an Element template
     *
     * Templated remove method with the ADM parameter type as template
     * argument. Removes the reference to the ADM elements with the specified
     * type.
     */
    template <typename Element>
    void removeReference();

    /**
     * @brief Clear references to Elements template
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

   private:
    friend class AudioStreamFormatAttorney;

    ADM_EXPORT AudioStreamFormat(AudioStreamFormatName name,
                                 FormatDescriptor format);
    ADM_EXPORT AudioStreamFormat(const AudioStreamFormat &) = default;
    ADM_EXPORT AudioStreamFormat(AudioStreamFormat &&) = default;

    ADM_EXPORT AudioStreamFormatId
        get(detail::ParameterTraits<AudioStreamFormatId>::tag) const;
    ADM_EXPORT AudioStreamFormatName
        get(detail::ParameterTraits<AudioStreamFormatName>::tag) const;
    ADM_EXPORT FormatDescriptor
        get(detail::ParameterTraits<FormatDescriptor>::tag) const;

    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioStreamFormatId>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioStreamFormatName>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FormatDescriptor>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT std::shared_ptr<AudioChannelFormat> getReference(
        detail::ParameterTraits<AudioChannelFormat>::tag);
    ADM_EXPORT std::shared_ptr<AudioPackFormat> getReference(
        detail::ParameterTraits<AudioPackFormat>::tag);
    ADM_EXPORT std::shared_ptr<const AudioChannelFormat> getReference(
        detail::ParameterTraits<AudioChannelFormat>::tag) const;
    ADM_EXPORT std::shared_ptr<const AudioPackFormat> getReference(
        detail::ParameterTraits<AudioPackFormat>::tag) const;

    ADM_EXPORT void removeReference(
        detail::ParameterTraits<AudioChannelFormat>::tag);
    ADM_EXPORT void removeReference(
        detail::ParameterTraits<AudioPackFormat>::tag);

    ADM_EXPORT void clearReferences(
        detail::ParameterTraits<AudioTrackFormat>::tag);

    ADM_EXPORT void disconnectReferences();

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;
    AudioStreamFormatName name_;
    AudioStreamFormatId id_;
    FormatDescriptor format_;

    std::shared_ptr<AudioChannelFormat> audioChannelFormat_;
    std::shared_ptr<AudioPackFormat> audioPackFormat_;
    std::vector<std::weak_ptr<AudioTrackFormat>> audioTrackFormats_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioStreamFormat> AudioStreamFormat::create(
      AudioStreamFormatName name, FormatDescriptor format,
      Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioStreamFormat> streamFormat(
        new AudioStreamFormat(name, format));
    detail::setNamedOptionHelper(
        streamFormat, std::forward<Parameters>(optionalNamedArgs)...);

    return streamFormat;
  }

  template <typename Parameter>
  Parameter AudioStreamFormat::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioStreamFormat::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioStreamFormat::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioStreamFormat::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename Element>
  std::shared_ptr<const Element> AudioStreamFormat::getReference() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReference(Tag());
  }

  template <typename Element>
  std::shared_ptr<Element> AudioStreamFormat::getReference() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReference(Tag());
  }

  template <typename Element>
  void AudioStreamFormat::removeReference() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    removeReference(Tag());
  }

  template <typename Element>
  void AudioStreamFormat::clearReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    clearReferences(Tag());
  }

}  // namespace adm
