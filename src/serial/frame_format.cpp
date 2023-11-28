#include "adm/serial/frame_format.hpp"
#include "adm/utilities/element_io.hpp"

namespace adm {

  // ---- Getter ---- //
  FrameFormatId FrameFormat::get(
      detail::ParameterTraits<FrameFormatId>::tag) const {
    return id_;
  }

  // ---- Has ---- //
  bool FrameFormat::has(detail::ParameterTraits<FrameFormatId>::tag) const {
    return true;
  }

  // ---- Setter ---- //
  /// @brief FrameFormatId setter
  void FrameFormat::set(FrameFormatId id) { id_ = id; }
  /// @brief CountToFull setter

  // ---- Common ---- //
  void FrameFormat::print(std::ostream& os) const {
    os << get<FrameFormatId>();
    os << " (";
    os << "start=" << formatTimecode(get<Start>().get());
    os << ", duration=" << formatTimecode(get<Duration>().get());
    os << ", type=" << formatValue(get<FrameType>());
    os << ", timeReference=" << formatValue(get<TimeReference>());
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

  std::string formatValue(FrameType value) {
    switch (value) {
      case FrameType::HEADER: {
        return "header";
      }
      case FrameType::FULL: {
        return "full";
      }
      case FrameType::DIVIDED: {
        return "divided";
      }
      case FrameType::INTERMEDIATE: {
        return "intermediate";
      }
      case FrameType::ALL: {
        return "all";
      }
    }
    throw std::runtime_error("Unsupported FrameType");
  }

  std::string formatValue(TimeReference value) {
    switch (value) {
      case TimeReference::TOTAL: {
        return "total";
      }
      case TimeReference::LOCAL: {
        return "local";
      }
    }
    throw std::runtime_error("Unsupported TimeReference");
  }
}  // namespace adm
