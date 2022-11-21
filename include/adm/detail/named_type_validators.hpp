#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include "adm/export.h"

namespace adm {

  struct ADM_EXPORT OutOfRangeError : public std::runtime_error {
    explicit OutOfRangeError(const char* msg) : std::runtime_error(msg) {}
    explicit OutOfRangeError(const std::string& msg)
        : std::runtime_error(msg) {}
  };
  struct ADM_EXPORT InvalidValueError : public std::runtime_error {
    explicit InvalidValueError(const char* msg) : std::runtime_error(msg) {}
    explicit InvalidValueError(const std::string& msg)
        : std::runtime_error(msg) {}
  };
  struct ADM_EXPORT InvalidStringError : public std::runtime_error {
    explicit InvalidStringError(const char* msg) : std::runtime_error(msg) {}
    explicit InvalidStringError(const std::string& msg)
        : std::runtime_error(msg) {}
  };

  namespace detail {

    /// helpers for range validation; this does not have the range as part of
    /// the type, so reduces duplication
    template <typename T>
    struct ValidateRangeHelper {
      static void validateRange(const T& value, int minValue, int maxValue) {
        if (value > static_cast<T>(maxValue) ||
            value < static_cast<T>(minValue)) {
          std::stringstream msg;
          msg << "'" << value << "'"
              << " is not in range [" << minValue << "," << maxValue << "]";
          throw OutOfRangeError(msg.str());
        }
      }

      static void validateMin(const T& value, int minValue) {
        if (value < static_cast<T>(minValue)) {
          std::stringstream msg;
          msg << "'" << value << "'' is not bigger than '" << minValue << "'";
          throw OutOfRangeError(msg.str());
        }
      }

      static void validateMax(const T& value, int maxValue) {
        if (value > static_cast<T>(maxValue)) {
          std::stringstream msg;
          msg << "'" << value << "'' is not smaller than '" << maxValue << "'";
          throw OutOfRangeError(msg.str());
        }
      }
    };

    extern template struct ADM_EXPORT_TEMPLATE_METHODS
        ValidateRangeHelper<float>;
    extern template struct ADM_EXPORT_TEMPLATE_METHODS
        ValidateRangeHelper<double>;
    extern template struct ADM_EXPORT_TEMPLATE_METHODS ValidateRangeHelper<int>;
    extern template struct ADM_EXPORT_TEMPLATE_METHODS
        ValidateRangeHelper<unsigned>;

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
        ValidateRangeHelper<T>::validateRange(value, minValue, maxValue);
      }
    };

    template <int minValue>
    struct MinValidator {
      template <typename T>
      static void validate(const T& value) {
        ValidateRangeHelper<T>::validateMin(value, minValue);
      }
    };

    template <int maxValue>
    struct MaxValidator {
      template <typename T>
      static void validate(const T& value) {
        ValidateRangeHelper<T>::validateMax(value, maxValue);
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

  }  // namespace detail
}  // namespace adm
