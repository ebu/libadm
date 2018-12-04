#pragma once

#include <string>
#include "adm/export.h"

namespace adm {
  namespace detail {

    ADM_EXPORT unsigned int parseHexValue(const std::string& value,
                                          unsigned int nmbOfChars = 4);
    ADM_EXPORT std::string formatHexValue(unsigned int value,
                                          unsigned int nmbOfChars = 4);
  }  // namespace detail
}  // namespace adm
