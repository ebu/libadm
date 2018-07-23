#include <adm/errors.hpp>
#include <boost/format.hpp>

namespace adm {
  namespace error {

    void AudioObjectReferenceCycle::formatMessage() {
      msg_ = boost::str(
          boost::format(
              "Cyclic AudioObject reference detectet from %1% to %2%") %
          formatId(referent_) % formatId(reference_));
    }

    XmlParsingError::XmlParsingError(int line) : line_(line) {
      msg_ = boost::str(boost::format("Error parsinge XML (:%1%)") % line_);
    }

    std::string XmlParsingError::formatMessageWithLine(
        const std::string& message) {
      return boost::str(boost::format("%1% (:%2%)") % message % line_);
    }

    XmlParsingDuplicateId::XmlParsingDuplicateId(const std::string& id,
                                                 int line)
        : XmlParsingError(line) {
      auto msg = boost::str(boost::format("Duplicate Id %1% found") % id);
      message(msg);
    }

  }  // namespace error
}  // namespace adm
