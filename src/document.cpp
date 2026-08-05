#include "adm/document.hpp"

#include "adm/elements.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/lookup.hpp"
#include "adm/detail/id_assigner.hpp"
#include "adm/private/copy.hpp"

#include <algorithm>
#include <stdexcept>

namespace adm {
  namespace {
    template <typename ContainerT, typename Predicate>
    bool pruneIf(ContainerT& container, Predicate predicate) {
      auto end = std::remove_if(container.begin(), container.end(), predicate);
      if (end == container.end()) {
        return false;
      }
      container.erase(end, container.end());
      return true;
    }

    template <typename Element>
    void pruneTagGroupsReferencing(
        Document& document,
        std::shared_ptr<Element> const& removedElement) {
      if (!document.has<TagList>()) return;
      auto list = document.get<TagList>();
      auto groups = list.get<TagGroups>();
      auto pruned = pruneIf(groups, [&](TagGroup const& group) {
        auto refs = group.template getReferences<Element>();
        return std::find(refs.begin(), refs.end(), removedElement) !=
               refs.end();
      });
      if (!pruned) return;
      if (groups.empty()) {
        document.unset<TagList>();
        return;
      }
      TagList newList;
      for (auto& group : groups) newList.add(group);
      document.set(newList);
    }
  }  // namespace

  namespace detail {
    template class OptionalParameter<Version>;
    template class OptionalParameter<TagList>;
    template class OptionalParameter<ProfileList>;
  }  // namespace detail

  namespace {
    // Bound the recursion depth of Document::add() to prevent a stack
    // overflow when the cross-reference graph is pathologically deep
    // (e.g. an adversarial XML with thousands of nested
    // audioPackFormatIDRefs). Re-entering an already-added element is
    // already short-circuited by checkParent(), so this only fires on
    // genuinely deep, unique chains. The limit is far above any realistic
    // ADM document.
    constexpr int kMaxAddRecursionDepth = 1000;
    thread_local int g_addRecursionDepth = 0;

    struct AddRecursionGuard {
      AddRecursionGuard() {
        if (g_addRecursionDepth >= kMaxAddRecursionDepth) {
          throw std::runtime_error(
              "Document::add: cross-reference recursion depth exceeded "
              "(possible deeply-nested or malformed input)");
        }
        ++g_addRecursionDepth;
      }
      ~AddRecursionGuard() { --g_addRecursionDepth; }
      AddRecursionGuard(const AddRecursionGuard&) = delete;
      AddRecursionGuard& operator=(const AddRecursionGuard&) = delete;
    };

    template <typename T>
    bool pruneLoudnessMetadataObjectRefs(
      LoudnessMetadatas& data,
      std::shared_ptr<T> removedElement) {
      bool changed = false;
      for (auto& loudnessMetadata : data) {
        if (!loudnessMetadata.has<LoudnessRenderer>()) {
          continue;
        }
        auto renderer = loudnessMetadata.get<LoudnessRenderer>();
        std::vector<std::shared_ptr<T>> refsToRemove;
        auto const removedId = removedElement->template get<typename T::id_type>();
        for (auto const& ref : renderer.template getReferences<T>()) {
          if (ref == removedElement ||
              ref->template get<typename T::id_type>() == removedId) {
            refsToRemove.push_back(ref);
          }
        }
        for (auto const& ref : refsToRemove) {
          renderer.removeReference(ref);
        }
        if (!refsToRemove.empty()) {
          loudnessMetadata.set(std::move(renderer));
          changed = true;
        }
      }
      return changed;
    }

    bool pruneLoudnessMetadataPackFormatRefs(
      LoudnessMetadatas& data,
      std::shared_ptr<AudioPackFormat> const& removedPackFormat) {
      bool changed = false;
      auto const removedId = removedPackFormat->get<AudioPackFormatId>();
      for (auto& loudnessMetadata : data) {
        if (!loudnessMetadata.has<LoudnessRenderer>()) {
          continue;
        }
        auto renderer = loudnessMetadata.get<LoudnessRenderer>();
        if (!renderer.has<RendererPackFormatIdRef>()) {
          continue;
        }
        auto const& ref = renderer.get<RendererPackFormatIdRef>();
        if (ref == removedPackFormat || ref->get<AudioPackFormatId>() == removedId) {
          renderer.unset<RendererPackFormatIdRef>();
          loudnessMetadata.set(std::move(renderer));
          changed = true;
        }
      }
      return changed;
    }

