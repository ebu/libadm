/// @file document.hpp
#pragma once

#include <memory>
#include <vector>
#include "adm/elements.hpp"
#include "adm/detail/id_assigner.hpp"
#include "adm/libadm_export.h"

namespace adm {

  /**
   * @ingroup main
   * @brief Class representation of a whole ADM document
   * @headerfile document.hpp <adm/document.hpp>
   *
   */
  class Document : public std::enable_shared_from_this<Document> {
   public:
    /**
     * @brief Static helper function to create an Document
     *
     * The constructor is private. This way it is ensured, that an Document
     * can only be created as a `std::shared_ptr`.
     */
    LIBADM_EXPORT static std::shared_ptr<Document> create();

    /**
     * @brief Create a copy of the Document including all elements
     */
    LIBADM_EXPORT std::shared_ptr<Document> deepCopy() const;

    /** @name Add ADM elements
     *
     * If the ADM element was already added to the Document, it will not be
     * added again.
     */
    ///@{
    /// @brief Add an AudioProgramme
    LIBADM_EXPORT bool add(std::shared_ptr<AudioProgramme> programme);
    /// @brief Add an AudioContent
    LIBADM_EXPORT bool add(std::shared_ptr<AudioContent> content);
    /// @brief Add an AudioObject
    LIBADM_EXPORT bool add(std::shared_ptr<AudioObject> object);
    /// @brief Add an AudioPackFormat
    LIBADM_EXPORT bool add(std::shared_ptr<AudioPackFormat> packFormat);
    /// @brief Add an AudioChannelFormat
    LIBADM_EXPORT bool add(std::shared_ptr<AudioChannelFormat> channelFormat);
    /// @brief Add an AudioStreamFormat
    LIBADM_EXPORT bool add(std::shared_ptr<AudioStreamFormat> streamFormat);
    /// @brief Add an AudioTrackFormat
    LIBADM_EXPORT bool add(std::shared_ptr<AudioTrackFormat> trackFormat);
    /// @brief Add an AudioTrackUid
    LIBADM_EXPORT bool add(std::shared_ptr<AudioTrackUid> trackUid);
    ///@}

