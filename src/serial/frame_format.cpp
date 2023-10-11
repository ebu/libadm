#include "adm/serial/frame_format.hpp"

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
  FlowId FrameFormat::get(
      detail::ParameterTraits<FlowId>::tag) const {
    return flowId_.get();
  }
  CountToFull FrameFormat::get(
      detail::ParameterTraits<CountToFull>::tag) const {
    return countToFull_.get();
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
  bool FrameFormat::has(detail::ParameterTraits<FlowId>::tag) const {
    return flowId_ != boost::none;
  }
  bool FrameFormat::has(detail::ParameterTraits<CountToFull>::tag) const {
    return countToFull_ != boost::none;
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
  /// @brief FlowId setter
  void FrameFormat::set(FlowId flowId) { flowId_ = flowId; }
  /// @brief CountToFull setter
  void FrameFormat::set(CountToFull countToFull) { countToFull_ = countToFull; }
  /// @brief FrameShift setter
  void FrameFormat::set(FrameShift frameShift) { frameShift_ = frameShift; }

  // ---- Unsetter ---- //
  void FrameFormat::unset(detail::ParameterTraits<TimeReference>::tag) {
    timeReference_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<FlowId>::tag) {
    flowId_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<CountToFull>::tag) {
    countToFull_ = boost::none;
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
    os << get<FrameFormatId>() << std::endl;
    os << " (start=" << formatTimecode(get<FrameStart>().get()) << std::endl;
    os << ", duration=" << formatTimecode(get<FrameDuration>().get()) << std::endl;
    os << ", timeReference=" << std::endl;
    os << get<TimeReference>().get() << std::endl;
    if (has<FlowId>()) {
      os << ", flowID=" << get<FlowId>() << std::endl;
    }
    os << ", type=";
    os << get<FrameType>().get() << std::endl;
    if (has<CountToFull>()) {
      os << ", countToFull=" << get<CountToFull>() << std::endl;
    }
    if (has<FrameShift>()) {
      os << ", frameShift=" << get<FrameShift>();
    }
    os << ")";
  }

}  // namespace adm
