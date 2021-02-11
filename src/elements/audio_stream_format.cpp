#include "adm/elements/audio_stream_format.hpp"
#include "adm/document.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

#include <algorithm>

namespace adm {

  // ---- Getter ---- //
  AudioStreamFormatId AudioStreamFormat::get(
      detail::ParameterTraits<AudioStreamFormatId>::tag) const {
    return id_;
  }
  AudioStreamFormatName AudioStreamFormat::get(
      detail::ParameterTraits<AudioStreamFormatName>::tag) const {
    return name_;
  }
  FormatDescriptor AudioStreamFormat::get(
      detail::ParameterTraits<FormatDescriptor>::tag) const {
    return format_;
  }

  // ---- Setter ---- //
  void AudioStreamFormat::set(AudioStreamFormatId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    id_ = id;
  }
  void AudioStreamFormat::set(AudioStreamFormatName name) { name_ = name; }

  // ---- Has ---- //
  bool AudioStreamFormat::has(
      detail::ParameterTraits<AudioStreamFormatId>::tag) const {
    return true;
  }
  bool AudioStreamFormat::has(
      detail::ParameterTraits<AudioStreamFormatName>::tag) const {
    return true;
  }
  bool AudioStreamFormat::has(
      detail::ParameterTraits<FormatDescriptor>::tag) const {
    return true;
  }

  // ---- References ---- //
  void AudioStreamFormat::setReference(
      std::shared_ptr<AudioChannelFormat> channelFormat) {
    autoParent(shared_from_this(), channelFormat);
    if (getParent().lock() != channelFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioStreamFormat cannot refer to an AudioChannelFormat in a "
          "different document");
    }
    audioChannelFormat_ = channelFormat;
  }

  void AudioStreamFormat::setReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    autoParent(shared_from_this(), packFormat);
    if (getParent().lock() != packFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioStreamFormat cannot refer to an AudioPackFormat in a "
          "different document");
    }
    audioPackFormat_ = packFormat;
  }

  bool AudioStreamFormat::addReference(
      std::weak_ptr<AudioTrackFormat> weakTrackFormat,
      ReferenceSyncOption sync) {
    if (sync != ReferenceSyncOption::sync_with_track_format) {
      throw std::logic_error("The given ReferenceSyncOption not implemented.");
    }
    auto trackFormat = weakTrackFormat.lock();
    if (!trackFormat) {
      return false;
    }
    autoParent(shared_from_this(), trackFormat);
    if (getParent().lock() != trackFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioStreamFormat cannot refer to an AudioTrackFormat in a "
          "different document");
    }
    auto it = std::find_if(audioTrackFormats_.begin(), audioTrackFormats_.end(),
                           [&trackFormat](std::weak_ptr<AudioTrackFormat> v) {
                             return v.lock() == trackFormat;
                           });
    if (it == audioTrackFormats_.end()) {
      audioTrackFormats_.push_back(trackFormat);
      trackFormat->setReference(shared_from_this());
      return true;
    } else {
      return false;
    }
  }

  std::shared_ptr<const AudioChannelFormat> AudioStreamFormat::getReference(
      detail::ParameterTraits<AudioChannelFormat>::tag) const {
    return audioChannelFormat_;
  }

  std::shared_ptr<const AudioPackFormat> AudioStreamFormat::getReference(
      detail::ParameterTraits<AudioPackFormat>::tag) const {
    return audioPackFormat_;
  }

  std::shared_ptr<AudioChannelFormat> AudioStreamFormat::getReference(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    return audioChannelFormat_;
  }

  std::shared_ptr<AudioPackFormat> AudioStreamFormat::getReference(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return audioPackFormat_;
  }

  ElementWeakRange<const AudioTrackFormat>
  AudioStreamFormat::getAudioTrackFormatReferences() const {
    return ElementWeakRange<const AudioTrackFormat>(audioTrackFormats_.begin(),
                                                    audioTrackFormats_.end());
  }

  ElementWeakRange<AudioTrackFormat>
  AudioStreamFormat::getAudioTrackFormatReferences() {
    return ElementWeakRange<AudioTrackFormat>(audioTrackFormats_.begin(),
                                              audioTrackFormats_.end());
  }

  void AudioStreamFormat::disconnectReferences() {
    removeReference<AudioChannelFormat>();
    removeReference<AudioPackFormat>();
    clearReferences<AudioTrackFormat>();
  }

  void AudioStreamFormat::removeReference(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    audioChannelFormat_ = nullptr;
  }

  void AudioStreamFormat::removeReference(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    audioPackFormat_ = nullptr;
  }

  void AudioStreamFormat::removeReference(
      std::weak_ptr<AudioTrackFormat> weakTrackFormat,
      ReferenceSyncOption sync) {
    if (sync != ReferenceSyncOption::sync_with_track_format) {
      throw std::logic_error("The given ReferenceSyncOption not implemented.");
    }
    auto trackFormat = weakTrackFormat.lock();
    auto it = std::find_if(audioTrackFormats_.begin(), audioTrackFormats_.end(),
                           [&trackFormat](std::weak_ptr<AudioTrackFormat> p) {
                             auto l = p.lock();
                             return l && trackFormat == l;
                           });

    if (it != audioTrackFormats_.end()) {
      audioTrackFormats_.erase(it);
      trackFormat->removeReference<AudioStreamFormat>();
    }
  }

  void AudioStreamFormat::clearReferences(
      detail::ParameterTraits<AudioTrackFormat>::tag) {
    audioTrackFormats_.clear();
  }

  // ---- Common ---- //
  void AudioStreamFormat::print(std::ostream& os) const {
    os << get<AudioStreamFormatId>();
    os << " (audioStreamFormatName=";
    os << get<AudioStreamFormatName>();
    os << ", formatLabel=" << formatFormatLabel(get<FormatDescriptor>());
    os << ", formatDefinition="
       << formatFormatDefinition(get<FormatDescriptor>());
    os << ")";
  }

  void AudioStreamFormat::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }

  std::weak_ptr<Document> AudioStreamFormat::getParent() const {
    return parent_;
  }

  std::shared_ptr<AudioStreamFormat> AudioStreamFormat::copy() const {
    auto audioStreamFormatCopy =
        std::shared_ptr<AudioStreamFormat>(new AudioStreamFormat(*this));
    audioStreamFormatCopy->setParent(std::weak_ptr<Document>());
    audioStreamFormatCopy->disconnectReferences();
    return audioStreamFormatCopy;
  }

  AudioStreamFormat::AudioStreamFormat(AudioStreamFormatName name,
                                       FormatDescriptor format)
      : name_(name),
        id_(AudioStreamFormatId(TypeDefinition::UNDEFINED)),
        format_(format) {}

}  // namespace adm
