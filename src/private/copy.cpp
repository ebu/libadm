#include "adm/private/copy.hpp"
#include "adm/document.hpp"

namespace adm {

  std::vector<ElementVariant> copyAllElements(
      std::shared_ptr<const Document> document, ElementMapping& mapping) {
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

  std::vector<ElementVariant> copyAllElements(
      std::shared_ptr<const Document> document) {
    ElementMapping mapping;
    return copyAllElements(std::move(document), mapping);
  }

  void copyAuxiliary(std::shared_ptr<const Document> src,
                     std::shared_ptr<Document> dest,
                     ElementMapping const& mapping) {
    if (src->has<Version>()) dest->set(src->get<Version>());
    if (src->has<ProfileList>()) dest->set(src->get<ProfileList>());
    if (!src->has<TagList>()) return;

    auto srcTagList = src->get<TagList>();
    TagList newTagList;
    for (auto const& srcGroup : srcTagList.get<TagGroups>()) {
      // Translate each ref through the mapping. The source document is
      // assumed valid: Document::set(TagList) and Document::remove() keep
      // every TagGroup ref attached to the document, so it is guaranteed
      // to be in the mapping (mirrors the assumption used by
      // resolveReferences for ordinary cross-references). TagGroup has no
      // default ctor, so the first translated ref seeds the new group.
      std::unique_ptr<TagGroup> newGroup;
      auto translate = [&](auto const& srcRefs, auto const& mappingMap) {
        for (auto const& r : srcRefs) {
          auto const& mapped = mappingMap.at(r);
          if (!newGroup)
            newGroup.reset(new TagGroup(mapped));
          else
            newGroup->addReference(mapped);
        }
      };
      translate(srcGroup.getReferences<AudioObject>(), mapping.audioObject);
      translate(srcGroup.getReferences<AudioContent>(), mapping.audioContent);
      translate(srcGroup.getReferences<AudioProgramme>(),
                mapping.audioProgramme);
      if (!newGroup) continue;  // not possible for a valid source document
      for (auto const& tag : srcGroup.get<Tags>()) newGroup->add(tag);
      newTagList.add(*newGroup);
    }
    dest->set(std::move(newTagList));
  }

}  // namespace adm
