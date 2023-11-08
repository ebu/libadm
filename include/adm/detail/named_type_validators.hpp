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
    template<typename T>
    void validateStringList(std::string const& type, std::string const& value, T&& list) {
      for(auto const& element : list) {
        if (element == value) return;
      }
      std::stringstream msg;
      msg << "'" << value << "' is not a valid " << type << ". Permitted values are:";
      for(auto const& element : list) {
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
          msg << "'" << value << "'"
              << " is not in range [" << minValue << "," << maxValue << "]";
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
        validateStringList("ScreenEdge",
                           value,
                           std::array<std::string, 4>{"left", "right", "top", "bottom"});
      }
    };

    struct HorizontalEdgeValidator {
      static void validate(const std::string& value) {
        validateStringList("HorizontalEdge",
                           value,
                           std::array<std::string, 2>{"left", "right"});
      }
    };

    struct VerticalEdgeValidator {
      static void validate(const std::string& value) {
        validateStringList("VerticalEdge",
                           value,
                           std::array<std::string, 2>{"top", "bottom"});
      }
    };

    struct FrequencyTypeValidator {
      static void validate(const std::string& value) {
        validateStringList("FrequencyType",
                           value,
                           std::array<std::string, 2>{"lowPass", "highPass"});
      }
    };

    struct CoordinateValueValidator {
      static void validate(const std::string& value) {
        validateStringList("CoordinateValue",
                           value,
                           std::array<std::string, 6>{"azimuth", "elevation", "distance", "X", "Y", "Z"});
      }
    };

    struct SphericalCoordinateValueValidator {
      static void validate(const std::string& value) {
        validateStringList("SphericalCoordinateValue",
                           value,
                           std::array<std::string, 3>{"azimuth", "elevation", "distance"});
      }
    };

    struct CartesianCoordinateValueValidator {
      static void validate(const std::string& value) {
        validateStringList("CartesianCoordinateValue", value,
                           std::array<std::string, 3>{"X", "Y", "Z"});
      }
    };

    struct BoundValueValidator {
      static void validate(const std::string& value) {
        validateStringList("Bound", value,
                           std::array<std::string, 2>{"min", "max"});
      }
    };

    struct TimeReferenceValueValidator {
      static void validate(const std::string& value) {
        validateStringList("TimeReference", value,
                           std::array<std::string, 2>{"total", "local"});
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

    struct CountToFullValueValidator {
      static void validate(const unsigned int& value) {
        return;
      }
    };

  }  // namespace detail
}  // namespace adm
