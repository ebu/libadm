/// @file frame.hpp
#pragma once

#include <memory>
#include <vector>
#include "adm/document.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/serial.hpp"

namespace adm {
  /**
   * @ingroup sadm
   * @brief Class representation of a whole adm frame
   */
  class Frame {
   public:
    /**
     * @brief Static helper function to create an Frame
     *
     * The constructor is private. This way it is ensured, that an Frame
     * can only be created as a `std::shared_ptr`.
     */
    ADM_EXPORT static std::shared_ptr<Frame> create(FrameStart start,
                                                    FrameDuration duration,
                                                    FrameType frameType);
    ADM_EXPORT static std::shared_ptr<Frame> create(FrameHeader frameHeader);

    /// @brief FrameStart getter
    ADM_EXPORT FrameStart start() const;
    /// @brief FrameDuration getter
    ADM_EXPORT FrameDuration duration() const;

    /**
     * @brief Create a copy of the Frame including all elements
     */
    ADM_EXPORT std::shared_ptr<Frame> deepCopy() const;

    /// @brief FrameHeader getter
    ADM_EXPORT const FrameHeader &frameHeader() const;
    ADM_EXPORT FrameHeader &frameHeader();
    ADM_EXPORT void setFrameHeader(FrameHeader frameHeader);
    
    /** @name Add Add ADM elements
     *
     * If the ADM element was already added to the Frame, it will not be
     * added again.
     */
    ///@{
    /// @brief Add an AudioProgramme
    ADM_EXPORT bool add(std::shared_ptr<AudioProgramme> programme);
    /// @brief Add an AudioContent
    ADM_EXPORT bool add(std::shared_ptr<AudioContent> content);
    /// @brief Add an AudioObject
    ADM_EXPORT bool add(std::shared_ptr<AudioObject> object);
    /// @brief Add an AudioPackFormat
    ADM_EXPORT bool add(std::shared_ptr<AudioPackFormat> packFormat);
    /// @brief Add an AudioChannelFormat
    ADM_EXPORT bool add(std::shared_ptr<AudioChannelFormat> channelFormat);
    /// @brief Add an AudioStreamFormat
    ADM_EXPORT bool add(std::shared_ptr<AudioStreamFormat> streamFormat);
    /// @brief Add an AudioTrackFormat
    ADM_EXPORT bool add(std::shared_ptr<AudioTrackFormat> trackFormat);
    /// @brief Add an AudioTrackUid
    ADM_EXPORT bool add(std::shared_ptr<AudioTrackUid> trackUid);
    ///@}

    /** @name Remove ADM elements
     *
     * References from and to the ADM element will automatically be removed
     * too.
     */
    ///@{
    /// @brief Remove an AudioProgramme
    ADM_EXPORT bool remove(std::shared_ptr<AudioProgramme> programme);
    /// @brief Remove an AudioContent
    ADM_EXPORT bool remove(std::shared_ptr<AudioContent> content);
    /// @brief Remove an AudioObject
    ADM_EXPORT bool remove(std::shared_ptr<AudioObject> object);
    /// @brief Remove an AudioPackFormat
    ADM_EXPORT bool remove(std::shared_ptr<AudioPackFormat> packFormat);
    /// @brief Remove an AudioChannelFormat
    ADM_EXPORT bool remove(std::shared_ptr<AudioChannelFormat> channelFormat);
    /// @brief Remove an AudioStreamFormat
    ADM_EXPORT bool remove(std::shared_ptr<AudioStreamFormat> streamFormat);
    /// @brief Remove an AudioTrackFormat
    ADM_EXPORT bool remove(std::shared_ptr<AudioTrackFormat> trackFormat);
    /// @brief Remove an AudioTrackUid
    ADM_EXPORT bool remove(std::shared_ptr<AudioTrackUid> trackUid);
    ///@}
    
