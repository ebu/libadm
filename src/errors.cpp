#include "adm/errors.hpp"
#include <boost/format.hpp>

namespace adm {
  namespace error {

    AdmException::AdmException(const std::string& msg)
        : std::runtime_error(msg) {}

    AdmGenericRuntimeError::AdmGenericRuntimeError(const std::string& msg)
        : AdmException(msg) {}

    AudioObjectReferenceCycle::AudioObjectReferenceCycle(
        const AudioObjectId& referent, const AudioObjectId& reference)
        : AdmException(formatMessage(referent, reference)) {}

    std::string AudioObjectReferenceCycle::formatMessage(
        AudioObjectId referent, AudioObjectId reference) const {
      return boost::str(
          boost::format(
              "Cyclic AudioObject reference detected from %1% to %2%") %
          formatId(referent) % formatId(reference));
    }

    XmlParsingError::XmlParsingError(const std::string& message,
                                     boost::optional<int> line)
        : AdmException(formatMessage(message, line)), line_(line) {}

    XmlParsingError::XmlParsingError(int line)
        : XmlParsingError(formatMessage("Error parsing XML", line)) {}

    std::string XmlParsingError::formatMessage(
        const std::string& message, boost::optional<int> line) const {
      if (line) {
        return boost::str(boost::format("%1% (:%2%)") % message % line.get());
      } else {
        return message;
      }
    }

    XmlParsingDuplicateId::XmlParsingDuplicateId(const std::string& id,
                                                 boost::optional<int> line)
        : XmlParsingError(formatMessage(id), line) {}

    std::string XmlParsingDuplicateId::formatMessage(const std::string& id) {
      return boost::str(boost::format("Duplicate Id %1% found") % id);
    }

    XmlParsingUnresolvedReference::XmlParsingUnresolvedReference(
        const std::string& id)
        : XmlParsingError(formatMessage(id)) {}

    std::string XmlParsingUnresolvedReference::formatMessage(
        const std::string& id) {
      return boost::str(boost::format("Id %1% could not be resolved") % id);
    }

  }  // namespace error
}  // namespace adm
