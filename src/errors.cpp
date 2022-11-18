#include "adm/errors.hpp"

namespace adm {
  namespace error {

    AdmException::AdmException(const std::string& msg)
        : std::runtime_error(msg) {}

    AdmGenericRuntimeError::AdmGenericRuntimeError(const std::string& msg)
        : AdmException(msg) {}

    AudioTrackUidMutuallyExclusiveReferences::
        AudioTrackUidMutuallyExclusiveReferences(
            const AudioChannelFormatId& audioChannelFormatRef,
            const AudioTrackFormatId& audioTrackFormatRef)
        : AdmException(
              formatMessage(audioChannelFormatRef, audioTrackFormatRef)) {}

    std::string AudioTrackUidMutuallyExclusiveReferences::formatMessage(
        AudioChannelFormatId audioChannelFormatRef,
        AudioTrackFormatId audioTrackFormatRef) const {
      return "Mutually-exclusive AudioTrackUid references detected: " +
             formatId(audioChannelFormatRef) + " and " +
             formatId(audioTrackFormatRef);
    }

    AudioObjectReferenceCycle::AudioObjectReferenceCycle(
        const AudioObjectId& referent, const AudioObjectId& reference)
        : AdmException(formatMessage(referent, reference)) {}

    std::string AudioObjectReferenceCycle::formatMessage(
        AudioObjectId referent, AudioObjectId reference) const {
      return "Cyclic AudioObject reference detected from " +
             formatId(referent) + " to " + formatId(reference);
    }

    XmlParsingError::XmlParsingError(const std::string& message,
                                     boost::optional<int> line)
        : AdmException(formatMessage(message, line)), line_(line) {}

    XmlParsingError::XmlParsingError(int line)
        : XmlParsingError(formatMessage("Error parsing XML", line)) {}

    std::string XmlParsingError::formatMessage(
        const std::string& message, boost::optional<int> line) const {
      if (line) {
        return message + " (:" + std::to_string(line.get()) + ")";
      } else {
        return message;
      }
    }

    XmlParsingDuplicateId::XmlParsingDuplicateId(const std::string& id,
                                                 boost::optional<int> line)
        : XmlParsingError(formatMessage(id), line) {}

    std::string XmlParsingDuplicateId::formatMessage(const std::string& id) {
      return "Duplicate Id " + id + " found";
    }

    XmlParsingUnresolvedReference::XmlParsingUnresolvedReference(
        const std::string& id)
        : XmlParsingError(formatMessage(id)) {}

    std::string XmlParsingUnresolvedReference::formatMessage(
        const std::string& id) {
      return "Id " + id + " could not be resolved";
    }

    XmlParsingUnexpectedAttrError::XmlParsingUnexpectedAttrError(
        const std::string& attr, const std::string& value,
        boost::optional<int> line)
        : XmlParsingError(formatMessage(attr, value), line) {}

    std::string XmlParsingUnexpectedAttrError::formatMessage(
        const std::string& attr, const std::string& value) {
      return "Unexpected value \"" + value + "\" found in attribute " + attr;
    }

  }  // namespace error
}  // namespace adm
