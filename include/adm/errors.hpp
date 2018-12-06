#pragma once
#include <stdexcept>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <adm/elements/audio_object_id.hpp>
#include "adm/export.h"

namespace adm {
  namespace error {

    class ADM_EXPORT AdmException : public std::runtime_error {
     public:
      explicit AdmException(const std::string& msg);
    };

    class ADM_EXPORT AdmGenericRuntimeError : public AdmException {
     public:
      explicit AdmGenericRuntimeError(const std::string& msg);
    };

    class ADM_EXPORT AudioObjectReferenceCycle : public AdmException {
     public:
      explicit AudioObjectReferenceCycle(const AudioObjectId& referent,
                                         const AudioObjectId& reference);

     private:
      std::string formatMessage(AudioObjectId referent,
                                AudioObjectId reference) const;
      AudioObjectId referent_;
      AudioObjectId reference_;
    };

    class ADM_EXPORT XmlParsingError : public AdmException {
     public:
      XmlParsingError(const std::string& message,
                      boost::optional<int> line = boost::none);
      XmlParsingError(int line);

     private:
      std::string formatMessage(const std::string& message,
                                boost::optional<int> line) const;

      boost::optional<int> line_;
    };

    class ADM_EXPORT XmlParsingDuplicateId : public XmlParsingError {
     public:
      XmlParsingDuplicateId(const std::string& id, boost::optional<int> line);

     private:
      std::string formatMessage(const std::string& id);
    };

    class ADM_EXPORT XmlParsingUnresolvedReference : public XmlParsingError {
     public:
      XmlParsingUnresolvedReference(const std::string& id);

     private:
      std::string formatMessage(const std::string& id);
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
