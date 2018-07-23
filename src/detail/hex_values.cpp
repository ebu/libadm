#include "adm/detail/hex_values.hpp"
#include <iomanip>
#include <regex>
#include <sstream>

namespace adm {
  namespace detail {

    unsigned int parseHexValue(const std::string& value,
                               unsigned int nmbOfChars) {
      std::stringstream regexString;
      regexString << "[0-9a-fA-F]{1," << nmbOfChars << "}";
      const std::regex r(regexString.str());
      if (std::regex_match(value, r)) {
        unsigned int ret;
        std::stringstream ss;
        ss << std::hex << value;
        ss >> ret;
        return static_cast<unsigned int>(ret);
      } else {
        std::stringstream errorString;
        errorString << "failed to convert hex value to int: " << value;
        throw std::runtime_error(errorString.str());
      }
    }

    std::string formatHexValue(unsigned int value, unsigned int nmbOfChars) {
      std::stringstream stream;
      stream << std::setfill('0') << std::setw(nmbOfChars) << std::hex << value;
      if (stream.str().length() > nmbOfChars) {
        std::stringstream errorString;
        errorString << "failed to convert int value to hex: " << value;
        throw std::runtime_error(errorString.str());
      }
      return stream.str();
    }
  }  // namespace detail
}  // namespace adm
