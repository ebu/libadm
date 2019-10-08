/// @file screen_ref.hpp
#pragma once

#include <boost/variant.hpp>
#include "adm/detail/named_type.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"
namespace adm {
  /// @brief Tag for NamedType ::ScreenRef
  struct ScreenRefTag {};
  /// @brief NamedType for screenref parameter
  using ScreenRef = detail::NamedType<bool, ScreenRefTag>;
}  // namespace adm
