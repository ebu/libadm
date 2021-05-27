#include "adm/elements/audio_programme.hpp"
#include "adm/document.hpp"
#include "adm/elements/time.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

#include <algorithm>

namespace adm {

  // ---- Defaults ---- //
  namespace {
    const Start startDefault{std::chrono::seconds(0)};
  }  // namespace

  // ---- Getter ---- //
  AudioProgrammeId AudioProgramme::get(
      detail::ParameterTraits<AudioProgrammeId>::tag) const {
    return id_;
  }
  AudioProgrammeName AudioProgramme::get(
      detail::ParameterTraits<AudioProgrammeName>::tag) const {
    return name_;
  }
  AudioProgrammeLanguage AudioProgramme::get(
      detail::ParameterTraits<AudioProgrammeLanguage>::tag) const {
    return language_.get();
  }
  Start AudioProgramme::get(detail::ParameterTraits<Start>::tag) const {
    return boost::get_optional_value_or(start_, startDefault);
  }
  End AudioProgramme::get(detail::ParameterTraits<End>::tag) const {
    return end_.get();
  }
  MaxDuckingDepth AudioProgramme::get(
      detail::ParameterTraits<MaxDuckingDepth>::tag) const {
    return maxDuckingDepth_.get();
  }
  AudioProgrammeReferenceScreen AudioProgramme::get(
      detail::ParameterTraits<AudioProgrammeReferenceScreen>::tag) const {
    return refScreen_.get();
  }

  // ---- Has ---- //
  bool AudioProgramme::has(
      detail::ParameterTraits<AudioProgrammeId>::tag) const {
    return true;
  }
  bool AudioProgramme::has(
      detail::ParameterTraits<AudioProgrammeName>::tag) const {
    return true;
  }
  bool AudioProgramme::has(
      detail::ParameterTraits<AudioProgrammeLanguage>::tag) const {
    return language_ != boost::none;
  }
  bool AudioProgramme::has(detail::ParameterTraits<Start>::tag) const {
    return true;
  }
  bool AudioProgramme::has(detail::ParameterTraits<End>::tag) const {
    return end_ != boost::none;
  }
  bool AudioProgramme::has(
      detail::ParameterTraits<MaxDuckingDepth>::tag) const {
    return maxDuckingDepth_ != boost::none;
  }
  bool AudioProgramme::has(
      detail::ParameterTraits<AudioProgrammeReferenceScreen>::tag) const {
    return refScreen_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioProgramme::isDefault(detail::ParameterTraits<Start>::tag) const {
    return start_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioProgramme::set(AudioProgrammeId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    id_ = id;
  }

  void AudioProgramme::set(AudioProgrammeName name) { name_ = name; }
  void AudioProgramme::set(AudioProgrammeLanguage language) {
    language_ = language;
  }
  void AudioProgramme::set(Start start) { start_ = start; }
  void AudioProgramme::set(End end) { end_ = end; }
  void AudioProgramme::set(MaxDuckingDepth depth) { maxDuckingDepth_ = depth; }
  void AudioProgramme::set(AudioProgrammeReferenceScreen refScreen) {
    refScreen_ = refScreen;
  }

  // ---- Unsetter ---- //
  void AudioProgramme::unset(
      detail::ParameterTraits<AudioProgrammeLanguage>::tag) {
    language_ = boost::none;
  }
  void AudioProgramme::unset(detail::ParameterTraits<Start>::tag) {
    start_ = boost::none;
  }
  void AudioProgramme::unset(detail::ParameterTraits<End>::tag) {
    end_ = boost::none;
  }
  void AudioProgramme::unset(detail::ParameterTraits<MaxDuckingDepth>::tag) {
    maxDuckingDepth_ = boost::none;
  }
  void AudioProgramme::unset(
      detail::ParameterTraits<AudioProgrammeReferenceScreen>::tag) {
    refScreen_ = boost::none;
  }

  // ---- References ---- //
  bool AudioProgramme::addReference(std::shared_ptr<AudioContent> content) {
    autoParent(shared_from_this(), content);
    if (getParent().lock() != content->getParent().lock()) {
      throw std::runtime_error(
          "AudioProgramme cannot refer to an AudioContent in a different "
          "document");
    }
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it == audioContents_.end()) {
      audioContents_.push_back(content);
      return true;
    } else {
      return false;
    }
  }

  void AudioProgramme::removeReference(std::shared_ptr<AudioContent> content) {
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it != audioContents_.end()) {
      audioContents_.erase(it);
    }
  }

  void AudioProgramme::disconnectReferences() {
    clearReferences<AudioContent>();
  }

  void AudioProgramme::clearReferences(
      detail::ParameterTraits<AudioContent>::tag) {
    audioContents_.clear();
  }

  // ---- Common ---- //
  void AudioProgramme::print(std::ostream& os) const {
    os << get<AudioProgrammeId>();
    os << " (audioProgrammeName=";
    os << get<AudioProgrammeName>();
    if (has<AudioProgrammeLanguage>()) {
      os << ", audioProgrammeLanguage=" << get<AudioProgrammeLanguage>();
    }
    if (has<Start>()) {
      os << ", start=" << formatTimecode(get<Start>().get());
    }
    if (has<End>()) {
      os << ", end=" << formatTimecode(get<End>().get());
    }
    if (has<LoudnessMetadatas>()) {
      os << ", loudnessMetadata=" << get<LoudnessMetadatas>();
    }
    if (has<MaxDuckingDepth>()) {
      os << ", maxDuckingDepth=" << get<MaxDuckingDepth>();
    }
    if (has<AudioProgrammeReferenceScreen>()) {
      os << ", audioProgrammeReferenceScreen="
         << get<AudioProgrammeReferenceScreen>();
    }
    os << ")";
  }

  void AudioProgramme::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }
  std::weak_ptr<Document> AudioProgramme::getParent() const { return parent_; };

  std::shared_ptr<AudioProgramme> AudioProgramme::copy() const {
    auto audioProgrammeCopy =
        std::shared_ptr<AudioProgramme>(new AudioProgramme(*this));
    audioProgrammeCopy->setParent(std::weak_ptr<Document>());
    audioProgrammeCopy->disconnectReferences();
    return audioProgrammeCopy;
  }

  AudioProgramme::AudioProgramme(AudioProgrammeName name) : name_(name){};
}  // namespace adm
