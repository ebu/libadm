#include "adm/elements/format_descriptor.hpp"
#include <sstream>
#include <regex>
#include <boost/format.hpp>

namespace adm {

  namespace FormatDefinition {
    const FormatDescriptor UNDEFINED = FormatDescriptor(0);
    const FormatDescriptor PCM = FormatDescriptor(1);
  }  // namespace FormatDefinition

  FormatDescriptor parseFormatLabel(const std::string& label) {
    if (label == "0000") {
      return FormatDefinition::UNDEFINED;
    } else if (label == "0001") {
      return FormatDefinition::PCM;
    } else {
      std::stringstream errorString;
      errorString << "invalid formatLabel: " << label;
      throw std::runtime_error(errorString.str());
    }
  }

  FormatDescriptor parseFormatDefinition(const std::string& definition) {
    if (definition == "Undefined") {
      return FormatDefinition::UNDEFINED;
    } else if (definition == "PCM") {
      return FormatDefinition::PCM;
    } else {
      std::stringstream errorString;
      errorString << "invalid formatDefinition: " << definition;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatFormatLabel(FormatDescriptor desc) {
    if (desc == FormatDefinition::UNDEFINED) {
      return "0000";
    } else if (desc == FormatDefinition::PCM) {
      return "0001";
    } else {
      assert(0);
      return "unknown format descriptor";
    }
  }

  std::string formatFormatDefinition(FormatDescriptor desc) {
    if (desc == FormatDefinition::UNDEFINED) {
      return "Undefined";
    } else if (desc == FormatDefinition::PCM) {
      return "PCM";
    } else {
      assert(0);
      return "unknown format descriptor";
    }
  }
}  // namespace adm
