#include "adm/utilities/object_creation.hpp"
#include <sstream>
#include "adm/common_definitions.hpp"
#include "adm/errors.hpp"

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

  SimpleObjectHolder addSimpleObjectTo(std::shared_ptr<Document> document,
                                       const std::string& name) {
    auto holder = createSimpleObject(name);
    document->add(holder.audioObject);
    return holder;
  }

  SimpleCommonDefinitionsObjectHolder addSimpleCommonDefinitionsObjectTo(
      std::shared_ptr<Document> document, const std::string& name,
      const std::string& speakerLayout) {
    SimpleCommonDefinitionsObjectHolder holder;
    holder.audioObject = AudioObject::create(AudioObjectName(name));
    if (audioPackFormatLookupTable().count(speakerLayout) == 0) {
      std::stringstream ss;
      ss << "Speaker Layout \"" << speakerLayout << "\" not supported.";
      throw error::AdmException(ss.str());
    }
    auto packFormat =
        document->lookup(audioPackFormatLookupTable().at(speakerLayout));
    if (!packFormat) {
      std::stringstream ss;
      ss << "AudioPackFormat for Speaker Layout \"" << speakerLayout
         << "\" not found. Are the common definitions added to the document?";
      throw error::AdmException(ss.str());
    }
    holder.audioObject->addReference(packFormat);

    auto labels = speakerLabelsLookupTable().at(speakerLayout);
    for (auto label : labels) {
      auto track = document->lookup(audioTrackFormatLookupTable().at(label));
      auto uid = AudioTrackUid::create();
      uid->setReference(packFormat);
      uid->setReference(track);
      holder.audioObject->addReference(uid);
      holder.audioTrackUids[label] = uid;
    }
    document->add(holder.audioObject);
    return holder;
  }
}  // namespace adm
