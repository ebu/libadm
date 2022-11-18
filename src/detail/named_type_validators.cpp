#include <adm/detail/named_type_validators.hpp>

namespace adm {
  namespace detail {
    template struct ValidateRangeHelper<float>;
    template struct ValidateRangeHelper<double>;
    template struct ValidateRangeHelper<int>;
    template struct ValidateRangeHelper<unsigned>;
  }  // namespace detail
}  // namespace adm
