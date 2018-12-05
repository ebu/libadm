#pragma once
#include <stdexcept>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <adm/elements/audio_object_id.hpp>
#include "adm/export.h"

namespace adm {
  namespace error {

    class AdmException : public std::runtime_error {
     public:
      AdmException(const std::string& msg) : std::runtime_error(msg) {}
    };

    class AdmGenericRuntimeError : public AdmException {
     public:
      AdmGenericRuntimeError(const std::string& msg) : AdmException(msg) {}
    };

    class AudioObjectReferenceCycle : public AdmException {
     public:
      AudioObjectReferenceCycle(AudioObjectId referent, AudioObjectId reference)
          : AdmException(formatMessage(referent, reference)),
            referent_(referent),
            reference_(reference) {}
      AudioObjectId reference() const { return reference_; }
      AudioObjectId referent() const { return referent_; }

     private:
      std::string formatMessage(AudioObjectId referent,
                                AudioObjectId reference) const {
        return boost::str(
            boost::format(
                "Cyclic AudioObject reference detectet from %1% to %2%") %
            formatId(referent_) % formatId(reference_));
      };
      AudioObjectId referent_;
      AudioObjectId reference_;
    };

    class XmlParsingError : public AdmException {
     public:
      XmlParsingError(const std::string& message,
                      boost::optional<int> line = boost::none)
          : AdmException(formatMessage(message, line)), line_(line) {}
      XmlParsingError(int line)
          : XmlParsingError(formatMessage("Error parsing XML", line)) {}

     private:
      std::string formatMessage(const std::string& message,
                                boost::optional<int> line) const {
        if (line) {
          return boost::str(boost::format("%1% (:%2%)") % message % line.get());
        } else {
          return message;
        }
      }

      boost::optional<int> line_;
    };

    class XmlParsingDuplicateId : public XmlParsingError {
     public:
      XmlParsingDuplicateId(const std::string& id, boost::optional<int> line)
          : XmlParsingError(formatMessage(id), line) {}

     private:
      std::string formatMessage(const std::string& id) {
        return boost::str(boost::format("Duplicate Id %1% found") % id);
      }
    };

    class XmlParsingUnresolvedReference : public XmlParsingError {
     public:
      XmlParsingUnresolvedReference(const std::string& id)
          : XmlParsingError(formatMessage(id)) {}

     private:
      std::string formatMessage(const std::string& id) {
        return boost::str(boost::format("Id %1% could not be resolved") % id);
      }
    };

    namespace detail {

      /**
       * @brief Helper to construct an Exception with a formatted message
       * that includes the Id of an ADM element that is related to the error at
       * hand.
       */
      template <typename ElementId>
      AdmGenericRuntimeError formatElementRuntimeError(
          ElementId id, const std::string& message) {
        return AdmGenericRuntimeError(formatId(id) + ": " + message);
      }

    }  // namespace detail

  }  // namespace error
}  // namespace adm
