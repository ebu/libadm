/// @file id_assignment.hpp
#pragma once

#include "adm/document.hpp"
#include "adm/export.h"

#include <map>
#include <memory>

namespace adm {

  /**
   * @brief Reassign ID's of a Document
   *
   * Assigns new IDs to all the elements within an Document. 
   * AudioTrackFormats which are not referenced by an AudioStreamFormat get an
   * ID with the value zero and are thereby marked as ADM elements which
   * should be ignored.
   * AudioChannelFormats which are not referenced by an AudioStreamFormat or
   * an AudioTrackUid get an ID with the value zero and are thereby marked as
   * ADM elements which should be ignored.
   *
   * @note
   * Elements that already have ID's with a value in the range 0x0001-0x0fff
   * will not get new ID's assigned, as they are considered to be common
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

}  // namespace adm
