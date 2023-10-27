#ifndef LIBADM_ID_REF_TRAITS_HPP
#define LIBADM_ID_REF_TRAITS_HPP

#include "adm/serial/changed_ids.hpp"
#include "xml_parser_helper.hpp"

namespace adm {
  namespace xml {
    namespace detail {
      template <typename T>
      struct IDRefTraits {};

      template <>
      struct IDRefTraits<AudioChannelFormatIdRefs> {
        static constexpr const char* elementName{"audioChannelFormatIDRef"};
        using id_type = AudioChannelFormatId;
        using parser_t = decltype(parseAudioChannelFormatId);
        static constexpr parser_t* parseId{parseAudioChannelFormatId};
      };
      template <>
      struct IDRefTraits<AudioPackFormatIdRefs> {
        static constexpr const char* elementName{"audioPackFormatIDRef"};
        using id_type = AudioPackFormatId;
        using parser_t = decltype(parseAudioPackFormatId);
        static constexpr parser_t* parseId{parseAudioPackFormatId};
      };
      template <>
      struct IDRefTraits<AudioTrackUidIdRefs> {
        static constexpr const char* elementName{"audioTrackUIDRef"};
        using id_type = AudioTrackUidId;
        using parser_t = decltype(parseAudioTrackUidId);
        static constexpr parser_t* parseId{parseAudioTrackUidId};
      };
      template <>
      struct IDRefTraits<AudioTrackFormatIdRefs> {
        static constexpr const char* elementName{"audioTrackFormatIDRef"};
        using id_type = AudioTrackFormatId;
        using parser_t = decltype(parseAudioTrackFormatId);
        static constexpr parser_t* parseId{parseAudioTrackFormatId};
      };
      template <>
      struct IDRefTraits<AudioStreamFormatIdRefs> {
        static constexpr const char* elementName{"audioStreamFormatIDRef"};
        using id_type = AudioStreamFormatId;
        using parser_t = decltype(parseAudioStreamFormatId);
        static constexpr parser_t* parseId{parseAudioStreamFormatId};
      };
      template <>
      struct IDRefTraits<AudioObjectIdRefs> {
        static constexpr const char* elementName{"audioObjectIDRef"};
        using id_type = AudioObjectId;
        using parser_t = decltype(parseAudioObjectId);
        static constexpr parser_t* parseId{parseAudioObjectId};
      };
      template <>
      struct IDRefTraits<AudioContentIdRefs> {
        static constexpr const char* elementName{"audioContentIDRef"};
        using id_type = AudioContentId;
        using parser_t = decltype(parseAudioContentId);
        static constexpr parser_t* parseId{parseAudioContentId};
      };
      template <>
      struct IDRefTraits<AudioProgrammeIdRefs> {
        static constexpr const char* elementName{"audioProgrammeIDRef"};
        using id_type = AudioProgrammeId;
        using parser_t = decltype(parseAudioProgrammeId);
        static constexpr parser_t* parseId{parseAudioProgrammeId};
      };
    }  // namespace detail
  }  // namespace xml
}  // namespace adm

#endif  //LIBADM_ID_REF_TRAITS_HPP
