#pragma once

/**
 * @file elements_fwd.hpp
 *
 * Forward declarations for the ADM element classes.
 */

namespace adm {
  class Document;
  class AudioProgramme;
  class AudioContent;
  class AudioObject;
  class AudioPackFormat;
  class AudioChannelFormat;
  class AudioStreamFormat;
  class AudioTrackFormat;
  class AudioTrackUid;
  class AudioBlockFormatDirectSpeakers;
  class AudioBlockFormatMatrix;
  class AudioBlockFormatObjects;
  class AudioBlockFormatHoa;
  class AudioBlockFormatBinaural;

  struct AudioProgrammeTag;
  struct AudioContentTag;
  struct AudioObjectTag;
  struct AudioPackFormatTag;
  struct AudioChannelFormatTag;
  struct AudioStreamFormatTag;
  struct AudioTrackFormatTag;
  struct AudioTrackUidTag;
  struct AudioBlockFormatDirectSpeakersTag;
  struct AudioBlockFormatMatrixTag;
  struct AudioBlockFormatObjectsTag;
  struct AudioBlockFormatHoaTag;
  struct AudioBlockFormatBinauralTag;

  class AudioProgrammeId;
  class AudioContentId;
  class AudioObjectId;
  class AudioPackFormatId;
  class AudioChannelFormatId;
  class AudioBlockFormatId;
  class AudioStreamFormatId;
  class AudioTrackFormatId;
  class AudioTrackUidId;

  class LoudnessMetadata;
  class AudioProgrammeReferenceScreen;
}  // namespace adm
