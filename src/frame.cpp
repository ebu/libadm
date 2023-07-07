#include <iostream>

#include "adm/frame.hpp"

#include <algorithm>
#include "adm/serial/frame_header.hpp"
#include "adm/serial/frame_format.hpp"
#include "adm/serial/transport_track_format.hpp"
#include "adm/elements/private/parent_attorneys.hpp"

namespace adm {
  Frame::Frame(FrameStart start, FrameDuration duration, FrameType frameType, FrameFormatId frameFormatId)
      : frameHeader_(FrameHeader(start, duration, frameType, frameFormatId)) {
    audioFormatExtended_ = Document::create();
  }

  Frame::Frame(FrameHeader frameHeader) : frameHeader_(frameHeader) {
    audioFormatExtended_ = Document::create();
  }

  std::shared_ptr<Frame> Frame::create(FrameStart start, FrameDuration duration,
                                       FrameType frameType, FrameFormatId frameFormatId) {
    return std::shared_ptr<Frame>(new Frame(start, duration, frameType, frameFormatId));
  }

  std::shared_ptr<Frame> Frame::create(FrameHeader frameHeader) {
    auto frame = std::shared_ptr<Frame>(new Frame(frameHeader));
    
    // Generate the ATU_00000000 audioTrackUID and add to frame
    auto audioTrackUid_zero = AudioTrackUid::create(AudioTrackUidIdValue(0));
    frame->add(audioTrackUid_zero);
    
    return frame;
  }

  FrameStart Frame::start() const {
    return frameHeader().frameFormat().get<adm::FrameStart>();
  }

  FrameDuration Frame::duration() const {
    return frameHeader().frameFormat().get<adm::FrameDuration>();
  }

  std::shared_ptr<Frame> Frame::deepCopy() const {
    auto copy = Frame::create(frameHeader());
    copy->audioFormatExtended_ = audioFormatExtended_->deepCopy();
    return copy;
  }

  const FrameHeader& Frame::frameHeader() const { return frameHeader_; }
  FrameHeader& Frame::frameHeader() { return frameHeader_; }
  void Frame::setFrameHeader(FrameHeader frameHeader) { frameHeader_ = frameHeader; }

  
  // ---- add elements ---- //
  bool Frame::add(std::shared_ptr<AudioProgramme> programme) {
    return audioFormatExtended_->add(programme);
  }

  bool Frame::add(std::shared_ptr<AudioContent> content) {
    return audioFormatExtended_->add(content);
  }

  bool Frame::add(std::shared_ptr<AudioObject> object) {
    return audioFormatExtended_->add(object);
  }

  bool Frame::add(std::shared_ptr<AudioPackFormat> packFormat) {
    return audioFormatExtended_->add(packFormat);
  }

  bool Frame::add(std::shared_ptr<AudioChannelFormat> channelFormat) {
    return audioFormatExtended_->add(channelFormat);
  }

  bool Frame::add(std::shared_ptr<AudioStreamFormat> streamFormat) {
    return audioFormatExtended_->add(streamFormat);
  }

  bool Frame::add(std::shared_ptr<AudioTrackFormat> trackFormat) {
    return audioFormatExtended_->add(trackFormat);
  }

  bool Frame::add(std::shared_ptr<AudioTrackUid> trackUid) {
    return audioFormatExtended_->add(trackUid);
  }

  // ---- remove elements --- //
  bool Frame::remove(std::shared_ptr<AudioProgramme> programme) {
    return audioFormatExtended_->remove(programme);
  }

  bool Frame::remove(std::shared_ptr<AudioContent> content) {
    return audioFormatExtended_->remove(content);
  }

  bool Frame::remove(std::shared_ptr<AudioObject> object) {
    return audioFormatExtended_->remove(object);
  }

  bool Frame::remove(std::shared_ptr<AudioPackFormat> packFormat) {
    return audioFormatExtended_->remove(packFormat);
  }

  bool Frame::remove(std::shared_ptr<AudioChannelFormat> channelFormat) {
    return audioFormatExtended_->remove(channelFormat);
  }

  bool Frame::remove(std::shared_ptr<AudioStreamFormat> streamFormat) {
    return audioFormatExtended_->remove(streamFormat);
  }

  bool Frame::remove(std::shared_ptr<AudioTrackFormat> trackFormat) {
    return audioFormatExtended_->remove(trackFormat);
  }

  bool Frame::remove(std::shared_ptr<AudioTrackUid> trackUid) {
    return audioFormatExtended_->remove(trackUid);
  }
  
  
  // // ---- get elements ---- //
  // ElementRange<const AudioProgramme> Frame::getElements(
  //     detail::ParameterTraits<AudioProgramme>::tag) const {
  //   return audioFormatExtended_->getElements<AudioProgramme>();
  // }
  //
  // ElementRange<const AudioContent> Frame::getElements(
  //     detail::ParameterTraits<AudioContent>::tag) const {
  //   return audioFormatExtended_->getElements<AudioContent>();
  // }
  //
  // ElementRange<const AudioObject> Frame::getElements(
  //     detail::ParameterTraits<AudioObject>::tag) const {
  //   return audioFormatExtended_->getElements<AudioObject>();
  // }
  //
  // ElementRange<const AudioPackFormat> Frame::getElements(
  //     detail::ParameterTraits<AudioPackFormat>::tag) const {
  //   return audioFormatExtended_->getElements<AudioPackFormat>();
  // }
  //
  // ElementRange<const AudioChannelFormat>
  // Frame::getElements(
  //     detail::ParameterTraits<AudioChannelFormat>::tag) const {
  //   return audioFormatExtended_->getElements<AudioChannelFormat>();
  // }
  //
  // ElementRange<const AudioStreamFormat>
  // Frame::getElements(
  //     detail::ParameterTraits<AudioStreamFormat>::tag) const {
  //   return audioFormatExtended_->getElements<AudioStreamFormat>();
  // }
  //
  // ElementRange<const AudioTrackFormat> Frame::getElements(
  //     detail::ParameterTraits<AudioTrackFormat>::tag) const {
  //   return audioFormatExtended_->getElements<AudioTrackFormat>();
  // }
  //
  // ElementRange<const AudioTrackUid> Frame::getElements(
  //     detail::ParameterTraits<AudioTrackUid>::tag) const {
  //   return audioFormatExtended_->getElements<AudioTrackUid>();
  // }

}  // namespace adm
