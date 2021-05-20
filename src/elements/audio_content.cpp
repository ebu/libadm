#include "adm/elements/audio_content.hpp"
#include <algorithm>
#include <functional>
#include "adm/document.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

namespace adm {

  namespace detail {
    template class VectorParameter<LoudnessMetadatas>;
  } // namespace detail

  // ---- Getter ---- //
  AudioContentId AudioContent::get(
      detail::ParameterTraits<AudioContentId>::tag) const {
    return id_;
  }
  AudioContentName AudioContent::get(
      detail::ParameterTraits<AudioContentName>::tag) const {
    return name_;
  }
  AudioContentLanguage AudioContent::get(
      detail::ParameterTraits<AudioContentLanguage>::tag) const {
    return language_.get();
  }
  DialogueId AudioContent::get(detail::ParameterTraits<DialogueId>::tag) const {
    return dialogueId_.get();
  }
  ContentKind AudioContent::get(
      detail::ParameterTraits<ContentKind>::tag) const {
    if (has<NonDialogueContentKind>()) {
      return ContentKind(nonDialogueContentKind_.get());
    } else if (has<DialogueContentKind>()) {
      return ContentKind(dialogueContentKind_.get());
    } else {
      return ContentKind(mixedContentKind_.get());
    }
  }
  NonDialogueContentKind AudioContent::get(
      detail::ParameterTraits<NonDialogueContentKind>::tag) const {
    return nonDialogueContentKind_.get();
  }
  DialogueContentKind AudioContent::get(
      detail::ParameterTraits<DialogueContentKind>::tag) const {
    return dialogueContentKind_.get();
  }
  MixedContentKind AudioContent::get(
      detail::ParameterTraits<MixedContentKind>::tag) const {
    return mixedContentKind_.get();
  }

  // ---- Has ---- //
  bool AudioContent::has(detail::ParameterTraits<AudioContentId>::tag) const {
    return true;
  }
  bool AudioContent::has(detail::ParameterTraits<AudioContentName>::tag) const {
    return true;
  }
  bool AudioContent::has(
      detail::ParameterTraits<AudioContentLanguage>::tag) const {
    return language_ != boost::none;
  }
  bool AudioContent::has(detail::ParameterTraits<DialogueId>::tag) const {
    return dialogueId_ != boost::none;
  }
  bool AudioContent::has(detail::ParameterTraits<ContentKind>::tag) const {
    return nonDialogueContentKind_ != boost::none ||
           dialogueContentKind_ != boost::none ||
           mixedContentKind_ != boost::none;
  }
  bool AudioContent::has(
      detail::ParameterTraits<NonDialogueContentKind>::tag) const {
    return nonDialogueContentKind_ != boost::none;
  }
  bool AudioContent::has(
      detail::ParameterTraits<DialogueContentKind>::tag) const {
    return dialogueContentKind_ != boost::none;
  }
  bool AudioContent::has(detail::ParameterTraits<MixedContentKind>::tag) const {
    return mixedContentKind_ != boost::none;
  }

  // ---- Setter ---- //
  void AudioContent::set(AudioContentId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    id_ = id;
  }
  void AudioContent::set(AudioContentName name) { name_ = name; }
  void AudioContent::set(AudioContentLanguage language) {
    language_ = language;
  }
  void AudioContent::set(DialogueId id) {
    if (dialogueId_ && dialogueId_.get() == id) {
      return;
    }
    unset<DialogueId>();
    dialogueId_ = id;
    if (id == Dialogue::NON_DIALOGUE) {
      nonDialogueContentKind_ = NonDialogueContent::UNDEFINED;
    } else if (id == Dialogue::DIALOGUE) {
      dialogueContentKind_ = DialogueContent::UNDEFINED;
    } else if (id == Dialogue::MIXED) {
      mixedContentKind_ = MixedContent::UNDEFINED;
    }
  }
  void AudioContent::set(ContentKind kind) {
    if (kind.which() == 0) {
      set(boost::get<NonDialogueContentKind>(kind));
    } else if (kind.which() == 1) {
      set(boost::get<DialogueContentKind>(kind));
    } else if (kind.which() == 2) {
      set(boost::get<MixedContentKind>(kind));
    }
  }
  void AudioContent::set(NonDialogueContentKind kind) {
    unset<DialogueId>();
    nonDialogueContentKind_ = kind;
    dialogueId_ = Dialogue::NON_DIALOGUE;
  }
  void AudioContent::set(DialogueContentKind kind) {
    unset<DialogueId>();
    dialogueContentKind_ = kind;
    dialogueId_ = Dialogue::DIALOGUE;
  }
  void AudioContent::set(MixedContentKind kind) {
    unset<DialogueId>();
    mixedContentKind_ = kind;
    dialogueId_ = Dialogue::MIXED;
  }