    /** @name Remove ADM elements
     *
     * References from and to the ADM element will automatically be removed
     * too.
     */
    ///@{
    /// @brief Remove an AudioProgramme
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioProgramme> programme);
    /// @brief Remove an AudioContent
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioContent> content);
    /// @brief Remove an AudioObject
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioObject> object);
    /// @brief Remove an AudioPackFormat
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioPackFormat> packFormat);
    /// @brief Remove an AudioChannelFormat
    LIBADM_EXPORT bool remove(
        std::shared_ptr<AudioChannelFormat> channelFormat);
    /// @brief Remove an AudioStreamFormat
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioStreamFormat> streamFormat);
    /// @brief Remove an AudioTrackFormat
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioTrackFormat> trackFormat);
    /// @brief Remove an AudioTrackUid
    LIBADM_EXPORT bool remove(std::shared_ptr<AudioTrackUid> trackUid);
    ///@}

    /**
     * @brief ADM elements getter template
     *
     * Templated getter with the wanted ADM element type as template
     * argument.
     *
     */
    template <typename Element>
    ElementRange<const Element> getElements() const;

    /**
     * @brief ADM elements getter template
     *
     * Templated getter with the wanted ADM element type as template
     * argument.
     *
     */
    template <typename Element>
    ElementRange<Element> getElements();

    /** @name Lookup ADM elements by its Id
     *
     * Lookup the first ADM element with the given Id.
     *
     */
    ///@{
    /**
     * @brief Lookup AudioProgramme using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioProgramme> lookup(
        const AudioProgrammeId &programmeId);
    /**
     * @brief Lookup AudioProgramme using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioProgramme> lookup(
        const AudioProgrammeId &programmeId) const;
    /**
     * @brief Lookup AudioContent using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioContent> lookup(
        const AudioContentId &contentId);
    /**
     * @brief Lookup AudioContent using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioContent> lookup(
        const AudioContentId &contentId) const;
    /**
     * @brief Lookup AudioObject using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioObject> lookup(
        const AudioObjectId &objectId);
    /**
     * @brief Lookup AudioObject using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioObject> lookup(
        const AudioObjectId &objectId) const;
    /**
     * @brief Lookup AudioPackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioPackFormat> lookup(
        const AudioPackFormatId &packFormatId);
    /**
     * @brief Lookup AudioPackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioPackFormat> lookup(
        const AudioPackFormatId &packFormatId) const;
    /**
     * @brief Lookup AudioChannelFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioChannelFormat> lookup(
        const AudioChannelFormatId &channelFormatId);
    /**
     * @brief Lookup AudioChannelFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioChannelFormat> lookup(
        const AudioChannelFormatId &channelFormatId) const;
    /**
     * @brief Lookup AudioStreamFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioStreamFormat> lookup(
        const AudioStreamFormatId &streamFormatId);
    /**
     * @brief Lookup AudioStreamFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioStreamFormat> lookup(
        const AudioStreamFormatId &streamFormatId) const;
    /**
     * @brief Lookup AudioTrackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioTrackFormat> lookup(
        const AudioTrackFormatId &trackFormatId);
    /**
     * @brief Lookup AudioTrackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioTrackFormat> lookup(
        const AudioTrackFormatId &trackFormatId) const;
    /**
     * @brief Lookup AudioTrackUid using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<AudioTrackUid> lookup(
        const AudioTrackUidId &trackUidId);
    /**
     * @brief Lookup AudioTrackUid using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    LIBADM_EXPORT std::shared_ptr<const AudioTrackUid> lookup(
        const AudioTrackUidId &trackUidId) const;
    ///@}

   private:
    LIBADM_EXPORT Document();
    LIBADM_EXPORT Document(const Document &) = default;
    LIBADM_EXPORT Document(Document &&) = default;

    LIBADM_EXPORT ElementRange<const AudioProgramme> getElements(
        detail::ParameterTraits<AudioProgramme>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioContent> getElements(
        detail::ParameterTraits<AudioContent>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioObject> getElements(
        detail::ParameterTraits<AudioObject>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioPackFormat> getElements(
        detail::ParameterTraits<AudioPackFormat>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioChannelFormat> getElements(
        detail::ParameterTraits<AudioChannelFormat>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioStreamFormat> getElements(
        detail::ParameterTraits<AudioStreamFormat>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioTrackFormat> getElements(
        detail::ParameterTraits<AudioTrackFormat>::tag) const;
    LIBADM_EXPORT ElementRange<const AudioTrackUid> getElements(
        detail::ParameterTraits<AudioTrackUid>::tag) const;
    LIBADM_EXPORT ElementRange<AudioProgramme> getElements(
        detail::ParameterTraits<AudioProgramme>::tag);
    LIBADM_EXPORT ElementRange<AudioContent> getElements(
        detail::ParameterTraits<AudioContent>::tag);
    LIBADM_EXPORT ElementRange<AudioObject> getElements(
        detail::ParameterTraits<AudioObject>::tag);
    LIBADM_EXPORT ElementRange<AudioPackFormat> getElements(
        detail::ParameterTraits<AudioPackFormat>::tag);
    LIBADM_EXPORT ElementRange<AudioChannelFormat> getElements(
        detail::ParameterTraits<AudioChannelFormat>::tag);
    LIBADM_EXPORT ElementRange<AudioStreamFormat> getElements(
        detail::ParameterTraits<AudioStreamFormat>::tag);
    LIBADM_EXPORT ElementRange<AudioTrackFormat> getElements(
        detail::ParameterTraits<AudioTrackFormat>::tag);
    LIBADM_EXPORT ElementRange<AudioTrackUid> getElements(
        detail::ParameterTraits<AudioTrackUid>::tag);

    std::vector<std::shared_ptr<AudioProgramme>> audioProgrammes_;
    std::vector<std::shared_ptr<AudioContent>> audioContents_;
    std::vector<std::shared_ptr<AudioObject>> audioObjects_;
    std::vector<std::shared_ptr<AudioPackFormat>> audioPackFormats_;
    std::vector<std::shared_ptr<AudioChannelFormat>> audioChannelFormats_;
    std::vector<std::shared_ptr<AudioStreamFormat>> audioStreamFormats_;
    std::vector<std::shared_ptr<AudioTrackFormat>> audioTrackFormats_;
    std::vector<std::shared_ptr<AudioTrackUid>> audioTrackUids_;
    detail::IdAssigner idAssigner_;
  };

  // ---- Implementation ---- //

  template <typename Element>
  ElementRange<const Element> Document::getElements() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getElements(Tag());
  }

  template <typename Element>
  ElementRange<Element> Document::getElements() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getElements(Tag());
  }

}  // namespace adm
