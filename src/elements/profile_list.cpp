#include "adm/elements/profile_list.hpp"

namespace adm {
  namespace detail {
    template class RequiredParameter<ProfileValue>;
    template class RequiredParameter<ProfileName>;
    template class RequiredParameter<ProfileVersion>;
    template class RequiredParameter<ProfileLevel>;
    template class VectorParameter<Profiles>;
  }  // namespace detail
}  // namespace adm
