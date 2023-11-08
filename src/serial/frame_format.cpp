#include "adm/serial/frame_format.hpp"
#include "adm/utilities/element_io.hpp"

namespace adm {
  namespace detail {
    template <>
    TimeReference getDefault<TimeReference>() {
      return TimeReference{TimeReferenceValue::TOTAL};
    };
  }  // namespace detail

  // ---- Getter ---- //
  FrameFormatId FrameFormat::get(
      detail::ParameterTraits<FrameFormatId>::tag) const {
    return id_;
  }
  FlowId FrameFormat::get(
      detail::ParameterTraits<FlowId>::tag) const {
    return flowId_.get();
  }
  CountToFull FrameFormat::get(
      detail::ParameterTraits<CountToFull>::tag) const {
    return countToFull_.get();
  }

  // ---- Has ---- //
  bool FrameFormat::has(detail::ParameterTraits<FrameFormatId>::tag) const {
    return true;
  }
  bool FrameFormat::has(detail::ParameterTraits<FrameType>::tag) const {
    return true;
  }
  bool FrameFormat::has(detail::ParameterTraits<FlowId>::tag) const {
    return flowId_ != boost::none;
  }
  bool FrameFormat::has(detail::ParameterTraits<CountToFull>::tag) const {
    return countToFull_ != boost::none;
  }

  // ---- Setter ---- //
  /// @brief FrameFormatId setter
  void FrameFormat::set(FrameFormatId id) { id_ = id; }
  /// @brief FlowId setter
  void FrameFormat::set(FlowId flowId) { flowId_ = flowId; }
  /// @brief CountToFull setter
  void FrameFormat::set(CountToFull countToFull) { countToFull_ = countToFull; }

  // ---- Unsetter ---- //
  void FrameFormat::unset(detail::ParameterTraits<FlowId>::tag) {
    flowId_ = boost::none;
  }
  void FrameFormat::unset(detail::ParameterTraits<CountToFull>::tag) {
    countToFull_ = boost::none;
  }

  // ---- Common ---- //
  void FrameFormat::print(std::ostream& os) const {
    os << get<FrameFormatId>();
    os << " (";
    os << "start=" << formatTimecode(get<FrameStart>().get());
    os << ", duration=" << formatTimecode(get<FrameDuration>().get());
    os << ", type=" << formatValue(get<FrameType>().get());
    os << ", timeReference=" << formatValue(get<TimeReference>().get());
    if (has<FlowId>()) {
      os << ", flowID=" << get<FlowId>();
    }
    if (has<CountToFull>()) {
      os << ", countToFull=" << get<CountToFull>();
    }
    if (has<NumMetadataChunks>()) {
      os << ", numMetadataChunks" << get<NumMetadataChunks>();
    }
    if (has<CountToSameChunk>()) {
      os << ", numMetadataChunks" << get<CountToSameChunk>();
    }
    os << ")" << std::endl;
  }

  std::string formatValue(FrameTypeValue value) {
    switch (value) {
      case FrameTypeValue::HEADER: {
        return "header";
      }
      case FrameTypeValue::FULL: {
        return "full";
      }
      case FrameTypeValue::DIVIDED: {
        return "divided";
      }
      case FrameTypeValue::INTERMEDIATE: {
        return "intermediate";
      }
      case FrameTypeValue::ALL: {
        return "all";
      }
    }
  }

  std::string formatValue(TimeReferenceValue value) {
    switch (value) {
      case TimeReferenceValue::TOTAL: {
        return "total";
      }
      case TimeReferenceValue::LOCAL: {
        return "local";
      }
    }
  }
}  // namespace adm
