#pragma once

#include "adm/elements_fwd.hpp"
#include <memory>
#include "adm/libadm_export.h"

namespace adm {

  class Document;

  namespace detail {

    class IdAssigner {
     public:
      LIBADM_EXPORT AudioProgrammeId assignId(AudioProgramme& programme);
      LIBADM_EXPORT AudioContentId assignId(AudioContent& content);
      LIBADM_EXPORT AudioObjectId assignId(AudioObject& object);
      LIBADM_EXPORT AudioPackFormatId assignId(AudioPackFormat& packFormat);
      LIBADM_EXPORT AudioChannelFormatId
      assignId(AudioChannelFormat& channelFormat);
      LIBADM_EXPORT AudioStreamFormatId
      assignId(AudioStreamFormat& streamFormat);
      LIBADM_EXPORT AudioTrackFormatId assignId(AudioTrackFormat& trackFormat);
      LIBADM_EXPORT AudioTrackUidId assignId(AudioTrackUid& trackUid);

     protected:
      const Document& document() const;

     private:
      friend class adm::Document;
      LIBADM_EXPORT void document(Document* document_);
      Document* document_;
    };

  }  // namespace detail
}  // namespace adm