    /**
     * @brief Adm elements getter template
     *
     * Templated getter with the wanted ADM element type as template
     * argument.
     *
     */
    template <typename Element>
    ElementRange<const Element> getElements() const;

    /**
     * @brief Adm elements getter template
     *
     * Templated getter with the wanted ADM element type as template
     * argument.
     *
     */
    template <typename Element>
    ElementRange<Element> getElements();

    /** @name Lookup Lookup ADM elements by its Id
     *
     * Lookup the first ADM element with the given Id.
     */
    ///@{
    /**
     * @brief Lookup AudioProgramme using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioProgramme> lookup(const AudioProgrammeId &programmeId);
    /**
     * @brief Lookup AudioProgramme using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioProgramme> lookup(
        const AudioProgrammeId &programmeId) const;
    /**
     * @brief Lookup AudioContent using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioContent> lookup(const AudioContentId &contentId);
    /**
     * @brief Lookup AudioContent using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioContent> lookup(
        const AudioContentId &contentId) const;
    /**
     * @brief Lookup AudioObject using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioObject> lookup(const AudioObjectId &objectId);
    /**
     * @brief Lookup AudioObject using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioObject> lookup(
        const AudioObjectId &objectId) const;
    /**
     * @brief Lookup AudioPackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioPackFormat> lookup(
        const AudioPackFormatId &packFormatId);
    /**
     * @brief Lookup AudioPackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioPackFormat> lookup(
        const AudioPackFormatId &packFormatId) const;
    /**
     * @brief Lookup AudioChannelFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioChannelFormat> lookup(
        const AudioChannelFormatId &channelFormatId);
    /**
     * @brief Lookup AudioChannelFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioChannelFormat> lookup(
        const AudioChannelFormatId &channelFormatId) const;
    /**
     * @brief Lookup AudioStreamFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioStreamFormat> lookup(
        const AudioStreamFormatId &streamFormatId);
    /**
     * @brief Lookup AudioStreamFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioStreamFormat> lookup(
        const AudioStreamFormatId &streamFormatId) const;
    /**
     * @brief Lookup AudioTrackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioTrackFormat> lookup(
        const AudioTrackFormatId &trackFormatId);
    /**
     * @brief Lookup AudioTrackFormat using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioTrackFormat> lookup(
        const AudioTrackFormatId &trackFormatId) const;
    /**
     * @brief Lookup AudioTrackUid using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<AudioTrackUid> lookup(const AudioTrackUidId &trackUidId);
    /**
     * @brief Lookup AudioTrackUid using its id
     * @returns `shared_ptr` to the element, `nullptr` if not found
     */
    std::shared_ptr<const AudioTrackUid> lookup(
        const AudioTrackUidId &trackUidId) const;
    ///@}

   private:
    ADM_EXPORT Frame(FrameStart start, FrameDuration duration,
                     FrameType frameType);
    ADM_EXPORT Frame(FrameHeader frameHeader);
    ADM_EXPORT Frame(const Frame &) = default;
    ADM_EXPORT Frame(Frame &&) = default;
    //
    // ADM_EXPORT ElementRange<const AudioProgramme> &getElements(
    //     detail::ParameterTraits<AudioProgramme>::tag) const;
    // ADM_EXPORT ElementRange<const AudioContent>
    //     &getElements(detail::ParameterTraits<AudioContent>::tag) const;
    // ADM_EXPORT ElementRange<const AudioObject>
    //     &getElements(detail::ParameterTraits<AudioObject>::tag) const;
    // ADM_EXPORT ElementRange<const AudioPackFormat>
    //     &getElements(detail::ParameterTraits<AudioPackFormat>::tag)
    //     const;
    // ADM_EXPORT ElementRange<const AudioChannelFormat>
    //     &getElements(
    //         detail::ParameterTraits<AudioChannelFormat>::tag) const;
    // ADM_EXPORT ElementRange<const AudioStreamFormat>
    //     getElements(detail::ParameterTraits<AudioStreamFormat>::tag)
    //     const;
    // ADM_EXPORT ElementRange<const AudioTrackFormat>
    //     &getElements(detail::ParameterTraits<AudioTrackFormat>::tag)
    //     const;
    // ADM_EXPORT ElementRange<const AudioTrackUid>
    //     &getElements(detail::ParameterTraits<AudioTrackUid>::tag) const;

