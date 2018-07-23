#include "adm/utilities/object_creation.hpp"
#include <sstream>
#include "adm/common_definitions.hpp"

namespace adm {

  SimpleObjectHolder createSimpleObject(const std::string& name) {
    // create
    SimpleObjectHolder holder;
    holder.audioObject = AudioObject::create(AudioObjectName(name));

    holder.audioPackFormat = AudioPackFormat::create(AudioPackFormatName(name),
                                                     TypeDefinition::OBJECTS);
    holder.audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName(name), FormatDefinition::PCM);
    holder.audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName(name), FormatDefinition::PCM);
    holder.audioChannelFormat = AudioChannelFormat::create(
        AudioChannelFormatName(name), TypeDefinition::OBJECTS);
    holder.audioTrackUid = AudioTrackUid::create();

    // reference
    holder.audioObject->addReference(holder.audioPackFormat);
    holder.audioPackFormat->addReference(holder.audioChannelFormat);
    holder.audioStreamFormat->setReference(holder.audioChannelFormat);
    holder.audioTrackFormat->setReference(holder.audioStreamFormat);
    holder.audioObject->addReference(holder.audioTrackUid);
    holder.audioTrackUid->setReference(holder.audioTrackFormat);
    holder.audioTrackUid->setReference(holder.audioPackFormat);

    return holder;
  }
}  // namespace adm
