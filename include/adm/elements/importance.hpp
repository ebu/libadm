/// @file importance.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {

  /// @brief Tag for NamedType ::Importance
  struct ImportanceTag {};
  /**
   * @brief NamedType for the importance parameter
   *
   * Valid values are in the range [0, 10].
   */
  using Importance =
      detail::NamedType<int, ImportanceTag, detail::RangeValidator<0, 10>>;

}  // namespace adm