  // ---- Unsetter ---- //
  void AudioContent::unset(detail::ParameterTraits<AudioContentLanguage>::tag) {
    language_ = boost::none;
  }
  void AudioContent::unset(detail::ParameterTraits<DialogueId>::tag) {
    dialogueId_ = boost::none;
    nonDialogueContentKind_ = boost::none;
    dialogueContentKind_ = boost::none;
    mixedContentKind_ = boost::none;
  }
  void AudioContent::unset(
      detail::ParameterTraits<NonDialogueContentKind>::tag) {
    unset<DialogueId>();
  }
  void AudioContent::unset(detail::ParameterTraits<DialogueContentKind>::tag) {
    unset<DialogueId>();
  }
  void AudioContent::unset(detail::ParameterTraits<MixedContentKind>::tag) {
    unset<DialogueId>();
  }

  // ---- References ---- //
  bool AudioContent::addReference(std::shared_ptr<AudioObject> object) {
    autoParent(shared_from_this(), object);
    if (getParent().lock() != object->getParent().lock()) {
      throw std::runtime_error(
          "AudioContent cannot refer to an AudioObject in a different "
          "document");
    }
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it == audioObjects_.end()) {
      audioObjects_.push_back(object);
      return true;
    } else {
      return false;
    }
  }

  void AudioContent::removeReference(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
    }
  }

  void AudioContent::disconnectReferences() { clearReferences<AudioObject>(); }

  void AudioContent::clearReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return audioObjects_.clear();
  }

  std::ostream& operator<<(std::ostream& stream, const LoudnessMetadatas& loudnessMetaDatas) {
    // Iterate over loudnessMetaDatas
    for (auto i = loudnessMetaDatas.begin(); i != loudnessMetaDatas.end(); ++i)
      stream << *i << ' ';
    return stream;
  }

  // ---- Common ---- //
  void AudioContent::print(std::ostream& os) const {
    os << get<AudioContentId>();
    os << " (audioContentName=";
    os << get<AudioContentName>();
    if (has<AudioContentLanguage>()) {
      os << ", audioContentLanguage=" << get<AudioContentLanguage>();
    }
    if (has<LoudnessMetadatas>()) {
      os << ", loudnessMetadata=" << get<LoudnessMetadatas>();
    }
    if (has<DialogueId>()) {
      os << ", dialogueId=" << get<DialogueId>();
    }
    if (has<ContentKind>()) {
      os << ", contentKind=" << get<ContentKind>();
    }
    if (has<NonDialogueContentKind>()) {
      os << ", nonDialogueContentKind=" << get<NonDialogueContentKind>();
    }
    if (has<DialogueContentKind>()) {
      os << ", dialogueContentKind=" << get<DialogueContentKind>();
    }
    if (has<MixedContentKind>()) {
      os << ", mixedContentKind=" << get<MixedContentKind>();
    }
    os << ")";
  }

  void AudioContent::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }
  std::weak_ptr<Document> AudioContent::getParent() const { return parent_; }

  std::shared_ptr<AudioContent> AudioContent::copy() const {
    auto audioContentCopy =
        std::shared_ptr<AudioContent>(new AudioContent(*this));
    audioContentCopy->setParent(std::weak_ptr<Document>());
    audioContentCopy->disconnectReferences();
    return audioContentCopy;
  }

  AudioContent::AudioContent(AudioContentName name) : name_(name) {}

}  // namespace adm
