#include "adm/elements/position_offset.hpp"

namespace adm {
  namespace detail {
    template class DefaultedBase<XOffset>;
    template class DefaultedBase<YOffset>;
    template class DefaultedBase<ZOffset>;
  }  // namespace detail
}  // namespace adm