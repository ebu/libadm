#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace adm {

  struct OutOfRangeError : public std::runtime_error {
    OutOfRangeError(const char* msg) : std::runtime_error(msg) {}
    OutOfRangeError(const std::string& msg) : std::runtime_error(msg) {}
  };
  struct InvalidValueError : public std::runtime_error {
    InvalidValueError(const char* msg) : std::runtime_error(msg) {}
    InvalidValueError(const std::string& msg) : std::runtime_error(msg) {}
  };
  struct InvalidStringError : public std::runtime_error {
    InvalidStringError(const char* msg) : std::runtime_error(msg) {}
    InvalidStringError(const std::string& msg) : std::runtime_error(msg) {}
  };

  namespace detail {

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
        for (auto& validString : {"left", "right", "top", "bottom"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal ScreenEdge: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct HorizontalEdgeValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"left", "right"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal HorizontalEdge: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct VerticalEdgeValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"top", "bottom"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal VerticalEdge: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct FrequencyTypeValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"lowPass", "highPass"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal FrequencyType: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct CoordinateValueValidator {
      static void validate(const std::string& value) {
        for (auto& validString :
             {"azimuth", "elevation", "distance", "X", "Y", "Z"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal CoordinateValue: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct SphericalCoordinateValueValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"azimuth", "elevation", "distance"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal SphericalCoordinateValue: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct CartesianCoordinateValueValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"X", "Y", "Z"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal CartesianCoordinateValue: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct BoundValueValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"min", "max"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal Bound: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct TimeReferenceValueValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"total", "local"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal TimeReference: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

    struct FrameTypeValueValidator {
      static void validate(const std::string& value) {
        for (auto& validString : {"header", "full", "intermediate", "all"}) {
          if (value == validString) {
            return;
          }
        }
        std::stringstream msg;
        msg << "illegal FrameType: '" << value << "'";
        throw InvalidStringError(msg.str());
      }
    };

  }  // namespace detail
}  // namespace adm
