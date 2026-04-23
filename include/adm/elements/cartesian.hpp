/// @file cartesian.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {
  /// @brief Tag for NamedType ::Cartesian
  struct CartesianTag {};
  /// @brief NamedType for cartesian parameter
  using Cartesian = detail::NamedType<bool, CartesianTag>;
}  // namespace adm
