/// @file audio_track_uid.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/audio_track_uid_id.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  class Document;

  /// @brief Tag for SampleRate
  struct SampleRateTag {};
  /// @brief NamedType for the sampleRate element
  using SampleRate = detail::NamedType<unsigned int, SampleRateTag>;
  /// @brief Tag for BitDepth
  struct BitDepthTag {};
  /// @brief NamedType for the bitDepth element
  using BitDepth = detail::NamedType<unsigned int, BitDepthTag>;

  /// @brief Tag for AudioTrackUid
  struct AudioTrackUidTag {};
  /**
   * @brief Class representation of the audioTrackUID ADM element
   *
   * @warning This class has unsupported parameters:
   *   - audioMXFLookUp
   *
   * @headerfile audio_track_uid.hpp \
   * <adm/elements/audio_track_uid.hpp>
   */
  class AudioTrackUid : public std::enable_shared_from_this<AudioTrackUid> {
   public:
    typedef AudioTrackUidTag tag;

    /// Type that holds the id for this element;
    typedef AudioTrackUidId id_type;

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioTrackUid object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioTrackUid> create(
        Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioTrackUid
     *
     * The actual copy constructor is private to ensure that an AudioTrackUid
     * can only be created as a `std::shared_ptr`. This is not a deep copy! All
     * referenced objects will be disconnected.
     */
    ADM_EXPORT std::shared_ptr<AudioTrackUid> copy() const;

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

    /// @brief AudioTrackUidId setter
    ADM_EXPORT void set(AudioTrackUidId id);
    /// @brief SampleRate setter
    ADM_EXPORT void set(SampleRate sampleRate);
    /// @brief BitDepth setter
    ADM_EXPORT void set(BitDepth bitDepth);

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
     * @brief Set reference to an AudioTrackFormat
     *
     * A pending unresolved reference will be removed.
     */
    ADM_EXPORT void setReference(std::shared_ptr<AudioTrackFormat> trackFormat);
    /**
     * @brief Set reference to an AudioPackFormat
     *
     * A pending unresolved reference will be removed.
     */
    ADM_EXPORT void setReference(std::shared_ptr<AudioPackFormat> packFormat);

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
     */
    template <typename Element>
    void removeReference();

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT std::weak_ptr<Document> getParent() const;

   private:
    friend class AudioTrackUidAttorney;

    ADM_EXPORT AudioTrackUid();
    ADM_EXPORT AudioTrackUid(const AudioTrackUid &) = default;
    ADM_EXPORT AudioTrackUid(AudioTrackUid &&) = default;

    ADM_EXPORT AudioTrackUidId
        get(detail::ParameterTraits<AudioTrackUidId>::tag) const;
    ADM_EXPORT BitDepth get(detail::ParameterTraits<BitDepth>::tag) const;
    ADM_EXPORT SampleRate get(detail::ParameterTraits<SampleRate>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioTrackUidId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<BitDepth>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<SampleRate>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT void unset(detail::ParameterTraits<BitDepth>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<SampleRate>::tag);

    ADM_EXPORT std::shared_ptr<const AudioTrackFormat> getReference(
        detail::ParameterTraits<AudioTrackFormat>::tag) const;
    ADM_EXPORT std::shared_ptr<const AudioPackFormat> getReference(
        detail::ParameterTraits<AudioPackFormat>::tag) const;
    ADM_EXPORT std::shared_ptr<AudioTrackFormat> getReference(
        detail::ParameterTraits<AudioTrackFormat>::tag);
    ADM_EXPORT std::shared_ptr<AudioPackFormat> getReference(
        detail::ParameterTraits<AudioPackFormat>::tag);

    ADM_EXPORT void removeReference(
        detail::ParameterTraits<AudioTrackFormat>::tag);
    ADM_EXPORT void removeReference(
        detail::ParameterTraits<AudioPackFormat>::tag);

    ADM_EXPORT void disconnectReferences();

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;
    AudioTrackUidId id_;
    boost::optional<BitDepth> bitDepth_;
    boost::optional<SampleRate> sampleRate_;

    std::shared_ptr<AudioTrackFormat> audioTrackFormat_;
    std::shared_ptr<AudioPackFormat> audioPackFormat_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioTrackUid> AudioTrackUid::create(
      Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioTrackUid> trackUid(new AudioTrackUid());
    detail::setNamedOptionHelper(
        trackUid, std::forward<Parameters>(optionalNamedArgs)...);

    return trackUid;
  }

  template <typename Parameter>
  Parameter AudioTrackUid::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioTrackUid::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioTrackUid::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioTrackUid::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename Element>
  std::shared_ptr<Element> AudioTrackUid::getReference() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReference(Tag());
  }

  template <typename Element>
  std::shared_ptr<const Element> AudioTrackUid::getReference() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReference(Tag());
  }

  template <typename Element>
  void AudioTrackUid::removeReference() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    removeReference(Tag());
  }

}  // namespace adm
