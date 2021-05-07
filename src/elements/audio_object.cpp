#include "adm/elements/audio_object.hpp"
#include "adm/document.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/errors.hpp"

#include <algorithm>

namespace adm {

  // ---- Defaults ---- //
  namespace {
    const Start startDefault{std::chrono::seconds(0)};
  }  // namespace

  // ---- Getter ---- //
  AudioObjectId AudioObject::get(
      detail::ParameterTraits<AudioObjectId>::tag) const {
    return id_;
  }
  AudioObjectName AudioObject::get(
      detail::ParameterTraits<AudioObjectName>::tag) const {
    return name_;
  }
  Start AudioObject::get(detail::ParameterTraits<Start>::tag) const {
    return boost::get_optional_value_or(start_, startDefault);
  }
  Duration AudioObject::get(detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }
  DialogueId AudioObject::get(detail::ParameterTraits<DialogueId>::tag) const {
    return dialogueId_.get();
  }
  Importance AudioObject::get(detail::ParameterTraits<Importance>::tag) const {
    return importance_.get();
  }
  Interact AudioObject::get(detail::ParameterTraits<Interact>::tag) const {
    return interact_.get();
  }
  DisableDucking AudioObject::get(
      detail::ParameterTraits<DisableDucking>::tag) const {
    return disableDucking_.get();
  }
  AudioObjectInteraction AudioObject::get(
      detail::ParameterTraits<AudioObjectInteraction>::tag) const {
    return audioObjectInteraction_.get();
  }
  Gain AudioObject::get(detail::ParameterTraits<Gain>::tag) const {
    return gain_.get();
  }

  // ---- Has ---- //
  bool AudioObject::has(detail::ParameterTraits<AudioObjectId>::tag) const {
    return true;
  }
  bool AudioObject::has(detail::ParameterTraits<AudioObjectName>::tag) const {
    return true;
  }
  bool AudioObject::has(detail::ParameterTraits<Start>::tag) const {
    return true;
  }
  bool AudioObject::has(detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }
  bool AudioObject::has(detail::ParameterTraits<DialogueId>::tag) const {
    return dialogueId_ != boost::none;
  }
  bool AudioObject::has(detail::ParameterTraits<Importance>::tag) const {
    return importance_ != boost::none;
  }
  bool AudioObject::has(detail::ParameterTraits<Interact>::tag) const {
    return interact_ != boost::none;
  }
  bool AudioObject::has(detail::ParameterTraits<DisableDucking>::tag) const {
    return disableDucking_ != boost::none;
  }
  bool AudioObject::has(
      detail::ParameterTraits<AudioObjectInteraction>::tag) const {
    return audioObjectInteraction_ != boost::none;
  }
  bool AudioObject::has(detail::ParameterTraits<Gain>::tag) const {
    return gain_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioObject::isDefault(detail::ParameterTraits<Start>::tag) const {
    return start_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioObject::set(AudioObjectId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    id_ = id;
  }
  void AudioObject::set(AudioObjectName name) { name_ = name; }
  void AudioObject::set(Start start) { start_ = start; }
  void AudioObject::set(Duration duration) { duration_ = duration; }
  void AudioObject::set(DialogueId id) { dialogueId_ = id; }
  void AudioObject::set(Importance importance) { importance_ = importance; }
  void AudioObject::set(Interact interact) { interact_ = interact; }
  void AudioObject::set(DisableDucking disableDucking) {
    disableDucking_ = disableDucking;
  }
  void AudioObject::set(AudioObjectInteraction audioObjectInteraction) {
    audioObjectInteraction_ = audioObjectInteraction;
  }
  void AudioObject::set(Gain gain) { gain_ = gain; }

  // ---- Unsetter ---- //
  void AudioObject::unset(detail::ParameterTraits<Start>::tag) {
    start_ = boost::none;
  }
  void AudioObject::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }
  void AudioObject::unset(detail::ParameterTraits<DialogueId>::tag) {
    dialogueId_ = boost::none;
  }
  void AudioObject::unset(detail::ParameterTraits<Importance>::tag) {
    importance_ = boost::none;
  }
  void AudioObject::unset(detail::ParameterTraits<Interact>::tag) {
    interact_ = boost::none;
  }
  void AudioObject::unset(detail::ParameterTraits<DisableDucking>::tag) {
    disableDucking_ = boost::none;
  }
  void AudioObject::unset(
      detail::ParameterTraits<AudioObjectInteraction>::tag) {
    audioObjectInteraction_ = boost::none;
  }
  void AudioObject::unset(
      detail::ParameterTraits<Gain>::tag) {
    gain_ = boost::none;
  }

  // ---- References ---- //

  bool AudioObject::isAudioObjectReferenceCycle(
      std::shared_ptr<AudioObject> destinationObject) {
    if (shared_from_this() == destinationObject) {
      return true;
    }
    for (auto object : getReferences<AudioObject>()) {
      if (object->isAudioObjectReferenceCycle(destinationObject)) {
        return true;
      }
    }
    return false;
  }

  bool AudioObject::addReference(std::shared_ptr<AudioObject> object) {
    if (object->isAudioObjectReferenceCycle(shared_from_this())) {
      throw error::AudioObjectReferenceCycle(this->get<AudioObjectId>(),
                                             object->get<AudioObjectId>());
    }

    autoParent(shared_from_this(), object);
    if (getParent().lock() != object->getParent().lock()) {
      throw std::runtime_error(
          "AudioObject cannot refer to an AudioObject in a different "
          "document");
    }
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it == audioObjects_.end()) {
      removeComplementary(object);
      audioObjects_.push_back(object);
      return true;
    } else {
      return false;
    }
  }

