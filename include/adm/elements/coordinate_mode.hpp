/// @file coordinate_mode.hpp
#pragma once

#include <string>
#include "adm/detail/named_type.hpp"

namespace adm {

  /// @brief Tag for NamedType ::CoordinateMode
  struct CoordinateModeTag {};
  /// @brief NamedType for the coordinateMode attribute
  ///
  /// Allowable values are "polar" or "cartesian".
  using CoordinateMode = detail::NamedType<std::string, CoordinateModeTag>;

}  // namespace adm
