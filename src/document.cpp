#include "adm/document.hpp"

#include "adm/elements.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/lookup.hpp"
#include "adm/detail/id_assigner.hpp"

#include <algorithm>

namespace adm {
  Document::Document() { idAssigner_.document(this); }

  std::shared_ptr<Document> Document::create() {
    return std::shared_ptr<Document>(new Document());
  }

  std::shared_ptr<Document> Document::deepCopy() const {
    return adm::deepCopy(shared_from_this());
  }

  // ---- add elements ---- //
  bool Document::add(std::shared_ptr<AudioProgramme> programme) {
    if (programme->getParent().lock() &&
        programme->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioProgramme already belongs to another Document");
    }
    auto it =
        std::find(audioProgrammes_.begin(), audioProgrammes_.end(), programme);
    if (it == audioProgrammes_.end()) {
      idAssigner_.assignId(*programme);
      AudioProgrammeAttorney::setParent(programme, shared_from_this());
      audioProgrammes_.push_back(programme);
      for (auto& reference : programme->getReferences<AudioContent>()) {
        add(reference);
      }
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioContent> content) {
    if (content->getParent().lock() &&
        content->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioContent already belongs to another Document");
    }
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it == audioContents_.end()) {
      idAssigner_.assignId(*content);
      AudioContentAttorney::setParent(content, shared_from_this());
      audioContents_.push_back(content);
      for (auto& reference : content->getReferences<AudioObject>()) {
        add(reference);
      }
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioObject> object) {
    if (object->getParent().lock() &&
        object->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioObject already belongs to another Document");
    }
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it == audioObjects_.end()) {
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
    if (packFormat->getParent().lock() &&
        packFormat->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioPackFormat already belongs to another Document");
    }
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it == audioPackFormats_.end()) {
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
    if (channelFormat->getParent().lock() &&
        channelFormat->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioChannelFormat already belongs to another Document");
    }
    auto it = std::find(audioChannelFormats_.begin(),
                        audioChannelFormats_.end(), channelFormat);
    if (it == audioChannelFormats_.end()) {
      idAssigner_.assignId(*channelFormat);
      AudioChannelFormatAttorney::setParent(channelFormat, shared_from_this());
      audioChannelFormats_.push_back(channelFormat);
      return true;
    } else {
      return false;
    }
  }

  bool Document::add(std::shared_ptr<AudioStreamFormat> streamFormat) {
    if (streamFormat->getParent().lock() &&
        streamFormat->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioStreamFormat already belongs to another Document");
    }
    auto it = std::find(audioStreamFormats_.begin(), audioStreamFormats_.end(),
                        streamFormat);
    if (it == audioStreamFormats_.end()) {
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
    if (trackFormat->getParent().lock() &&
        trackFormat->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioTrackFormat already belongs to another Document");
    }
    auto it = std::find(audioTrackFormats_.begin(), audioTrackFormats_.end(),
                        trackFormat);
    if (it == audioTrackFormats_.end()) {
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
    if (trackUid->getParent().lock() &&
        trackUid->getParent().lock() != shared_from_this()) {
      throw std::runtime_error(
          "AudioTrackUid already belongs to another Document");
    }
    auto it =
        std::find(audioTrackUids_.begin(), audioTrackUids_.end(), trackUid);
    if (it == audioTrackUids_.end()) {
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
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioContent> content) {
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it != audioContents_.end()) {
      audioContents_.erase(it);
      for (auto& audioProgramme : audioProgrammes_) {
        audioProgramme->removeReference(content);
      }
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
      for (auto& audioObject : audioObjects_) {
        audioObject->removeReference(object);
      }
      for (auto& audioContent : audioContents_) {
        audioContent->removeReference(object);
      }
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioPackFormat> packFormat) {
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it != audioPackFormats_.end()) {
      audioPackFormats_.erase(it);
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
      return true;
    }
    return false;
  }

  bool Document::remove(std::shared_ptr<AudioChannelFormat> channelFormat) {
    auto it = std::find(audioChannelFormats_.begin(),
                        audioChannelFormats_.end(), channelFormat);
    if (it != audioChannelFormats_.end()) {
      audioChannelFormats_.erase(it);
      for (auto& audioPackFormat : audioPackFormats_) {
        audioPackFormat->removeReference(channelFormat);
      }
      for (auto& audioStreamFormat : audioStreamFormats_) {
        if (audioStreamFormat->getReference<AudioChannelFormat>() ==
            channelFormat) {
          audioStreamFormat->removeReference<AudioChannelFormat>();
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

}  // namespace adm