  bool AudioObject::addReference(std::shared_ptr<AudioPackFormat> packFormat) {
    autoParent(shared_from_this(), packFormat);
    if (getParent().lock() != packFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioObject cannot refer to an AudioPackFormat in a different "
          "document");
    }
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it == audioPackFormats_.end()) {
      audioPackFormats_.push_back(packFormat);
      return true;
    } else {
      return false;
    }
  }

  bool AudioObject::addReference(std::shared_ptr<AudioTrackUid> trackUid) {
    autoParent(shared_from_this(), trackUid);
    if (getParent().lock() != trackUid->getParent().lock()) {
      throw std::runtime_error(
          "AudioObject cannot refer to an AudioTrackUid in a different "
          "document");
    }
    auto it =
        std::find(audioTrackUids_.begin(), audioTrackUids_.end(), trackUid);
    if (it == audioTrackUids_.end()) {
      audioTrackUids_.push_back(trackUid);
      return true;
    } else {
      return false;
    }
  }

  void AudioObject::removeReference(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
    }
  }

  void AudioObject::removeReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it != audioPackFormats_.end()) {
      audioPackFormats_.erase(it);
    }
  }

  void AudioObject::removeReference(std::shared_ptr<AudioTrackUid> trackUid) {
    auto it =
        std::find(audioTrackUids_.begin(), audioTrackUids_.end(), trackUid);
    if (it != audioTrackUids_.end()) {
      audioTrackUids_.erase(it);
    }
  }

  void AudioObject::disconnectReferences() {
    clearReferences<AudioObject>();
    clearReferences<AudioPackFormat>();
    clearReferences<AudioTrackUid>();
    clearComplementaryObjects();
  }

  void AudioObject::clearReferences(detail::ParameterTraits<AudioObject>::tag) {
    return audioObjects_.clear();
  }

  void AudioObject::clearReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return audioPackFormats_.clear();
  }

  void AudioObject::clearReferences(
      detail::ParameterTraits<AudioTrackUid>::tag) {
    return audioTrackUids_.clear();
  }

  // --- ComplementaryObjects ---- //
  bool AudioObject::isComplementaryAudioObjectReferenceCycle(
      std::shared_ptr<AudioObject> destinationObject) {
    if (shared_from_this() == destinationObject) {
      return true;
    }
    for (auto object : getComplementaryObjects()) {
      if (object->isComplementaryAudioObjectReferenceCycle(destinationObject)) {
        return true;
      }
    }
    return false;
  }

  bool AudioObject::addComplementary(std::shared_ptr<AudioObject> object) {
    if (object->isComplementaryAudioObjectReferenceCycle(shared_from_this())) {
      throw error::AudioObjectReferenceCycle(this->get<AudioObjectId>(),
                                             object->get<AudioObjectId>());
    }
    if (getParent().lock() != object->getParent().lock()) {
      throw std::runtime_error(
          "cannot refer to an ADM element in a different document");
    }
    auto it = std::find(audioComplementaryObjects_.begin(),
                        audioComplementaryObjects_.end(), object);
    if (it == audioComplementaryObjects_.end()) {
      removeReference(object);
      audioComplementaryObjects_.push_back(object);
      return true;
    } else {
      return false;
    }
  }

  void AudioObject::removeComplementary(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioComplementaryObjects_.begin(),
                        audioComplementaryObjects_.end(), object);
    if (it != audioComplementaryObjects_.end()) {
      audioComplementaryObjects_.erase(it);
    }
  }

  const std::vector<std::shared_ptr<AudioObject>>&
  AudioObject::getComplementaryObjects() const {
    return audioComplementaryObjects_;
  }

  void AudioObject::clearComplementaryObjects() {
    return audioComplementaryObjects_.clear();
  }

  // ---- Common ---- //
  void AudioObject::print(std::ostream& os) const {
    os << get<AudioObjectId>();
    os << " (audioObjectName=";
    os << get<AudioObjectName>();
    if (has<Start>()) {
      os << ", start=" << formatTimecode(get<Start>().get());
    }
    if (has<Duration>()) {
      os << ", duration=" << formatTimecode(get<Duration>().get());
    }
    if (has<DialogueId>()) {
      os << ", dialogueId=" << get<DialogueId>();
    }
    if (has<Importance>()) {
      os << ", importance=" << get<Importance>();
    }
    if (has<Interact>()) {
      os << ", interact=" << get<Interact>();
    }
    if (has<DisableDucking>()) {
      os << ", disableDucking=" << get<DisableDucking>();
    }
    os << ")";
  }

  void AudioObject::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }

  std::weak_ptr<Document> AudioObject::getParent() const { return parent_; }

  std::shared_ptr<AudioObject> AudioObject::copy() const {
    auto audioObjectCopy = std::shared_ptr<AudioObject>(new AudioObject(*this));
    audioObjectCopy->setParent(std::weak_ptr<Document>());
    audioObjectCopy->disconnectReferences();
    return audioObjectCopy;
  }

  AudioObject::AudioObject(AudioObjectName name) : name_(name) {}

}  // namespace adm