    template<typename Element>
    void pruneElementLoudnessPackRefs(
      std::shared_ptr<Element> const& element,
      std::shared_ptr<AudioPackFormat> const& removedElement) {
      if (!element->template has<LoudnessMetadatas>()) {
        return;
      }
      auto loudnessMetadatas = element->template get<LoudnessMetadatas>();
      if (!pruneLoudnessMetadataPackFormatRefs(loudnessMetadatas, removedElement)) {
        return;
      }
      element->set(std::move(loudnessMetadatas));
    }

    template<typename Element, typename RemovedElement>
    void pruneElementLoudnessObjectRefs(
      std::shared_ptr<Element> const& element,
      std::shared_ptr<RemovedElement> const& removedElement) {
      if (!element->template has<LoudnessMetadatas>()) {
        return;
      }
      auto loudnessMetadatas = element->template get<LoudnessMetadatas>();
      if (!pruneLoudnessMetadataObjectRefs(loudnessMetadatas, removedElement)) {
        return;
      }
      element->set(std::move(loudnessMetadatas));
    }

    bool pruneAuthoringInformationPackFormatIdRefs(
        AuthoringInformation& info,
        AudioPackFormatId const& removedId,
        std::shared_ptr<AudioPackFormat> const& removedPackFormat) {
      bool changed = false;

      if (info.has<Renderers>()) {
        auto renderers = info.get<Renderers>();
        bool renderersChanged = false;
        for (auto& renderer : renderers) {
          auto const removedId =
              removedPackFormat->get<AudioPackFormatId>();
          std::vector<std::shared_ptr<AudioPackFormat>> refsToRemove;
          for (auto const& ref :
               renderer.getReferences<AudioPackFormat>()) {
            if (ref == removedPackFormat ||
                ref->get<AudioPackFormatId>() == removedId) {
              refsToRemove.push_back(ref);
            }
          }
          for (auto const& ref : refsToRemove) {
            renderer.removeReference(ref);
          }
          renderersChanged |= !refsToRemove.empty();
        }
        if (renderersChanged) {
          if (renderers.empty()) {
            info.unset<Renderers>();
          } else {
            info.set(std::move(renderers));
          }
          changed = true;
        }
      }

      if (info.has<ReferenceLayouts>()) {
        auto referenceLayouts = info.get<ReferenceLayouts>();
        auto pruned = pruneIf(referenceLayouts, [&](ReferenceLayout const& layout) {
          auto const& ref = layout.get();
          if (ref == removedPackFormat) {
            return true;
          }
          return ref->get<AudioPackFormatId>() == removedId;
        });
        if (pruned) {
          if (referenceLayouts.empty()) {
            info.unset<ReferenceLayouts>();
          } else {
            info.set(std::move(referenceLayouts));
          }
          changed = true;
        }
      }

      return changed;
    }

    void prunePackFormatIdRefs(
        Document& document,
        AudioPackFormatId const& removedId,
        std::shared_ptr<AudioPackFormat> const& removedPackFormat) {
      for (auto const& programme : document.getElements<AudioProgramme>()) {
        if (programme->has<AuthoringInformation>()) {
          auto info = programme->get<AuthoringInformation>();
          if (pruneAuthoringInformationPackFormatIdRefs(
                  info,
                  removedId,
                  removedPackFormat)) {
            programme->set(std::move(info));
          }
        }
        pruneElementLoudnessPackRefs(programme, removedPackFormat);
      }

      for (auto const& content : document.getElements<AudioContent>()) {
        pruneElementLoudnessPackRefs(content, removedPackFormat);
      }
    }

    void pruneObjectIdRefs(
        Document& document,
        std::shared_ptr<AudioObject> const& removedObject) {
      for (auto const& programme : document.getElements<AudioProgramme>()) {
        pruneElementLoudnessObjectRefs(programme, removedObject);
      }

      for (auto const& content : document.getElements<AudioContent>()) {
        pruneElementLoudnessObjectRefs(content, removedObject);
      }
    }
  }  // namespace

