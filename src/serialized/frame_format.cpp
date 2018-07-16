#include "adm/serialized/frame_format.hpp"

#include <algorithm>
#include <functional>
#include "adm/utilities/element_io.hpp"

namespace adm {

  namespace {
    const TimeReference timeReferenceDefault{"total"};
  }

  // ---- Getter ---- //
  FrameFormatId FrameFormat::get(
      detail::ParameterTraits<FrameFormatId>::tag) const {
    return id_;
  }
  FrameStart FrameFormat::get(detail::ParameterTraits<FrameStart>::tag) const {
    return start_;
  }
  FrameDuration FrameFormat::get(
      detail::ParameterTraits<FrameDuration>::tag) const {
    return duration_;
  }
  FrameType FrameFormat::get(detail::ParameterTraits<FrameType>::tag) const {
    return frameType_;
  }
  TimeReference FrameFormat::get(
      detail::ParameterTraits<TimeReference>::tag) const {
    return boost::get_optional_value_or(timeReference_, timeReferenceDefault);
  }
  CountToFull FrameFormat::get(
      detail::ParameterTraits<CountToFull>::tag) const {
    return countToFull_.get();
  }
  NumSubFrame FrameFormat::get(
      detail::ParameterTraits<NumSubFrame>::tag) const {
    return numSubFrame_.get();
  }
  FrameSkip FrameFormat::get(detail::ParameterTraits<FrameSkip>::tag) const {
    return frameSkip_.get();
  }
  FrameShift FrameFormat::get(detail::ParameterTraits<FrameShift>::tag) const {
    return frameShift_.get();
  }

  // ---- Has ---- //
  bool FrameFormat::has(detail::ParameterTraits<FrameFormatId>::tag) const {
    return true;
  }
  bool FrameFormat::has(detail::ParameterTraits<FrameStart>::tag) const {
    return true;
  }
  bool FrameFormat::has(detail::ParameterTraits<FrameDuration>::tag) const {
    return true;
  }
  bool FrameFormat::has(detail::ParameterTraits<FrameType>::tag) const {
    return true;
  }
  bool FrameFormat::has(detail::ParameterTraits<TimeReference>::tag) const {
    return timeReference_ != boost::none;
  }
  bool FrameFormat::has(detail::ParameterTraits<CountToFull>::tag) const {
    return countToFull_ != boost::none;
  }
  bool FrameFormat::has(detail::ParameterTraits<NumSubFrame>::tag) const {
    return numSubFrame_ != boost::none;
  }
  bool FrameFormat::has(detail::ParameterTraits<FrameSkip>::tag) const {
    return frameSkip_ != boost::none;
  }
  bool FrameFormat::has(detail::ParameterTraits<FrameShift>::tag) const {
    return frameShift_ != boost::none;
  }

  // ---- Setter ---- //
  /// @brief FrameFormatId setter
  void FrameFormat::set(FrameFormatId id) { id_ = id; }
  /// @brief FrameStart setter
  void FrameFormat::set(FrameStart start) { start_ = start; }
  /// @brief FrameDuration setter
  void FrameFormat::set(FrameDuration duration) { duration_ = duration; }
  /// @brief FrameType setter
  void FrameFormat::set(FrameType frameType) { frameType_ = frameType; }
  /// @brief TimeReference setter
  void FrameFormat::set(TimeReference timeReference) {
    timeReference_ = timeReference;
  }
  /// @brief CountToFull setter
  void FrameFormat::set(CountToFull countToFull) { countToFull_ = countToFull; }
  /// @brief numSubFrame setter
  void FrameFormat::set(NumSubFrame numSubFrame) { numSubFrame_ = numSubFrame; }
  /// @brief FrameSkip setter
  void FrameFormat::set(FrameSkip frameSkip) { frameSkip_ = frameSkip; }
  /// @brief FrameShift setter
  void FrameFormat::set(FrameShift frameShift) { frameShift_ = frameShift; }

  // ---- Unsetter ---- //
  void FrameFormat::unset(detail::ParameterTraits<TimeReference>::tag) {
    timeReference_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<CountToFull>::tag) {
    countToFull_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<NumSubFrame>::tag) {
    numSubFrame_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<FrameSkip>::tag) {
    frameSkip_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<FrameShift>::tag) {
    frameShift_ = boost::none;
  }

  // ---- isDefault ---- //
  void FrameFormat::isDefault(detail::ParameterTraits<TimeReference>::tag) {
    timeReference_ = boost::none;
  }

  // ---- Common ---- //
  void FrameFormat::print(std::ostream& os) const {
    os << get<FrameFormatId>();
    os << " (frameStart=" << formatTimecode(get<FrameStart>().get());
    os << ", frameDuration=" << formatTimecode(get<FrameDuration>().get());
    os << ", timeReference=";
    os << get<TimeReference>().get();
    os << ", frameType=";
    os << get<FrameType>().get();
    if (has<CountToFull>()) {
      os << ", countToFull=" << get<CountToFull>();
    }
    if (has<NumSubFrame>()) {
      os << ", numSubFrame=" << get<NumSubFrame>();
    }
    if (has<FrameSkip>()) {
      os << ", frameSkip=" << get<FrameSkip>();
    }
    if (has<FrameShift>()) {
      os << ", frameShift=" << get<FrameShift>();
    }
    os << ")";
  }

}  // namespace adm
