#include "adm/detail/id_assigner.hpp"
#include "adm/document.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/elements.hpp"

namespace adm {

  namespace detail {

    const Document& IdAssigner::document() const { return *document_; }

    void IdAssigner::document(Document* document) { document_ = document; }

    AudioProgrammeId IdAssigner::assignId(AudioProgramme& programme) {
      if (isCommonDefinitionsId(programme.get<AudioProgrammeId>())) {
        return programme.get<AudioProgrammeId>();
      }
      AudioProgrammeIdValue idValue(0x1001u);
      if (!isUndefined(programme.get<AudioProgrammeId>())) {
        idValue =
            programme.get<AudioProgrammeId>().get<AudioProgrammeIdValue>();
      }
      while (document().lookup(AudioProgrammeId(idValue))) {
        ++idValue;
      }
      auto id = AudioProgrammeId(idValue);
      programme.set(id);
      return id;
    }

    AudioContentId IdAssigner::assignId(AudioContent& content) {
      if (isCommonDefinitionsId(content.get<AudioContentId>())) {
        return content.get<AudioContentId>();
      }
      AudioContentIdValue idValue(0x1001u);
      if (!isUndefined(content.get<AudioContentId>())) {
        idValue = content.get<AudioContentId>().get<AudioContentIdValue>();
      }
      while (document().lookup(AudioContentId(idValue))) {
        ++idValue;
      }
      auto id = AudioContentId(idValue);
      content.set(id);
      return id;
    }

    AudioObjectId IdAssigner::assignId(AudioObject& object) {
      if (isCommonDefinitionsId(object.get<AudioObjectId>())) {
        return object.get<AudioObjectId>();
      }
      AudioObjectIdValue idValue(0x1001u);
      if (!isUndefined(object.get<AudioObjectId>())) {
        idValue = object.get<AudioObjectId>().get<AudioObjectIdValue>();
      }
      while (document().lookup(AudioObjectId(idValue))) {
        ++idValue;
      }
      auto id = AudioObjectId(idValue);
      object.set(id);
      return id;
    }

    AudioPackFormatId IdAssigner::assignId(AudioPackFormat& packFormat) {
      if (isCommonDefinitionsId(packFormat.get<AudioPackFormatId>())) {
        return packFormat.get<AudioPackFormatId>();
      }
      AudioPackFormatIdValue idValue(0x1001u);
      auto typeDescriptor = packFormat.get<TypeDescriptor>();
      if (!isUndefined(packFormat.get<AudioPackFormatId>())) {
        idValue =
            packFormat.get<AudioPackFormatId>().get<AudioPackFormatIdValue>();
      }
      while (document().lookup(AudioPackFormatId(typeDescriptor, idValue))) {
        ++idValue;
      }
      auto id = AudioPackFormatId(typeDescriptor, idValue);
      packFormat.set(id);
      return id;
    }

    AudioChannelFormatId IdAssigner::assignId(
        AudioChannelFormat& channelFormat) {
      if (isCommonDefinitionsId(channelFormat.get<AudioChannelFormatId>())) {
        return channelFormat.get<AudioChannelFormatId>();
      }
      AudioChannelFormatIdValue idValue(0x1001u);
      auto typeDescriptor = channelFormat.get<TypeDescriptor>();
      if (!isUndefined(channelFormat.get<AudioChannelFormatId>())) {
        idValue = channelFormat.get<AudioChannelFormatId>()
                      .get<AudioChannelFormatIdValue>();
      }
      while (document().lookup(AudioChannelFormatId(typeDescriptor, idValue))) {
        ++idValue;
      }
      auto id = AudioChannelFormatId(typeDescriptor, idValue);
      channelFormat.set(id);
      return id;
    }

    AudioStreamFormatId IdAssigner::assignId(AudioStreamFormat& streamFormat) {
      if (isCommonDefinitionsId(streamFormat.get<AudioStreamFormatId>())) {
        return streamFormat.get<AudioStreamFormatId>();
      }
      auto typeDescriptor = TypeDefinition::UNDEFINED;
      AudioStreamFormatIdValue idValue(0x1001u);
      if (!isUndefined(streamFormat.get<AudioStreamFormatId>())) {
        typeDescriptor =
            streamFormat.get<AudioStreamFormatId>().get<TypeDescriptor>();
        idValue = streamFormat.get<AudioStreamFormatId>()
                      .get<AudioStreamFormatIdValue>();
      } else {
        auto channelFormat = streamFormat.getReference<AudioChannelFormat>();
        auto packFormat = streamFormat.getReference<AudioPackFormat>();
        if (channelFormat) {
          typeDescriptor = channelFormat->get<TypeDescriptor>();
        } else if (packFormat) {
          typeDescriptor = packFormat->get<TypeDescriptor>();
        }
      }
      while (document().lookup(AudioStreamFormatId(typeDescriptor, idValue))) {
        ++idValue;
      }
      auto id = AudioStreamFormatId(typeDescriptor, idValue);
      streamFormat.set(id);
      return id;
    }

    AudioTrackFormatId IdAssigner::assignId(AudioTrackFormat& trackFormat) {
      if (isCommonDefinitionsId(trackFormat.get<AudioTrackFormatId>())) {
        return trackFormat.get<AudioTrackFormatId>();
      }
      auto typeDescriptor = TypeDefinition::UNDEFINED;
      AudioTrackFormatIdValue idValue(0x1001u);
      AudioTrackFormatIdCounter idCounter(1);
      if (!isUndefined(trackFormat.get<AudioTrackFormatId>())) {
        idValue = trackFormat.get<AudioTrackFormatId>()
                      .get<AudioTrackFormatIdValue>();
        idCounter = trackFormat.get<AudioTrackFormatId>()
                        .get<AudioTrackFormatIdCounter>();
        typeDescriptor =
            trackFormat.get<AudioTrackFormatId>().get<TypeDescriptor>();
      } else {
        auto streamFormat = trackFormat.getReference<AudioStreamFormat>();
        if (streamFormat) {
          auto streamFormatId = streamFormat->get<AudioStreamFormatId>();
          idValue = AudioTrackFormatIdValue(
              streamFormatId.get<AudioStreamFormatIdValue>().get());
          typeDescriptor = streamFormatId.get<TypeDescriptor>();
        }
      }
      while (document().lookup(
          AudioTrackFormatId(typeDescriptor, idValue, idCounter))) {
        ++idCounter;
      }
      auto id = AudioTrackFormatId(typeDescriptor, idValue, idCounter);
      trackFormat.set(id);
      return id;
    }

    AudioTrackUidId IdAssigner::assignId(AudioTrackUid& trackUid) {
      if (isCommonDefinitionsId(trackUid.get<AudioTrackUidId>()) ||
          trackUid.isSilent()) {
        return trackUid.get<AudioTrackUidId>();
      }
      AudioTrackUidIdValue idValue(0x0001u);
      if (!isUndefined(trackUid.get<AudioTrackUidId>())) {
        idValue = trackUid.get<AudioTrackUidId>().get<AudioTrackUidIdValue>();
      }
      while (document().lookup(AudioTrackUidId(idValue))) {
        ++idValue;
      }
      auto id = AudioTrackUidId(idValue);
      trackUid.set(id);
      return id;
    }

  }  // namespace detail
}  // namespace adm
