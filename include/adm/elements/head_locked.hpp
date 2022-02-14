#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {
  /// Tag for NamedType ::HeadLocked
  struct HeadLockedTag {};
  /// @brief NamedType for the headLocked parameter
  using HeadLocked = detail::NamedType<bool, HeadLockedTag>;
}  // namespace adm