  Document::Document() { idAssigner_.document(this); }

  std::shared_ptr<Document> Document::create() {
    return std::shared_ptr<Document>(new Document());
  }

  std::shared_ptr<Document> Document::deepCopy() const {
    auto copy = Document::create();
    copy->audioProgrammes_.reserve(audioProgrammes_.size());
    copy->audioContents_.reserve(audioContents_.size());
    copy->audioObjects_.reserve(audioObjects_.size());
    copy->audioPackFormats_.reserve(audioPackFormats_.size());
    copy->audioChannelFormats_.reserve(audioChannelFormats_.size());
    copy->audioStreamFormats_.reserve(audioStreamFormats_.size());
    copy->audioTrackFormats_.reserve(audioTrackFormats_.size());
    copy->audioTrackUids_.reserve(audioTrackUids_.size());

    ElementMapping mapping;
    auto elements = copyAllElements(shared_from_this(), mapping);
    for (auto& e : elements) {
      if (auto v = boost::get<std::shared_ptr<AudioProgramme>>(&e)) {
        AudioProgrammeAttorney::setParent(*v, copy);
        copy->audioProgrammes_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioContent>>(&e)) {
        AudioContentAttorney::setParent(*v, copy);
        copy->audioContents_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioObject>>(&e)) {
        AudioObjectAttorney::setParent(*v, copy);
        copy->audioObjects_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioPackFormat>>(&e)) {
        AudioPackFormatAttorney::setParent(*v, copy);
        copy->audioPackFormats_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioChannelFormat>>(&e)) {
        AudioChannelFormatAttorney::setParent(*v, copy);
        copy->audioChannelFormats_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioStreamFormat>>(&e)) {
        AudioStreamFormatAttorney::setParent(*v, copy);
        copy->audioStreamFormats_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioTrackFormat>>(&e)) {
        AudioTrackFormatAttorney::setParent(*v, copy);
        copy->audioTrackFormats_.push_back(*v);
      } else if (auto v = boost::get<std::shared_ptr<AudioTrackUid>>(&e)) {
        AudioTrackUidAttorney::setParent(*v, copy);
        copy->audioTrackUids_.push_back(*v);
      }
    }
    for (auto const& programme : copy->getElements<AudioProgramme>()) {
      AudioProgrammeAttorney::setAuthoringInformationParent(programme, copy);
      AudioProgrammeAttorney::setLoudnessMetadataParent(programme, copy);
    }
    for (auto const& content : copy->getElements<AudioContent>()) {
      AudioContentAttorney::setLoudnessMetadataParent(content, copy);
    }
    copyAuxiliary(shared_from_this(), copy, mapping);
    return copy;
  }

