#include "adm/serialized/audio_track.hpp"
#include "adm/utilities/element_io.hpp"

namespace adm {

  // ---- Getter ---- //
  TrackId AudioTrack::get(detail::ParameterTraits<TrackId>::tag) const {
    return trackId_;
  }
  FormatDescriptor AudioTrack::get(
      detail::ParameterTraits<FormatDescriptor>::tag) const {
    return format_.get();
  }

  // ---- Setter ---- //
  void AudioTrack::set(TrackId trackId) { trackId_ = trackId; }
  void AudioTrack::set(FormatDescriptor format) { format_ = format; }

  // ---- Has ---- //
  bool AudioTrack::has(detail::ParameterTraits<FormatDescriptor>::tag) const {
    return format_ != boost::none;
  }

  void AudioTrack::unset(detail::ParameterTraits<FormatDescriptor>::tag) {
    format_ = boost::none;
  }

  // ---- AudioTrackUidIds ---- //
  void AudioTrack::add(const AudioTrackUidId& audioTrackUidId) {
    audioTrackUidIds_.push_back(audioTrackUidId);
  }

  AudioTrackUidIdConstRange AudioTrack::audioTrackUidIds() const {
    return boost::make_iterator_range(audioTrackUidIds_.cbegin(),
                                      audioTrackUidIds_.cend());
  }

  AudioTrackUidIdRange AudioTrack::audioTrackUidIds() {
    return boost::make_iterator_range(audioTrackUidIds_.begin(),
                                      audioTrackUidIds_.end());
  }

  void AudioTrack::clearAudioTrackUidIds() { audioTrackUidIds_.clear(); }

  // ---- Common ---- //
  void AudioTrack::print(std::ostream& os) const {
    os << get<TrackId>();
    os << ", formatLabel=" << formatFormatLabel(get<FormatDescriptor>());
    os << ", formatDefinition="
       << formatFormatDefinition(get<FormatDescriptor>());
    os << ")";
  }

}  // namespace adm
