/// @file audio_track_format.hpp
#pragma once

#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_format_id.hpp"
#include "adm/elements/format_descriptor.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include <boost/optional.hpp>
#include <memory>

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::AudioTrackFormatName
  struct AudioTrackFormatNameTag {};
  /// @brief NamedType for the audioTrackFormatName attribute
  using AudioTrackFormatName =
      detail::NamedType<std::string, AudioTrackFormatNameTag>;

  /// @brief Tag for AudioTrackFormat
  struct AudioTrackFormatTag {};
  /**
   * @brief Class representation of the audioTrackFormat ADM element
   *
   * @headerfile audio_track_format.hpp <adm/elements/audio_track_format.hpp>
   */
  class AudioTrackFormat
      : public std::enable_shared_from_this<AudioTrackFormat> {
   public:
    typedef AudioTrackFormatTag tag;
    /// Type that holds the id for this element;
    typedef AudioTrackFormatId id_type;

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
      sync_with_stream_format =
          0x1,  ///< commit all changes to AudioStreamFormat
    };

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioTrackFormat object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioTrackFormat> create(
        AudioTrackFormatName name, FormatDescriptor format,
        Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioTrackFormat
     *
     * The actual copy constructor is private to ensure that an
     * AudioTrackFormat can only be created as a `std::shared_ptr`. This is not
     * deep copy! All referenced objects will be disconnected.
     *

     */
    ADM_EXPORT std::shared_ptr<AudioTrackFormat> copy() const;

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

    /// @brief AudioTrackFormatId setter
    ADM_EXPORT void set(AudioTrackFormatId id);
    /// @brief AudioTrackFormatName setter
    ADM_EXPORT void set(AudioTrackFormatName name);

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
     * @brief Set reference to an AudioStreamFormat
     *
     * A pending unresolved reference will be removed.
     *
     * @param streamFormat which should be referenced.
     * @param sync controls if *this* will be automatically added to
     *`streamFormat` as a reference as well.
     */
    ADM_EXPORT void setReference(
        std::shared_ptr<AudioStreamFormat> streamFormat,
        ReferenceSyncOption sync =
            ReferenceSyncOption::sync_with_stream_format);

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
     * @brief Remove reference to an Element template
     *
     * Templated remove method with the ADM parameter type as template
     * argument. Removes the reference to the ADM elements with the specified
     * type.
     *
     * @param sync controls if any reference to *this* will be automatically
     *removed from any referenced `streamFormat` as well, **if `Element` is
     *`AudioStreamFormat`**. For other `Element` types (which do not exist),
     *this option is ignored does nothing
     */
    template <typename Element>
    void removeReference(ReferenceSyncOption sync =
                             ReferenceSyncOption::sync_with_stream_format);

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT std::weak_ptr<Document> getParent() const;

   private:
    friend class AudioTrackFormatAttorney;

    ADM_EXPORT AudioTrackFormat(AudioTrackFormatName name,
                                FormatDescriptor channelType);
    ADM_EXPORT AudioTrackFormat(const AudioTrackFormat &) = default;
    ADM_EXPORT AudioTrackFormat(AudioTrackFormat &&) = default;

    ADM_EXPORT AudioTrackFormatId
        get(detail::ParameterTraits<AudioTrackFormatId>::tag) const;
    ADM_EXPORT AudioTrackFormatName
        get(detail::ParameterTraits<AudioTrackFormatName>::tag) const;
    ADM_EXPORT FormatDescriptor
        get(detail::ParameterTraits<FormatDescriptor>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioTrackFormatId>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioTrackFormatName>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<FormatDescriptor>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT std::shared_ptr<const AudioStreamFormat> getReference(
        detail::ParameterTraits<AudioStreamFormat>::tag) const;
    ADM_EXPORT std::shared_ptr<AudioStreamFormat> getReference(
        detail::ParameterTraits<AudioStreamFormat>::tag);
    ADM_EXPORT void removeReference(
        detail::ParameterTraits<AudioStreamFormat>::tag,
        ReferenceSyncOption sync);

    ADM_EXPORT void removeReference(
        detail::ParameterTraits<AudioStreamFormatId>::tag);

    ADM_EXPORT void disconnectReferences();

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;
    AudioTrackFormatName name_;
    AudioTrackFormatId id_;
    FormatDescriptor format_;
    std::shared_ptr<AudioStreamFormat> audioStreamFormat_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioTrackFormat> AudioTrackFormat::create(
      AudioTrackFormatName name, FormatDescriptor format,
      Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioTrackFormat> trackFormat(
        new AudioTrackFormat(name, format));
    detail::setNamedOptionHelper(
        trackFormat, std::forward<Parameters>(optionalNamedArgs)...);

    return trackFormat;
  }
  template <typename Parameter>
  Parameter AudioTrackFormat::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioTrackFormat::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioTrackFormat::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioTrackFormat::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename Element>
  std::shared_ptr<const Element> AudioTrackFormat::getReference() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReference(Tag());
  }

  template <typename Element>
  std::shared_ptr<Element> AudioTrackFormat::getReference() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReference(Tag());
  }

  template <typename Element>
  void AudioTrackFormat::removeReference(ReferenceSyncOption sync) {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    removeReference(Tag(), sync);
  }

}  // namespace adm