  // ---- add elements ---- //
  bool Document::add(std::shared_ptr<AudioProgramme> programme) {
    AddRecursionGuard guard;
    if (!checkParent(programme, "AudioProgramme")) {
      idAssigner_.assignId(*programme);
      AudioProgrammeAttorney::setParent(programme, shared_from_this());
      audioProgrammes_.push_back(programme);
      AudioProgrammeAttorney::setAuthoringInformationParent(
          programme, shared_from_this());
      AudioProgrammeAttorney::setLoudnessMetadataParent(
          programme, shared_from_this());
      for (auto& reference : programme->getReferences<AudioContent>()) {
        add(reference);
      }
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioContent> content) {
    AddRecursionGuard guard;
    if (!checkParent(content, "AudioContent")) {
      idAssigner_.assignId(*content);
      AudioContentAttorney::setParent(content, shared_from_this());
      audioContents_.push_back(content);
      AudioContentAttorney::setLoudnessMetadataParent(
          content, shared_from_this());
      for (auto& reference : content->getReferences<AudioObject>()) {
        add(reference);
      }
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioObject> object) {
    AddRecursionGuard guard;
    if (!checkParent(object, "AudioObject")) {
      idAssigner_.assignId(*object);
      AudioObjectAttorney::setParent(object, shared_from_this());
      audioObjects_.push_back(object);
      for (auto& reference : object->getReferences<AudioObject>()) {
        add(reference);
      }
      for (auto& reference : object->getReferences<AudioPackFormat>()) {
        add(reference);
      }
      for (auto& reference : object->getReferences<AudioTrackUid>()) {
        add(reference);
      }
      for (auto& reference : object->getComplementaryObjects()) {
        add(reference);
      }
      return true;
    } else {
      return false;
    }
  }
  bool Document::add(std::shared_ptr<AudioPackFormat> packFormat) {
    AddRecursionGuard guard;
    if (!checkParent(packFormat, "AudioPackFormat")) {
      idAssigner_.assignId(*packFormat);
      AudioPackFormatAttorney::setParent(packFormat, shared_from_this());
      audioPackFormats_.push_back(packFormat);
      for (auto& reference : packFormat->getReferences<AudioPackFormat>()) {
        add(reference);
      }
      for (auto& reference : packFormat->getReferences<AudioChannelFormat>()) {
        add(reference);
      }
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioChannelFormat> channelFormat) {
    AddRecursionGuard guard;
    if (!checkParent(channelFormat, "AudioChannelFormat")) {
      idAssigner_.assignId(*channelFormat);
      AudioChannelFormatAttorney::setParent(channelFormat, shared_from_this());
      audioChannelFormats_.push_back(channelFormat);
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioStreamFormat> streamFormat) {
    AddRecursionGuard guard;
    if (!checkParent(streamFormat, "AudioStreamFormat")) {
      idAssigner_.assignId(*streamFormat);
      AudioStreamFormatAttorney::setParent(streamFormat, shared_from_this());
      audioStreamFormats_.push_back(streamFormat);
      auto audioChannelFormat =
          streamFormat->getReference<AudioChannelFormat>();
      if (audioChannelFormat) {
        add(audioChannelFormat);
      }
      auto audioPackFormat = streamFormat->getReference<AudioPackFormat>();
      if (audioPackFormat) {
        add(audioPackFormat);
      }
      for (auto& weak_reference :
           streamFormat->getAudioTrackFormatReferences()) {
        auto reference = weak_reference.lock();
        if (reference) {
          add(reference);
        }
      }
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioTrackFormat> trackFormat) {
    AddRecursionGuard guard;
    if (!checkParent(trackFormat, "AudioTrackFormat")) {
      // NOTE: That the id assignment works properly the AudioStreamFormats
      // have to be added before the AudioTrackFormat.
      auto audioStreamFormat = trackFormat->getReference<AudioStreamFormat>();
      if (audioStreamFormat) {
        add(audioStreamFormat);
      }
      auto it = std::find(audioTrackFormats_.begin(), audioTrackFormats_.end(),
                          trackFormat);
      if (it != audioTrackFormats_.end()) {
        return true;
      }
      idAssigner_.assignId(*trackFormat);
      AudioTrackFormatAttorney::setParent(trackFormat, shared_from_this());
      audioTrackFormats_.push_back(trackFormat);
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioTrackUid> trackUid) {
    AddRecursionGuard guard;
    if (!checkParent(trackUid, "AudioTrackUid")) {
      idAssigner_.assignId(*trackUid);
      AudioTrackUidAttorney::setParent(trackUid, shared_from_this());
      audioTrackUids_.push_back(trackUid);

      auto audioTrackFormat = trackUid->getReference<AudioTrackFormat>();
      if (audioTrackFormat) {
        add(audioTrackFormat);
      }
      auto audioPackFormat = trackUid->getReference<AudioPackFormat>();
      if (audioPackFormat) {
        add(audioPackFormat);
      }
      auto audioChannelFormat = trackUid->getReference<AudioChannelFormat>();
      if (audioChannelFormat) {
        add(audioChannelFormat);
      }
      return true;
    } else {
      return false;
    }
  }

  // ---- remove elements --- //
  bool Document::remove(std::shared_ptr<AudioProgramme> programme) {
    auto it =
        std::find(audioProgrammes_.begin(), audioProgrammes_.end(), programme);
    if (it != audioProgrammes_.end()) {
      audioProgrammes_.erase(it);
      AudioProgrammeAttorney::setParent(programme, {});
      AudioProgrammeAttorney::setAuthoringInformationParent(programme, {});
      AudioProgrammeAttorney::setLoudnessMetadataParent(programme, {});
      pruneTagGroupsReferencing(*this, programme);
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioContent> content) {
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it != audioContents_.end()) {
      audioContents_.erase(it);
      AudioContentAttorney::setParent(content, {});
      AudioContentAttorney::setLoudnessMetadataParent(content, {});
      for (auto& audioProgramme : audioProgrammes_) {
        audioProgramme->removeReference(content);
      }
      pruneTagGroupsReferencing(*this, content);
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
      AudioObjectAttorney::setParent(object, {});
      for (auto& audioObject : audioObjects_) {
        audioObject->removeReference(object);
      }
      for (auto& audioContent : audioContents_) {
        audioContent->removeReference(object);
      }
      pruneTagGroupsReferencing(*this, object);
      pruneObjectIdRefs(*this, object);
      return true;
    }
    return false;
  }

  namespace {
    template <typename Element>
    bool tagGroupRefsBelongToOtherDoc(
        Document const& doc,
        std::vector<std::shared_ptr<Element>> const& refs) {
      for (auto const& ref : refs) {
        auto parent = ref->getParent().lock();
        if (parent && parent.get() != &doc) return true;
      }
      return false;
    }
  }  // namespace

  bool Document::set(TagList tagList) {
    // Validate every TagGroup reference against this document up-front so a
    // failure leaves the document unmodified.
    for (auto const& group : tagList.get<TagGroups>()) {
      if (tagGroupRefsBelongToOtherDoc(*this, group.audioProgrammes_) ||
          tagGroupRefsBelongToOtherDoc(*this, group.audioContents_) ||
          tagGroupRefsBelongToOtherDoc(*this, group.audioObjects_)) {
        return false;
      }
    }
    // Adopt any unparented references into this document. Elements already
    // belonging to this document are short-circuited by checkParent() inside
    // add().
    for (auto const& group : tagList.get<TagGroups>()) {
      for (auto const& p : group.audioProgrammes_) add(p);
      for (auto const& c : group.audioContents_) add(c);
      for (auto const& o : group.audioObjects_) add(o);
    }
    detail::DocumentBase::set(std::move(tagList));
    return true;
  }

  bool Document::remove(std::shared_ptr<AudioPackFormat> packFormat) {
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it != audioPackFormats_.end()) {
      auto removedPackId = packFormat->get<AudioPackFormatId>();
      audioPackFormats_.erase(it);
      AudioPackFormatAttorney::setParent(packFormat, {});
      for (auto& audioPackFormat : audioPackFormats_) {
        audioPackFormat->removeReference(packFormat);
      }
      for (auto& audioObject : audioObjects_) {
        audioObject->removeReference(packFormat);
      }
      for (auto& audioStreamFormat : audioStreamFormats_) {
        if (audioStreamFormat->getReference<AudioPackFormat>() == packFormat) {
          audioStreamFormat->removeReference<AudioPackFormat>();
        }
      }
      for (auto& audioTrackUid : audioTrackUids_) {
        if (audioTrackUid->getReference<AudioPackFormat>() == packFormat) {
          audioTrackUid->removeReference<AudioPackFormat>();
        }
      }
      prunePackFormatIdRefs(*this, removedPackId, packFormat);
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioChannelFormat> channelFormat) {
    auto it = std::find(audioChannelFormats_.begin(),
                        audioChannelFormats_.end(), channelFormat);
    if (it != audioChannelFormats_.end()) {
      audioChannelFormats_.erase(it);
      AudioChannelFormatAttorney::setParent(channelFormat, {});
      for (auto& audioPackFormat : audioPackFormats_) {
        audioPackFormat->removeReference(channelFormat);
      }
      for (auto& audioStreamFormat : audioStreamFormats_) {
        if (audioStreamFormat->getReference<AudioChannelFormat>() ==
            channelFormat) {
          audioStreamFormat->removeReference<AudioChannelFormat>();
        }
      }
      for (auto& audioTrackUid : audioTrackUids_) {
        if (audioTrackUid->getReference<AudioChannelFormat>() ==
            channelFormat) {
          audioTrackUid->removeReference<AudioChannelFormat>();
        }
      }
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioStreamFormat> streamFormat) {
    auto it = std::find(audioStreamFormats_.begin(), audioStreamFormats_.end(),
                        streamFormat);
    if (it != audioStreamFormats_.end()) {
      audioStreamFormats_.erase(it);
      AudioStreamFormatAttorney::setParent(streamFormat, {});

      for (auto& audioTrackFormat : audioTrackFormats_) {
        if (audioTrackFormat->getReference<AudioStreamFormat>() ==
            streamFormat) {
          audioTrackFormat->removeReference<AudioStreamFormat>();
        }
      }
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioTrackFormat> trackFormat) {
    auto it = std::find(audioTrackFormats_.begin(), audioTrackFormats_.end(),
                        trackFormat);
    if (it != audioTrackFormats_.end()) {
      audioTrackFormats_.erase(it);
      AudioTrackFormatAttorney::setParent(trackFormat, {});

      for (auto& audioStreamFormat : audioStreamFormats_) {
        audioStreamFormat->removeReference(trackFormat);
      }
      for (auto& audioTrackUid : audioTrackUids_) {
        if (audioTrackUid->getReference<AudioTrackFormat>() == trackFormat) {
          audioTrackUid->removeReference<AudioTrackFormat>();
        }
      }
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioTrackUid> trackUid) {
    auto it =
        std::find(audioTrackUids_.begin(), audioTrackUids_.end(), trackUid);
    if (it != audioTrackUids_.end()) {
      audioTrackUids_.erase(it);
      AudioTrackUidAttorney::setParent(trackUid, {});
      for (auto& audioObject : audioObjects_) {
        audioObject->removeReference(trackUid);
      }
      return true;
    }
    return false;
  }

  // ---- get elements ---- //
  ElementRange<const AudioProgramme> Document::getElements(
      detail::ParameterTraits<AudioProgramme>::tag) const {
    return detail::makeElementRange<AudioProgramme>(audioProgrammes_);
  }

  ElementRange<const AudioContent> Document::getElements(
      detail::ParameterTraits<AudioContent>::tag) const {
    return detail::makeElementRange<AudioContent>(audioContents_);
  }

  ElementRange<const AudioObject> Document::getElements(
      detail::ParameterTraits<AudioObject>::tag) const {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  ElementRange<const AudioPackFormat> Document::getElements(
      detail::ParameterTraits<AudioPackFormat>::tag) const {
    return detail::makeElementRange<AudioPackFormat>(audioPackFormats_);
  }

  ElementRange<const AudioChannelFormat> Document::getElements(
      detail::ParameterTraits<AudioChannelFormat>::tag) const {
    return detail::makeElementRange<AudioChannelFormat>(audioChannelFormats_);
  }

  ElementRange<const AudioStreamFormat> Document::getElements(
      detail::ParameterTraits<AudioStreamFormat>::tag) const {
    return detail::makeElementRange<AudioStreamFormat>(audioStreamFormats_);
  }

  ElementRange<const AudioTrackFormat> Document::getElements(
      detail::ParameterTraits<AudioTrackFormat>::tag) const {
    return detail::makeElementRange<AudioTrackFormat>(audioTrackFormats_);
  }

  ElementRange<const AudioTrackUid> Document::getElements(
      detail::ParameterTraits<AudioTrackUid>::tag) const {
    return detail::makeElementRange<AudioTrackUid>(audioTrackUids_);
  }

  ElementRange<AudioProgramme> Document::getElements(
      detail::ParameterTraits<AudioProgramme>::tag) {
    return detail::makeElementRange<AudioProgramme>(audioProgrammes_);
  }

  ElementRange<AudioContent> Document::getElements(
      detail::ParameterTraits<AudioContent>::tag) {
    return detail::makeElementRange<AudioContent>(audioContents_);
  }

  ElementRange<AudioObject> Document::getElements(
      detail::ParameterTraits<AudioObject>::tag) {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  ElementRange<AudioPackFormat> Document::getElements(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return detail::makeElementRange<AudioPackFormat>(audioPackFormats_);
  }

  ElementRange<AudioChannelFormat> Document::getElements(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    return detail::makeElementRange<AudioChannelFormat>(audioChannelFormats_);
  }

  ElementRange<AudioStreamFormat> Document::getElements(
      detail::ParameterTraits<AudioStreamFormat>::tag) {
    return detail::makeElementRange<AudioStreamFormat>(audioStreamFormats_);
  }

  ElementRange<AudioTrackFormat> Document::getElements(
      detail::ParameterTraits<AudioTrackFormat>::tag) {
    return detail::makeElementRange<AudioTrackFormat>(audioTrackFormats_);
  }

  ElementRange<AudioTrackUid> Document::getElements(
      detail::ParameterTraits<AudioTrackUid>::tag) {
    return detail::makeElementRange<AudioTrackUid>(audioTrackUids_);
  }

  // ---- lookup elements ---- //
  std::shared_ptr<AudioProgramme> Document::lookup(const AudioProgrammeId& id) {
    return lookupById<AudioProgramme>(getElements<AudioProgramme>(), id);
  }
  std::shared_ptr<const AudioProgramme> Document::lookup(
      const AudioProgrammeId& id) const {
    return lookupById<const AudioProgramme>(getElements<AudioProgramme>(), id);
  }

  std::shared_ptr<AudioContent> Document::lookup(const AudioContentId& id) {
    return lookupById<AudioContent>(getElements<AudioContent>(), id);
  }
  std::shared_ptr<const AudioContent> Document::lookup(
      const AudioContentId& id) const {
    return lookupById<const AudioContent>(getElements<AudioContent>(), id);
  }

  std::shared_ptr<AudioObject> Document::lookup(const AudioObjectId& id) {
    return lookupById<AudioObject>(getElements<AudioObject>(), id);
  }
  std::shared_ptr<const AudioObject> Document::lookup(
      const AudioObjectId& id) const {
    return lookupById<const AudioObject>(getElements<AudioObject>(), id);
  }

  std::shared_ptr<AudioPackFormat> Document::lookup(
      const AudioPackFormatId& id) {
    return lookupById<AudioPackFormat>(getElements<AudioPackFormat>(), id);
  }
  std::shared_ptr<const AudioPackFormat> Document::lookup(
      const AudioPackFormatId& id) const {
    return lookupById<const AudioPackFormat>(getElements<AudioPackFormat>(),
                                             id);
  }

  std::shared_ptr<AudioChannelFormat> Document::lookup(
      const AudioChannelFormatId& id) {
    return lookupById<AudioChannelFormat>(getElements<AudioChannelFormat>(),
                                          id);
  }
  std::shared_ptr<const AudioChannelFormat> Document::lookup(
      const AudioChannelFormatId& id) const {
    return lookupById<const AudioChannelFormat>(
        getElements<const AudioChannelFormat>(), id);
  }

  std::shared_ptr<AudioStreamFormat> Document::lookup(
      const AudioStreamFormatId& id) {
    return lookupById<AudioStreamFormat>(getElements<AudioStreamFormat>(), id);
  }
  std::shared_ptr<const AudioStreamFormat> Document::lookup(
      const AudioStreamFormatId& id) const {
    return lookupById<const AudioStreamFormat>(getElements<AudioStreamFormat>(),
                                               id);
  }

  std::shared_ptr<AudioTrackFormat> Document::lookup(
      const AudioTrackFormatId& id) {
    return lookupById<AudioTrackFormat>(getElements<AudioTrackFormat>(), id);
  }
  std::shared_ptr<const AudioTrackFormat> Document::lookup(
      const AudioTrackFormatId& id) const {
    return lookupById<const AudioTrackFormat>(getElements<AudioTrackFormat>(),
                                              id);
  }

  std::shared_ptr<AudioTrackUid> Document::lookup(const AudioTrackUidId& id) {
    return lookupById<AudioTrackUid>(getElements<AudioTrackUid>(), id);
  }
  std::shared_ptr<const AudioTrackUid> Document::lookup(
      const AudioTrackUidId& id) const {
    return lookupById<const AudioTrackUid>(getElements<AudioTrackUid>(), id);
  }

  template <typename Element>
  bool Document::checkParent(const std::shared_ptr<Element> &element, const char *type) {
    auto parentPtr = element->getParent().lock();
    if (parentPtr && parentPtr.get() != this)
      throw std::runtime_error(
          std::string{type} + " already belongs to another Document");
    else
      return static_cast<bool>(parentPtr);
  }

}  // namespace adm
