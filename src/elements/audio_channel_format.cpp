#include "adm/elements/audio_channel_format.hpp"
#include <algorithm>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_block_format_binaural.hpp"
#include "adm/elements/audio_block_format_direct_speakers.hpp"
#include "adm/elements/audio_block_format_hoa.hpp"
#include "adm/elements/audio_block_format_matrix.hpp"
#include "adm/elements/audio_block_format_objects.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/comparator.hpp"

namespace adm {

  // ---- Getter ---- //
  AudioChannelFormatId AudioChannelFormat::get(
      detail::ParameterTraits<AudioChannelFormatId>::tag) const {
    return id_;
  }
  AudioChannelFormatName AudioChannelFormat::get(
      detail::ParameterTraits<AudioChannelFormatName>::tag) const {
    return name_;
  }
  TypeDescriptor AudioChannelFormat::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return typeDescriptor_;
  }
  Frequency AudioChannelFormat::get(
      detail::ParameterTraits<Frequency>::tag) const {
    if (has<Frequency>()) {
      return frequency_.get();
    } else {
      throw std::runtime_error("optional value for Frequency not set");
    }
  }

  // ---- Setter ---- //
  void AudioChannelFormat::set(AudioChannelFormatId id) {
    if (isUndefined(id)) {
      id_ = id;
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    if (id.get<TypeDescriptor>() == get<TypeDescriptor>()) {
      id_ = id;
      assignNewIdValue<AudioBlockFormatDirectSpeakers>();
      assignNewIdValue<AudioBlockFormatMatrix>();
      assignNewIdValue<AudioBlockFormatObjects>();
      assignNewIdValue<AudioBlockFormatHoa>();
      assignNewIdValue<AudioBlockFormatBinaural>();
    } else {
      std::stringstream errorString;
      errorString << "missmatch between TypeDefinition of AudioChannelFormat ("
                  << formatTypeDefinition(get<TypeDescriptor>())
                  << ") and AudioChannelFormatId ("
                  << formatTypeDefinition(id.get<TypeDescriptor>()) << ")";
      throw std::runtime_error(errorString.str());
    }
  }
  void AudioChannelFormat::set(AudioChannelFormatName name) { name_ = name; }
  void AudioChannelFormat::set(Frequency frequency) { frequency_ = frequency; }

  // ---- Has ---- //
  bool AudioChannelFormat::has(
      detail::ParameterTraits<AudioChannelFormatId>::tag) const {
    return true;
  }
  bool AudioChannelFormat::has(
      detail::ParameterTraits<AudioChannelFormatName>::tag) const {
    return true;
  }
  bool AudioChannelFormat::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioChannelFormat::has(detail::ParameterTraits<Frequency>::tag) const {
    return frequency_ != boost::none;
  }

  // ---- Unsetter ---- //
  void AudioChannelFormat::unset(detail::ParameterTraits<Frequency>::tag) {
    frequency_ = boost::none;
  }

  // ---- AudioBlocks ---- //
  void AudioChannelFormat::add(AudioBlockFormatDirectSpeakers blockFormat) {
    if (audioBlockFormatsDirectSpeakers_.empty()) {
      assignId(blockFormat);
    } else {
      assignId(blockFormat, &audioBlockFormatsDirectSpeakers_.back());
    }
    audioBlockFormatsDirectSpeakers_.push_back(blockFormat);
  }
  void AudioChannelFormat::add(AudioBlockFormatMatrix blockFormat) {
    if (audioBlockFormatsMatrix_.empty()) {
      assignId(blockFormat);
    } else {
      assignId(blockFormat, &audioBlockFormatsMatrix_.back());
    }
    audioBlockFormatsMatrix_.push_back(blockFormat);
  }

  void AudioChannelFormat::add(AudioBlockFormatObjects blockFormat) {
    if (audioBlockFormatsObjects_.empty()) {
      assignId(blockFormat);
    } else {
      assignId(blockFormat, &audioBlockFormatsObjects_.back());
    }
    audioBlockFormatsObjects_.push_back(blockFormat);
  }

  void AudioChannelFormat::add(AudioBlockFormatHoa blockFormat) {
    if (audioBlockFormatsHoa_.empty()) {
      assignId(blockFormat);
    } else {
      assignId(blockFormat, &audioBlockFormatsHoa_.back());
    }
    audioBlockFormatsHoa_.push_back(blockFormat);
  }

  void AudioChannelFormat::add(AudioBlockFormatBinaural blockFormat) {
    if (audioBlockFormatsBinaural_.empty()) {
      assignId(blockFormat);
    } else {
      assignId(blockFormat, &audioBlockFormatsBinaural_.back());
    }
    audioBlockFormatsBinaural_.push_back(blockFormat);
  }

  BlockFormatsConstRange<AudioBlockFormatDirectSpeakers>
  AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatDirectSpeakers>::tag) const {
    return boost::make_iterator_range(audioBlockFormatsDirectSpeakers_.begin(),
                                      audioBlockFormatsDirectSpeakers_.end());
  }
  BlockFormatsConstRange<AudioBlockFormatMatrix> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatMatrix>::tag) const {
    return boost::make_iterator_range(audioBlockFormatsMatrix_.begin(),
                                      audioBlockFormatsMatrix_.end());
  }
  BlockFormatsConstRange<AudioBlockFormatObjects> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatObjects>::tag) const {
    return boost::make_iterator_range(audioBlockFormatsObjects_.begin(),
                                      audioBlockFormatsObjects_.end());
  }
  BlockFormatsConstRange<AudioBlockFormatHoa> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatHoa>::tag) const {
    return boost::make_iterator_range(audioBlockFormatsHoa_.begin(),
                                      audioBlockFormatsHoa_.end());
  }
  BlockFormatsConstRange<AudioBlockFormatBinaural> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatBinaural>::tag) const {
    return boost::make_iterator_range(audioBlockFormatsBinaural_.begin(),
                                      audioBlockFormatsBinaural_.end());
  }

  BlockFormatsRange<AudioBlockFormatDirectSpeakers> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatDirectSpeakers>::tag) {
    return boost::make_iterator_range(audioBlockFormatsDirectSpeakers_.begin(),
                                      audioBlockFormatsDirectSpeakers_.end());
  }
  BlockFormatsRange<AudioBlockFormatMatrix> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatMatrix>::tag) {
    return boost::make_iterator_range(audioBlockFormatsMatrix_.begin(),
                                      audioBlockFormatsMatrix_.end());
  }
  BlockFormatsRange<AudioBlockFormatObjects> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatObjects>::tag) {
    return boost::make_iterator_range(audioBlockFormatsObjects_.begin(),
                                      audioBlockFormatsObjects_.end());
  }
  BlockFormatsRange<AudioBlockFormatHoa> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatHoa>::tag) {
    return boost::make_iterator_range(audioBlockFormatsHoa_.begin(),
                                      audioBlockFormatsHoa_.end());
  }
  BlockFormatsRange<AudioBlockFormatBinaural> AudioChannelFormat::get(
      detail::ParameterTraits<AudioBlockFormatBinaural>::tag) {
    return boost::make_iterator_range(audioBlockFormatsBinaural_.begin(),
                                      audioBlockFormatsBinaural_.end());
  }

  void AudioChannelFormat::clearAudioBlockFormats() {
    audioBlockFormatsDirectSpeakers_.clear();
    audioBlockFormatsMatrix_.clear();
    audioBlockFormatsObjects_.clear();
    audioBlockFormatsHoa_.clear();
    audioBlockFormatsBinaural_.clear();
  }

  // ---- Common ---- //
  void AudioChannelFormat::print(std::ostream& os) const {
    os << get<AudioChannelFormatId>();
    os << " (audioChannelFormatName=";
    os << get<AudioChannelFormatName>();
    os << ", typeLabel=" << formatTypeLabel(get<TypeDescriptor>());
    os << ", typeDefinition=" << formatTypeDefinition(get<TypeDescriptor>());
    if (has<Frequency>()) {
      os << ", frequency=" << get<Frequency>();
    }
    os << ")";
  }

  void AudioChannelFormat::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }

  std::weak_ptr<Document> AudioChannelFormat::getParent() const {
    return parent_;
  }

  std::shared_ptr<AudioChannelFormat> AudioChannelFormat::copy() const {
    auto audioChannelFormatCopy =
        std::shared_ptr<AudioChannelFormat>(new AudioChannelFormat(*this));
    audioChannelFormatCopy->setParent(std::weak_ptr<Document>());
    return audioChannelFormatCopy;
  }

  AudioChannelFormat::AudioChannelFormat(AudioChannelFormatName name,
                                         TypeDescriptor channelType)
      : name_(name), typeDescriptor_(channelType) {}

}  // namespace adm
