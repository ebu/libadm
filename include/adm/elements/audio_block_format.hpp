#pragma once
#include <boost/optional.hpp>
#include "adm/elements/gain.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  class AudioBlockFormat {
   public:
    ADM_EXPORT void set(Gain gain);
    ADM_EXPORT Gain get(detail::ParameterTraits<Gain>::tag) const;
    ADM_EXPORT bool has(detail::ParameterTraits<Gain>::tag) const;
    ADM_EXPORT bool isDefault(detail::ParameterTraits<Gain>::tag) const;
    ADM_EXPORT void unset(detail::ParameterTraits<Gain>::tag);

   private:
    boost::optional<Gain> gain_;
  };

}  // namespace adm
