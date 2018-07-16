/// @file initialize_block.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {
  /// @brief Tag for NamedType ::InitializeBlock
  struct InitializeBlockTag {};
  /// @brief NamedType for the InitializeBlock attribute
  using InitializeBlock = detail::NamedType<bool, InitializeBlockTag>;
}  // namespace adm
