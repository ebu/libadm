/// @file normalization.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {
    /// @brief Tag for NamedType ::Normalization
    struct NormalizationTag {};
    /// @brief NamedType for a normalization parameter, defaulting to SN3D
    using Normalization = detail::NamedType<std::string, NormalizationTag>;
}  // namespace adm
