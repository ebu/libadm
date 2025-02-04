#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <array>

namespace adm {

  struct OutOfRangeError : public std::runtime_error {
    explicit OutOfRangeError(const char* msg) : std::runtime_error(msg) {}
    explicit OutOfRangeError(const std::string& msg)
        : std::runtime_error(msg) {}
  };
  struct InvalidValueError : public std::runtime_error {
    explicit InvalidValueError(const char* msg) : std::runtime_error(msg) {}
    explicit InvalidValueError(const std::string& msg)
        : std::runtime_error(msg) {}
  };
  struct InvalidStringError : public std::runtime_error {
    explicit InvalidStringError(const char* msg) : std::runtime_error(msg) {}
    explicit InvalidStringError(const std::string& msg)
        : std::runtime_error(msg) {}
  };

  namespace detail {
    inline void validateStringList(std::string const& value, const char* type,
                                   std::initializer_list<char const*> list) {
      for (auto const& element : list) {
        if (element == value) return;
      }
      std::stringstream msg;
      msg << "'" << value << "' is not a valid " << type
          << ". Permitted values are:";
      for (auto const& element : list) {
        msg << " '" << element << "'";
      }
      throw InvalidStringError(msg.str());
    }

    struct DefaultValidator {
      template <typename T>
      static void validate(const T&) {
        return;
      }
    };

    template <int minValue, int maxValue>
    struct RangeValidator {
      template <typename T>
      static void validate(const T& value) {
        if (value > maxValue || value < minValue) {
          std::stringstream msg;
          msg << "'" << value << "'" << " is not in range [" << minValue << ","
              << maxValue << "]";
          throw OutOfRangeError(msg.str());
        }
      }
    };

    template <int minValue>
    struct MinValidator {
      template <typename T>
      static void validate(const T& value) {
        if (value < minValue) {
          std::stringstream msg;
          msg << "'" << value << "'' is not bigger than '" << minValue << "'";
          throw OutOfRangeError(msg.str());
        }
      }
    };

    template <int maxValue>
    struct MaxValidator {
      template <typename T>
      static void validate(const T& value) {
        if (value > maxValue) {
          std::stringstream msg;
          msg << "'" << value << "'' is not smaller than '" << maxValue << "'";
          throw OutOfRangeError(msg.str());
        }
      }
    };

    template <typename T, T... ValidValues>
    struct ValueValidator {
      static void validate(const T& v) {
        for (auto& validString : {ValidValues...}) {
          if (v == validString) {
            return;
          }
        }
        throw InvalidValueError("");
      }
    };

    struct ScreenEdgeValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "ScreenEdge",
                           {"left", "right", "top", "bottom"});
      }
    };

    struct HorizontalEdgeValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "HorizontalEdge", {"left", "right"});
      }
    };

    struct VerticalEdgeValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "VerticalEdge", {"top", "bottom"});
      }
    };

    struct FrequencyTypeValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "FrequencyType", {"lowPass", "highPass"});
      }
    };

    struct CoordinateValueValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "CoordinateValue",
                           {"azimuth", "elevation", "distance", "X", "Y", "Z"});
      }
    };

    struct SphericalCoordinateValueValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "SphericalCoordinateValue",
                           {"azimuth", "elevation", "distance"});
      }
    };

    struct CartesianCoordinateValueValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "CartesianCoordinateValue", {"X", "Y", "Z"});
      }
    };

    struct BoundValueValidator {
      static void validate(const std::string& value) {
        validateStringList(value, "Bound", {"min", "max"});
      }
    };

    struct FrameIndexValidator {
      static void validate(unsigned int value) {
        if (!(value > 0 && value <= 0xFFFFFFFF)) {
          throw OutOfRangeError("FrameIndex value: " + std::to_string(value) +
                                " not within valid range of [0x1,0xFFFFFFFF]");
        }
      }
    };

    struct ChunkIndexValidator {
      static void validate(unsigned int value) {
        if (!(value > 0 && value <= 0xFF)) {
          throw OutOfRangeError("ChunkIndex value: " + std::to_string(value) +
                                " not within valid range of [0x1,0xFF]");
        }
      }
    };

  }  // namespace detail
}  // namespace adm
