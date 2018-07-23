#pragma once

#include <string>
#include "adm/libadm_export.h"

namespace adm {
  namespace detail {

    LIBADM_EXPORT unsigned int parseHexValue(const std::string& value,
                                             unsigned int nmbOfChars = 4);
    LIBADM_EXPORT std::string formatHexValue(unsigned int value,
                                             unsigned int nmbOfChars = 4);
  }  // namespace detail
}  // namespace adm
