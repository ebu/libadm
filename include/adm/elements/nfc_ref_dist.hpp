/// @file nfc_ref_dist.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {
    /// @brief Tag for NamedType ::NfcRefDist
    struct NfcRefDistTag {};
    /// @brief NamedType for degree parameter
    using NfcRefDist = detail::NamedType<float, NfcRefDistTag>;
}  // namespace adm
