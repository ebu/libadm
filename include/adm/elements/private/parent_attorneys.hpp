#pragma once
#include <memory>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/audio_track_uid.hpp"

// see:
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Friendship_and_the_Attorney-Client

namespace adm {

  class AudioProgrammeAttorney {
   private:
    friend class Document;

    static void setParent(std::shared_ptr<AudioProgramme> programme,
                          std::weak_ptr<Document> parent) {
      programme->setParent(parent);
    }
  };

  class AudioContentAttorney {
   private:
    friend class Document;
    friend class AudioProgramme;

    static void setParent(std::shared_ptr<AudioContent> content,
                          std::weak_ptr<Document> parent) {
      content->setParent(parent);
    }
  };

  class AudioObjectAttorney {
   private:
    friend class Document;
    friend class AudioContent;
    friend class AudioObject;

    static void setParent(std::shared_ptr<AudioObject> object,
                          std::weak_ptr<Document> parent) {
      object->setParent(parent);
    }
  };

  class AudioPackFormatAttorney {
   private:
    friend class Document;
    friend class AudioPackFormat;
    friend class AudioTrackUid;
    friend class AudioStreamFormat;
    friend class AudioObject;

    static void setParent(std::shared_ptr<AudioPackFormat> packFormat,
                          std::weak_ptr<Document> parent) {
      packFormat->setParent(parent);
    }
  };

  class AudioChannelFormatAttorney {
   private:
    friend class Document;
    friend class AudioPackFormat;
    friend class AudioStreamFormat;

    static void setParent(std::shared_ptr<AudioChannelFormat> channelFormat,
                          std::weak_ptr<Document> parent) {
      channelFormat->setParent(parent);
    }
  };

  class AudioStreamFormatAttorney {
   private:
    friend class Document;
    friend class AudioTrackFormat;

    static void setParent(std::shared_ptr<AudioStreamFormat> streamFormat,
                          std::weak_ptr<Document> parent) {
      streamFormat->setParent(parent);
    }
  };

  class AudioTrackFormatAttorney {
   private:
    friend class Document;
    friend class AudioTrackUid;
    friend class AudioStreamFormat;

    static void setParent(std::shared_ptr<AudioTrackFormat> trackFormat,
                          std::weak_ptr<Document> parent) {
      trackFormat->setParent(parent);
    }
  };

  class AudioTrackUidAttorney {
   private:
    friend class Document;
    friend class AudioObject;

    static void setParent(std::shared_ptr<AudioTrackUid> trackFormat,
                          std::weak_ptr<Document> parent) {
      trackFormat->setParent(parent);
    }
  };

}  // namespace adm
