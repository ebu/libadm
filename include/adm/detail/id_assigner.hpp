#pragma once

#include "adm/elements_fwd.hpp"
#include <memory>
#include "adm/export.h"

namespace adm {

  class Document;

  namespace detail {

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
