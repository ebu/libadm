#include "adm/elements/audio_track_format.hpp"
#include "adm/document.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

namespace adm {

  // ---- Getter ---- //
  AudioTrackFormatId AudioTrackFormat::get(
      detail::ParameterTraits<AudioTrackFormatId>::tag) const {
    return id_;
  }
  AudioTrackFormatName AudioTrackFormat::get(
      detail::ParameterTraits<AudioTrackFormatName>::tag) const {
    return name_;
  }
  FormatDescriptor AudioTrackFormat::get(
      detail::ParameterTraits<FormatDescriptor>::tag) const {
    return format_;
  }

  // ---- Setter ---- //
  void AudioTrackFormat::set(AudioTrackFormatId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    id_ = id;
  }
  void AudioTrackFormat::set(AudioTrackFormatName name) {
    name_ = std::move(name);
  }

  // ---- Has ---- //
  bool AudioTrackFormat::has(
      detail::ParameterTraits<AudioTrackFormatId>::tag) const {
    return true;
  }
  bool AudioTrackFormat::has(
      detail::ParameterTraits<AudioTrackFormatName>::tag) const {
    return true;
  }
  bool AudioTrackFormat::has(
      detail::ParameterTraits<FormatDescriptor>::tag) const {
    return true;
  }

  // ---- References ---- //
  void AudioTrackFormat::setReference(
      std::shared_ptr<AudioStreamFormat> streamFormat,
      ReferenceSyncOption sync) {
    if (sync != ReferenceSyncOption::sync_with_stream_format) {
      throw std::logic_error("The given ReferenceSyncOption not implemented.");
    }
    if (streamFormat == audioStreamFormat_) {
      return;
    }
    if (!autoParent(*this, streamFormat)) {
      throw std::runtime_error(
          "AudioTrackFormat cannot refer to an AudioStreamFormat in a "
          "different document");
    }

    removeReference<AudioStreamFormat>();
    audioStreamFormat_ = std::move(streamFormat);
    if (audioStreamFormat_) {
      audioStreamFormat_->addReference(
          std::weak_ptr<AudioTrackFormat>(shared_from_this()));
    }
  }

  std::shared_ptr<const AudioStreamFormat> AudioTrackFormat::getReference(
      detail::ParameterTraits<AudioStreamFormat>::tag) const {
    return audioStreamFormat_;
  }

  std::shared_ptr<AudioStreamFormat> AudioTrackFormat::getReference(
      detail::ParameterTraits<AudioStreamFormat>::tag) {
    return audioStreamFormat_;
  }

  void AudioTrackFormat::disconnectReferences() {
    removeReference<AudioStreamFormat>();
  }

  void AudioTrackFormat::removeReference(
      detail::ParameterTraits<AudioStreamFormat>::tag,
      ReferenceSyncOption sync) {
    if (sync != ReferenceSyncOption::sync_with_stream_format) {
      throw std::logic_error("The given ReferenceSyncOption not implemented.");
    }
    if (audioStreamFormat_) {
      // remove from this first, to avoid  cyclic reference removement calls
      auto tmp = audioStreamFormat_;
      audioStreamFormat_.reset();
      if (tmp) {
        tmp->removeReference(shared_from_this());
      }
    }
  }

  // ---- Common ---- //
  void AudioTrackFormat::print(std::ostream& os) const {
    os << get<AudioTrackFormatId>();
    os << " (audioTrackFormatName=";
    os << get<AudioTrackFormatName>();
    os << ", formatLabel=" << formatFormatLabel(get<FormatDescriptor>());
    os << ", formatDefinition="
       << formatFormatDefinition(get<FormatDescriptor>());
    os << ")";
  }

  void AudioTrackFormat::setParent(std::weak_ptr<Document> document) {
    parent_ = std::move(document);
  }

  const std::weak_ptr<Document>& AudioTrackFormat::getParent() const {
    return parent_;
  }

  std::shared_ptr<AudioTrackFormat> AudioTrackFormat::copy() const {
    auto audioTrackFormatCopy =
        std::shared_ptr<AudioTrackFormat>(new AudioTrackFormat(*this));
    audioTrackFormatCopy->setParent(std::weak_ptr<Document>());
    audioTrackFormatCopy->disconnectReferences();
    return audioTrackFormatCopy;
  }

  AudioTrackFormat::AudioTrackFormat(AudioTrackFormatName name,
                                     FormatDescriptor format)
      : name_(std::move(name)),
        id_(AudioTrackFormatId(TypeDefinition::UNDEFINED)),
        format_(format) {}

}  // namespace adm
