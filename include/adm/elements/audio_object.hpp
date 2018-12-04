/// @file audio_object.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_object_id.hpp"
#include "adm/elements/audio_object_interaction.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_uid.hpp"
#include "adm/elements/dialogue.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/helper/element_range.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::AudioObjectName
  struct AudioObjectNameTag {};
  /// @brief NamedType for the audioObjectName attribute
  using AudioObjectName = detail::NamedType<std::string, AudioObjectNameTag>;
  /// @brief Tag for NamedType ::Interact
  struct InteractTag {};
  /// @brief NamedType for the interact attribute
  using Interact = detail::NamedType<bool, InteractTag>;
  /// @brief Tag for NamedType ::DisableDucking
  struct DisableDuckingTag {};
  /// @brief NamedType for the disableDucking attribute
  using DisableDucking = detail::NamedType<bool, DisableDuckingTag>;

  /// @brief Tag for AudioObject
  struct AudioObjectTag {};
  /**
   * @brief Class representation of the audioObject ADM element
   *
   * @headerfile audio_object.hpp <adm/elements/audio_object.hpp>
   */
  class AudioObject : public std::enable_shared_from_this<AudioObject> {
   public:
    typedef AudioObjectTag tag;
    /// Type that holds the id for this element;
    typedef AudioObjectId id_type;

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioObject object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioObject> create(AudioObjectName name,
                                               Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioObject
     *
     * The actual copy constructor is private to ensure that an AudioObject
     * can only be created as a `std::shared_ptr`. This is not a deep copy! All
     * referenced objects will be disconnected.
     */
    ADM_EXPORT std::shared_ptr<AudioObject> copy() const;

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

    /// @brief AudioObjectId setter
    ADM_EXPORT void set(AudioObjectId id);
    /// @brief AudioObjectName setter
    ADM_EXPORT void set(AudioObjectName name);
    /// @brief Start setter
    ADM_EXPORT void set(Start start);
    /// @brief Duration setter
    ADM_EXPORT void set(Duration duration);
    /// @brief DialogueId setter
    ADM_EXPORT void set(DialogueId id);
    /// @brief Importance setter
    ADM_EXPORT void set(Importance importance);
    /// @brief Interact setter
    ADM_EXPORT void set(Interact interact);
    /// @brief DisableDucking setter
    ADM_EXPORT void set(DisableDucking disableDucking);
    /// @brief AudioObjectInteraction setter
    ADM_EXPORT void set(AudioObjectInteraction objectInteraction);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    /// @brief Add reference to another AudioObject
    ADM_EXPORT bool addReference(std::shared_ptr<AudioObject> object);
    /// @brief Add reference to an AudioPackFormat
    ADM_EXPORT bool addReference(std::shared_ptr<AudioPackFormat> packFormat);
    /// @brief Add reference to an AudioTrackUid
    ADM_EXPORT bool addReference(std::shared_ptr<AudioTrackUid> trackUid);

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns a set of all references to the ADM elements with the
     * specified type.
     */
    template <typename Element>
    ElementRange<const Element> getReferences() const;

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns a set of all references to the ADM elements with the
     * specified type.
     */
    template <typename Element>
    ElementRange<Element> getReferences();

    /// @brief Remove reference to an AudioObject
    ADM_EXPORT void removeReference(std::shared_ptr<AudioObject> object);
    /// @brief Remove reference to an AudioPackFormat
    ADM_EXPORT void removeReference(
        std::shared_ptr<AudioPackFormat> packFormat);
    /// @brief Remove reference to an AudioTrackUid
    ADM_EXPORT void removeReference(std::shared_ptr<AudioTrackUid> trackUid);

    /**
     * @brief Clear references to Elements template
     *
     * Templated clear method with the ADM parameter type as template
     * argument. Removes all references to the ADM elements with the specified
     * type.
     */
    template <typename Element>
    void clearReferences();

    /// @brief Add reference to a complementary AudioObject
    ADM_EXPORT bool addComplementary(std::shared_ptr<AudioObject> object);
    /// @brief Get references to complementary AudioObjects
    ADM_EXPORT const std::vector<std::shared_ptr<AudioObject>>
        &getComplementaryObjects() const;
    /// @brief Remove reference to a complementary AudioObject
    ADM_EXPORT void removeComplementary(std::shared_ptr<AudioObject> object);
    /// @brief Remove all references to complementary AudioObjects
    ADM_EXPORT void clearComplementaryObjects();

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT std::weak_ptr<Document> getParent() const;

   private:
    friend class AudioObjectAttorney;

    ADM_EXPORT AudioObject(AudioObjectName name);
    ADM_EXPORT AudioObject(const AudioObject &) = default;
    ADM_EXPORT AudioObject(AudioObject &&) = default;

    ADM_EXPORT AudioObjectId
        get(detail::ParameterTraits<AudioObjectId>::tag) const;
    ADM_EXPORT AudioObjectName
        get(detail::ParameterTraits<AudioObjectName>::tag) const;
    ADM_EXPORT Start get(detail::ParameterTraits<Start>::tag) const;
    ADM_EXPORT Duration get(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT DialogueId get(detail::ParameterTraits<DialogueId>::tag) const;
    ADM_EXPORT Importance get(detail::ParameterTraits<Importance>::tag) const;
    ADM_EXPORT Interact get(detail::ParameterTraits<Interact>::tag) const;
    ADM_EXPORT DisableDucking
        get(detail::ParameterTraits<DisableDucking>::tag) const;
    ADM_EXPORT AudioObjectInteraction
        get(detail::ParameterTraits<AudioObjectInteraction>::tag) const;

    ADM_EXPORT bool has(detail::ParameterTraits<AudioObjectId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<AudioObjectName>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Start>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Duration>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DialogueId>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Importance>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Interact>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<DisableDucking>::tag) const;
    ADM_EXPORT bool has(
        detail::ParameterTraits<AudioObjectInteraction>::tag) const;

    template <typename Tag>
    bool isDefault(Tag) const {
      return false;
    }

    ADM_EXPORT bool isDefault(detail::ParameterTraits<Start>::tag) const;

    ADM_EXPORT void unset(detail::ParameterTraits<Start>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Duration>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DialogueId>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Importance>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<Interact>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<DisableDucking>::tag);
    ADM_EXPORT void unset(detail::ParameterTraits<AudioObjectInteraction>::tag);

    bool isAudioObjectReferenceCycle(
        std::shared_ptr<AudioObject> destinationObject);
    bool isComplementaryAudioObjectReferenceCycle(
        std::shared_ptr<AudioObject> destinationObject);

    ElementRange<const AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag) const;
    ElementRange<const AudioPackFormat> getReferences(
        detail::ParameterTraits<AudioPackFormat>::tag) const;
    ElementRange<const AudioTrackUid> getReferences(
        detail::ParameterTraits<AudioTrackUid>::tag) const;

    ElementRange<AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag);
    ElementRange<AudioPackFormat> getReferences(
        detail::ParameterTraits<AudioPackFormat>::tag);
    ElementRange<AudioTrackUid> getReferences(
        detail::ParameterTraits<AudioTrackUid>::tag);

    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioObject>::tag);
    ADM_EXPORT void clearReferences(
        detail::ParameterTraits<AudioPackFormat>::tag);
    ADM_EXPORT void clearReferences(
        detail::ParameterTraits<AudioTrackUid>::tag);

    ADM_EXPORT void disconnectReferences();

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;
    AudioObjectId id_;
    AudioObjectName name_;
    std::vector<std::shared_ptr<AudioObject>> audioObjects_;
    std::vector<std::shared_ptr<AudioObject>> audioComplementaryObjects_;
    std::vector<std::shared_ptr<AudioPackFormat>> audioPackFormats_;
    std::vector<std::shared_ptr<AudioTrackUid>> audioTrackUids_;
    boost::optional<Start> start_;
    boost::optional<Duration> duration_;
    boost::optional<DialogueId> dialogueId_;
    boost::optional<Importance> importance_;
    boost::optional<Interact> interact_;
    boost::optional<DisableDucking> disableDucking_;
    boost::optional<AudioObjectInteraction> audioObjectInteraction_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioObject> AudioObject::create(
      AudioObjectName name, Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioObject> object(new AudioObject(name));
    detail::setNamedOptionHelper(
        object, std::forward<Parameters>(optionalNamedArgs)...);

    return object;
  }

  template <typename Parameter>
  Parameter AudioObject::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool AudioObject::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool AudioObject::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void AudioObject::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename Element>
  ElementRange<const Element> AudioObject::getReferences() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  template <typename Element>
  ElementRange<Element> AudioObject::getReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  inline ElementRange<const AudioObject> AudioObject::getReferences(
      detail::ParameterTraits<AudioObject>::tag) const {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  };
  inline ElementRange<const AudioPackFormat> AudioObject::getReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) const {
    return detail::makeElementRange<AudioPackFormat>(audioPackFormats_);
  };
  inline ElementRange<const AudioTrackUid> AudioObject::getReferences(
      detail::ParameterTraits<AudioTrackUid>::tag) const {
    return detail::makeElementRange<AudioTrackUid>(audioTrackUids_);
  };

  inline ElementRange<AudioObject> AudioObject::getReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  };
  inline ElementRange<AudioPackFormat> AudioObject::getReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return detail::makeElementRange<AudioPackFormat>(audioPackFormats_);
  };
  inline ElementRange<AudioTrackUid> AudioObject::getReferences(
      detail::ParameterTraits<AudioTrackUid>::tag) {
    return detail::makeElementRange<AudioTrackUid>(audioTrackUids_);
  };

  template <typename Element>
  void AudioObject::clearReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    clearReferences(Tag());
  }

}  // namespace adm