    std::shared_ptr<Document> audioFormatExtended_;
    FrameHeader frameHeader_;
  };

  // ---- Implementation ---- //

  template <typename Element>
  ElementRange<const Element> Frame::getElements() const {
    return std::const_pointer_cast<const Document>(audioFormatExtended_)
        ->getElements<Element>();
  }

  template <typename Element>
  ElementRange<Element> Frame::getElements() {
    return audioFormatExtended_->getElements<Element>();
  }

  // ---- lookup elements ---- //
  inline std::shared_ptr<AudioProgramme> Frame::lookup(
      const AudioProgrammeId &programmeId) {
    return audioFormatExtended_->lookup(programmeId);
  }
  inline std::shared_ptr<const AudioProgramme> Frame::lookup(
      const AudioProgrammeId &programmeId) const {
    return audioFormatExtended_->lookup(programmeId);
  }
  inline std::shared_ptr<AudioContent> Frame::lookup(
      const AudioContentId &contentId) {
    return audioFormatExtended_->lookup(contentId);
  }
  inline std::shared_ptr<const AudioContent> Frame::lookup(
      const AudioContentId &contentId) const {
    return audioFormatExtended_->lookup(contentId);
  }
  inline std::shared_ptr<AudioObject> Frame::lookup(
      const AudioObjectId &objectId) {
    return audioFormatExtended_->lookup(objectId);
  }
  inline std::shared_ptr<const AudioObject> Frame::lookup(
      const AudioObjectId &objectId) const {
    return audioFormatExtended_->lookup(objectId);
  }
  inline std::shared_ptr<AudioPackFormat> Frame::lookup(
      const AudioPackFormatId &packFormatId) {
    return audioFormatExtended_->lookup(packFormatId);
  }
  inline std::shared_ptr<const AudioPackFormat> Frame::lookup(
      const AudioPackFormatId &packFormatId) const {
    return audioFormatExtended_->lookup(packFormatId);
  }
  inline std::shared_ptr<AudioChannelFormat> Frame::lookup(
      const AudioChannelFormatId &channelFormatId) {
    return audioFormatExtended_->lookup(channelFormatId);
  }
  inline std::shared_ptr<const AudioChannelFormat> Frame::lookup(
      const AudioChannelFormatId &channelFormatId) const {
    return audioFormatExtended_->lookup(channelFormatId);
  }
  inline std::shared_ptr<AudioStreamFormat> Frame::lookup(
      const AudioStreamFormatId &streamFormatId) {
    return audioFormatExtended_->lookup(streamFormatId);
  }
  inline std::shared_ptr<const AudioStreamFormat> Frame::lookup(
      const AudioStreamFormatId &streamFormatId) const {
    return audioFormatExtended_->lookup(streamFormatId);
  }
  inline std::shared_ptr<AudioTrackFormat> Frame::lookup(
      const AudioTrackFormatId &trackFormatId) {
    return audioFormatExtended_->lookup(trackFormatId);
  }
  inline std::shared_ptr<const AudioTrackFormat> Frame::lookup(
      const AudioTrackFormatId &trackFormatId) const {
    return audioFormatExtended_->lookup(trackFormatId);
  }
  inline std::shared_ptr<AudioTrackUid> Frame::lookup(
      const AudioTrackUidId &trackUidId) {
    return audioFormatExtended_->lookup(trackUidId);
  }
  inline std::shared_ptr<const AudioTrackUid> Frame::lookup(
      const AudioTrackUidId &trackUidId) const {
    return audioFormatExtended_->lookup(trackUidId);
  }

}  // namespace adm
