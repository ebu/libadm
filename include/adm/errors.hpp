#pragma once
#include <stdexcept>
#include <boost/optional.hpp>
#include <adm/elements/audio_object_id.hpp>
#include <adm/elements/audio_channel_format_id.hpp>
#include <adm/elements/audio_track_format_id.hpp>
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

    class ADM_EXPORT AudioTrackUidMutuallyExclusiveReferences
        : public AdmException {
     public:
      explicit AudioTrackUidMutuallyExclusiveReferences(
          const AudioChannelFormatId& audioChannelFormatRef,
          const AudioTrackFormatId& audioTrackFormatRef);

     private:
      std::string formatMessage(AudioChannelFormatId audioChannelFormatRef,
                                AudioTrackFormatId audioTrackFormatRef) const;
      AudioChannelFormatId audioChannelFormatRef_;
      AudioTrackFormatId audioTrackFormatRef_;
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
      explicit XmlParsingError(const std::string& message,
                               boost::optional<int> line = boost::none);
      explicit XmlParsingError(int line);

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
      explicit XmlParsingUnresolvedReference(const std::string& id);

     private:
      std::string formatMessage(const std::string& id);
    };

    class ADM_EXPORT XmlParsingUnexpectedAttrError : public XmlParsingError {
     public:
      XmlParsingUnexpectedAttrError(const std::string& attr,
                                    const std::string& value,
                                    boost::optional<int> line = boost::none);

     private:
      static std::string formatMessage(const std::string& attr,
                                       const std::string& value);
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
