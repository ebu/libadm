/// @file screen_ref.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {
  /// @brief Tag for NamedType ::ScreenRef
  struct ScreenRefTag {};
  /// @brief NamedType for screenref parameter
  using ScreenRef = detail::NamedType<bool, ScreenRefTag>;
}  // namespace adm
