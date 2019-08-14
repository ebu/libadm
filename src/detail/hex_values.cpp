#include "adm/detail/hex_values.hpp"
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

namespace adm {
  namespace detail {

    unsigned int parseHexValue(const std::string& value,
                               unsigned int nmbOfChars) {
      const static std::regex r("[0-9a-fA-F]+");

      if (std::regex_match(value, r) && value.size() <= nmbOfChars) {
        return static_cast<unsigned int>(std::stol(value, nullptr, 16));
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
