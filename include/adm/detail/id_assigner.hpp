#pragma once

#include "adm/elements_fwd.hpp"
#include <memory>
#include "adm/export.h"

namespace adm {

  class Document;

  namespace detail {
    /**
     * @brief Assigns a unique ID to elements.
     *
     * Works by using lookups to find the next available element 
     * ID.
     * 
     * @note This class differs from IdReassigner in that it can
     * function on a document which already has elements with ID's
     * which you wish to maintain. However, the trade-off is that
     * it is less efficient.
     */
    class IdAssigner {
     public:
      ADM_EXPORT AudioProgrammeId assignId(AudioProgramme& programme);
      ADM_EXPORT AudioContentId assignId(AudioContent& content);
      ADM_EXPORT AudioObjectId assignId(AudioObject& object);
      ADM_EXPORT AudioPackFormatId assignId(AudioPackFormat& packFormat);
      ADM_EXPORT AudioChannelFormatId
      assignId(AudioChannelFormat& channelFormat);
      ADM_EXPORT AudioStreamFormatId assignId(AudioStreamFormat& streamFormat);
      ADM_EXPORT AudioTrackFormatId assignId(AudioTrackFormat& trackFormat);
      ADM_EXPORT AudioTrackUidId assignId(AudioTrackUid& trackUid);

     protected:
      const Document& document() const;

     private:
      friend class adm::Document;
      ADM_EXPORT void document(Document* document_);
      Document* document_;
    };

  }  // namespace detail
}  // namespace adm
