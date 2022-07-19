#include "adm/document.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/errors.hpp"

namespace adm {

  // ---- Getter ---- //
  AudioTrackUidId AudioTrackUid::get(
      detail::ParameterTraits<AudioTrackUidId>::tag) const {
    return id_;
  }
  SampleRate AudioTrackUid::get(
      detail::ParameterTraits<SampleRate>::tag) const {
    return sampleRate_.get();
  }
  BitDepth AudioTrackUid::get(detail::ParameterTraits<BitDepth>::tag) const {
    return bitDepth_.get();
  }

  // ---- Setter ---- //
  void AudioTrackUid::set(AudioTrackUidId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    id_ = id;
  }
  void AudioTrackUid::set(SampleRate sampleRate) { sampleRate_ = sampleRate; }
  void AudioTrackUid::set(BitDepth bitDepth) { bitDepth_ = bitDepth; }

  // ---- Has ---- //
  bool AudioTrackUid::has(detail::ParameterTraits<AudioTrackUidId>::tag) const {
    return true;
  }
  bool AudioTrackUid::has(detail::ParameterTraits<BitDepth>::tag) const {
    return bitDepth_ != boost::none;
  }
  bool AudioTrackUid::has(detail::ParameterTraits<SampleRate>::tag) const {
    return sampleRate_ != boost::none;
  }

  // ---- Unsetter ---- //
  void AudioTrackUid::unset(detail::ParameterTraits<BitDepth>::tag) {
    bitDepth_ = boost::none;
  }
  void AudioTrackUid::unset(detail::ParameterTraits<SampleRate>::tag) {
    sampleRate_ = boost::none;
  }

  // ---- References ---- //
  void AudioTrackUid::setReference(
      std::shared_ptr<AudioTrackFormat> trackFormat) {
    autoParent(shared_from_this(), trackFormat);
    if (getParent().lock() != trackFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioTrackUid cannot refer to an AudioTrackFormat in a different "
          "document");
    }

    if (audioChannelFormat_ != nullptr) {
      throw adm::error::AudioTrackUidMutuallyExclusiveReferences(
          audioChannelFormat_->get<AudioChannelFormatId>(),
          trackFormat->get<AudioTrackFormatId>());
    }

    audioTrackFormat_ = trackFormat;
  }

  void AudioTrackUid::setReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    autoParent(shared_from_this(), packFormat);
    if (getParent().lock() != packFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioTrackUid cannot refer to an AudioPackFormat in a different "
          "document");
    }
    audioPackFormat_ = packFormat;
  }

  void AudioTrackUid::setReference(
      std::shared_ptr<AudioChannelFormat> channelFormat) {
    autoParent(shared_from_this(), channelFormat);
    if (getParent().lock() != channelFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioTrackUid cannot refer to an AudioChannelFormat in a different "
          "document");
    }

    if (audioTrackFormat_ != nullptr) {
      throw adm::error::AudioTrackUidMutuallyExclusiveReferences(
          channelFormat->get<AudioChannelFormatId>(),
          audioTrackFormat_->get<AudioTrackFormatId>());
    }

    audioChannelFormat_ = channelFormat;
  }

  std::shared_ptr<const AudioTrackFormat> AudioTrackUid::getReference(
      detail::ParameterTraits<AudioTrackFormat>::tag) const {
    return audioTrackFormat_;
  }

  std::shared_ptr<const AudioChannelFormat> AudioTrackUid::getReference(
      detail::ParameterTraits<AudioChannelFormat>::tag) const {
    return audioChannelFormat_;
  }

  std::shared_ptr<const AudioPackFormat> AudioTrackUid::getReference(
      detail::ParameterTraits<AudioPackFormat>::tag) const {
    return audioPackFormat_;
  }

  std::shared_ptr<AudioTrackFormat> AudioTrackUid::getReference(
      detail::ParameterTraits<AudioTrackFormat>::tag) {
    return audioTrackFormat_;
  }

  std::shared_ptr<AudioChannelFormat> AudioTrackUid::getReference(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    return audioChannelFormat_;
  }

  std::shared_ptr<AudioPackFormat> AudioTrackUid::getReference(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    return audioPackFormat_;
  }

  void AudioTrackUid::disconnectReferences() {
    removeReference<AudioTrackFormat>();
    removeReference<AudioChannelFormat>();
    removeReference<AudioPackFormat>();
  }

  void AudioTrackUid::removeReference(
      detail::ParameterTraits<AudioTrackFormat>::tag) {
    audioTrackFormat_ = nullptr;
  }

  void AudioTrackUid::removeReference(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    audioChannelFormat_ = nullptr;
  }

  void AudioTrackUid::removeReference(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    audioPackFormat_ = nullptr;
  }

  // ---- Common ---- //
  void AudioTrackUid::print(std::ostream& os) const {
    os << get<AudioTrackUidId>();
    os << " (";
    if (has<SampleRate>()) {
      os << "sampleRate=" << get<SampleRate>() << ", ";
    }
    if (has<BitDepth>()) {
      os << "bitDepth=" << get<BitDepth>();
    }

    os << ")";
  }

  void AudioTrackUid::setParent(std::weak_ptr<Document> document) {
    parent_ = std::move(document);
  }

  std::weak_ptr<Document> AudioTrackUid::getParent() const { return parent_; }

  std::shared_ptr<AudioTrackUid> AudioTrackUid::copy() const {
    auto audioTrackUidCopy =
        std::shared_ptr<AudioTrackUid>(new AudioTrackUid(*this));
    audioTrackUidCopy->setParent(std::weak_ptr<Document>());
    audioTrackUidCopy->disconnectReferences();
    return audioTrackUidCopy;
  }

  AudioTrackUid::AudioTrackUid() {}

}  // namespace adm
