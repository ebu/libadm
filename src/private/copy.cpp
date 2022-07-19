#include "adm/private/copy.hpp"
#include "adm/document.hpp"

namespace adm {

  struct ElementMapping {
    // clang-format off
    std::map<std::shared_ptr<const AudioProgramme>, std::shared_ptr<AudioProgramme>> audioProgramme;
    std::map<std::shared_ptr<const AudioContent>, std::shared_ptr<AudioContent>> audioContent;
    std::map<std::shared_ptr<const AudioObject>, std::shared_ptr<AudioObject>> audioObject;
    std::map<std::shared_ptr<const AudioPackFormat>, std::shared_ptr<AudioPackFormat>> audioPackFormat;
    std::map<std::shared_ptr<const AudioChannelFormat>, std::shared_ptr<AudioChannelFormat>> audioChannelFormat;
    std::map<std::shared_ptr<const AudioStreamFormat>, std::shared_ptr<AudioStreamFormat>> audioStreamFormat;
    std::map<std::shared_ptr<const AudioTrackFormat>, std::shared_ptr<AudioTrackFormat>> audioTrackFormat;
    std::map<std::shared_ptr<const AudioTrackUid>, std::shared_ptr<AudioTrackUid>> audioTrackUid;
    // clang-format on
  };

  std::vector<ElementVariant> copyAllElements(
      std::shared_ptr<const Document> document) {
    ElementMapping mapping;
    std::vector<ElementVariant> copiedElements;
    // copy
    for (const auto& element : document->getElements<AudioProgramme>()) {
      mapping.audioProgramme[element] = element->copy();
      copiedElements.push_back(mapping.audioProgramme.at(element));
    }
    for (const auto& element : document->getElements<AudioContent>()) {
      mapping.audioContent[element] = element->copy();
      copiedElements.push_back(mapping.audioContent.at(element));
    }
    for (const auto& element : document->getElements<AudioObject>()) {
      mapping.audioObject[element] = element->copy();
      copiedElements.push_back(mapping.audioObject.at(element));
    }
    for (const auto& element : document->getElements<AudioPackFormat>()) {
      mapping.audioPackFormat[element] = element->copy();
      copiedElements.push_back(mapping.audioPackFormat.at(element));
    }
    for (const auto& element : document->getElements<AudioChannelFormat>()) {
      mapping.audioChannelFormat[element] = element->copy();
      copiedElements.push_back(mapping.audioChannelFormat.at(element));
    }
    for (const auto& element : document->getElements<AudioStreamFormat>()) {
      mapping.audioStreamFormat[element] = element->copy();
      copiedElements.push_back(mapping.audioStreamFormat.at(element));
    }
    for (const auto& element : document->getElements<AudioTrackFormat>()) {
      mapping.audioTrackFormat[element] = element->copy();
      copiedElements.push_back(mapping.audioTrackFormat.at(element));
    }
    for (const auto& element : document->getElements<AudioTrackUid>()) {
      mapping.audioTrackUid[element] = element->copy();
      copiedElements.push_back(mapping.audioTrackUid.at(element));
    }

    // resolve
    for (const auto& element : document->getElements<AudioProgramme>()) {
      resolveReferences(element, mapping.audioProgramme, mapping.audioContent);
    }
    for (const auto& element : document->getElements<AudioContent>()) {
      resolveReferences(element, mapping.audioContent, mapping.audioObject);
    }
    for (const auto& element : document->getElements<AudioObject>()) {
      resolveReferences(element, mapping.audioObject, mapping.audioObject);
      resolveReferences(element, mapping.audioObject, mapping.audioPackFormat);
      resolveReferences(element, mapping.audioObject, mapping.audioTrackUid);
      resolveComplementaries(element, mapping.audioObject, mapping.audioObject);
    }
    for (const auto& element : document->getElements<AudioPackFormat>()) {
      resolveReferences(element, mapping.audioPackFormat,
                        mapping.audioPackFormat);
      resolveReferences(element, mapping.audioPackFormat,
                        mapping.audioChannelFormat);
    }
    for (const auto& element : document->getElements<AudioStreamFormat>()) {
      resolveReference(element, mapping.audioStreamFormat,
                       mapping.audioPackFormat);
      resolveReference(element, mapping.audioStreamFormat,
                       mapping.audioChannelFormat);
      resolveReferences(element, mapping.audioStreamFormat,
                        mapping.audioTrackFormat);
    }
    for (const auto& element : document->getElements<AudioTrackFormat>()) {
      resolveReference(element, mapping.audioTrackFormat,
                       mapping.audioStreamFormat);
    }
    for (const auto& element : document->getElements<AudioTrackUid>()) {
      resolveReference(element, mapping.audioTrackUid,
                       mapping.audioTrackFormat);
      resolveReference(element, mapping.audioTrackUid, mapping.audioPackFormat);
      resolveReference(element, mapping.audioTrackUid,
                       mapping.audioChannelFormat);
    }
    return copiedElements;
  }

}  // namespace adm
