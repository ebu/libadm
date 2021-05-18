#include "adm/elements/position_offset.hpp"

namespace adm {
  namespace detail {
    template class DefaultParameter<XOffset>;
    template class DefaultParameter<YOffset>;
    template class DefaultParameter<ZOffset>;
  }  // namespace detail
}  // namespace adm