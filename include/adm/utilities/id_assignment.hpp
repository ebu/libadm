/// @file id_assignment.hpp
#pragma once

#include "adm/document.hpp"
#include "adm/export.h"

#include <map>
#include <memory>

namespace adm {

  /**
   * @brief Reassign ids of an Document
   *
   * Assigns new IDs to all the elements wihtin an Document. 
   * audioTrackFormats which are not referenced by an audioStreamFormat get an
   * ID with the value zero and are thereby marked as ADM elements which
   * should be ignored.
   * audioChannelFormats which are not referenced by an audioStreamFormat or
   * an audioTrackUid get an ID with the value zero and are thereby marked as
   * ADM elements which should be ignored.
   *
   * @note
   * Element that already have IDs with a value in the range 0x0001-0x0fff
   * will not get new Ids assigned, as they are considered to be common
   * definitions.
   */
  ADM_EXPORT void reassignIds(std::shared_ptr<Document> document);

  /** @name Check if ID is a common definitions ID
   */
  ///@{
  inline bool isCommonDefinitionsId(AudioProgrammeId id) {
    return (0x0001u <= id.get<AudioProgrammeIdValue>().get() &&
            id.get<AudioProgrammeIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioContentId id) {
    return (0x0001u <= id.get<AudioContentIdValue>().get() &&
            id.get<AudioContentIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioObjectId id) {
    return (0x0001u <= id.get<AudioObjectIdValue>().get() &&
            id.get<AudioObjectIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioTrackUidId /*id*/) { return false; }
  inline bool isCommonDefinitionsId(AudioPackFormatId id) {
    return (0x0001u <= id.get<AudioPackFormatIdValue>().get() &&
            id.get<AudioPackFormatIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioStreamFormatId id) {
    return (0x0001u <= id.get<AudioStreamFormatIdValue>().get() &&
            id.get<AudioStreamFormatIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioChannelFormatId id) {
    return (0x0001u <= id.get<AudioChannelFormatIdValue>().get() &&
            id.get<AudioChannelFormatIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioTrackFormatId id) {
    return (0x0001u <= id.get<AudioTrackFormatIdValue>().get() &&
            id.get<AudioTrackFormatIdValue>().get() <= 0xfffu);
  }
  inline bool isCommonDefinitionsId(AudioBlockFormatId id) {
    return (0x0001u <= id.get<AudioBlockFormatIdValue>().get() &&
            id.get<AudioBlockFormatIdValue>().get() <= 0xfffu);
  }
  ///@}

  /** @name Check if ID is undefined
   *
   * An ID is undefined if type, value and counter are 0u.
   */
  ///@{
  inline bool isUndefined(AudioProgrammeId id) {
    return (id == AudioProgrammeId());
  }
  inline bool isUndefined(AudioContentId id) {
    return (id == AudioContentId());
  }
  inline bool isUndefined(AudioObjectId id) { return (id == AudioObjectId()); }
  inline bool isUndefined(AudioTrackUidId id) {
    return (id == AudioTrackUidId());
  }
  inline bool isUndefined(AudioPackFormatId id) {
    return (id == AudioPackFormatId());
  }
  inline bool isUndefined(AudioStreamFormatId id) {
    return (id == AudioStreamFormatId());
  }
  inline bool isUndefined(AudioChannelFormatId id) {
    return (id == AudioChannelFormatId());
  }
  inline bool isUndefined(AudioTrackFormatId id) {
    return (id == AudioTrackFormatId());
  }
  inline bool isUndefined(AudioBlockFormatId id) {
    return (id == AudioBlockFormatId());
  }
  ///@}

  class IdReassigner {
   public:
    IdReassigner(std::shared_ptr<Document> document);

   private:
    void reassignAllIds();

    void reassignAudioProgrammeIds();
    void reassignAudioContentIds();
    void reassignAudioObjectIds();
    void reassignAudioPackFormatIds();
    void reassignAudioStreamFormatIds();
    void reassignAudioTrackUidIds();
    void reassignAudioBlockFormatIds(
        std::shared_ptr<AudioChannelFormat> audioChannelFormat);

    class IdIssuer {
     public:
      IdIssuer();
      AudioProgrammeId issueAudioProgrammeId();
      AudioContentId issueAudioContentId();
      AudioObjectId issueAudioObjectId();
      AudioTrackUidId issueAudioTrackUidId();
      AudioPackFormatId issueAudioPackFormatId(
          const TypeDescriptor& typeDescriptor);
      AudioChannelFormatId issueAudioChannelFormatId(
          const TypeDescriptor& typeDescriptor);
      uint16_t issueAudioChannelStreamTrackFormatIdValue(
          const TypeDescriptor& typeDescriptor);

     private:
      AudioProgrammeIdValue nextAudioProgrammeIdValue{0x1001};
      AudioContentIdValue nextAudioContentIdValue{0x1001};
      AudioObjectIdValue nextAudioObjectIdValue{0x1001};
      AudioTrackUidIdValue nextAudioTrackUidIdValue{0x00000001};
      std::map<TypeDescriptor, AudioPackFormatIdValue>
          nextAudioPackFormatIdValue;
      std::map<TypeDescriptor, uint16_t>
          nextAudioChannelStreamTrackFormatIdValue;

    } idIssuer;

    std::shared_ptr<Document> document;
  };

}  // namespace adm
