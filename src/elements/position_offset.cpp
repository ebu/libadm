#include "adm/elements/position_offset.hpp"
#include "adm/detail/holds_alternative.hpp"
#include "adm/detail/print_helper.hpp"

namespace adm {

  namespace detail {
    template class OptionalParameter<AzimuthOffset>;
    template class OptionalParameter<ElevationOffset>;
    template class OptionalParameter<DistanceOffset>;

    template class OptionalParameter<XOffset>;
    template class OptionalParameter<YOffset>;
    template class OptionalParameter<ZOffset>;
  }  // namespace detail

  void SphericalPositionOffset::print(std::ostream& os) const {
    auto print_helper = make_print_helper(*this, os);

    print_helper.print<AzimuthOffset>("azimuth");
    print_helper.print<ElevationOffset>("elevation");
    print_helper.print<DistanceOffset>("distance");
  }

  void CartesianPositionOffset::print(std::ostream& os) const {
    auto print_helper = make_print_helper(*this, os);

    print_helper.print<XOffset>("X");
    print_helper.print<YOffset>("Y");
    print_helper.print<ZOffset>("Z");
  }

  bool isSpherical(const PositionOffset& offset) {
    return detail::holds_alternative<SphericalPositionOffset>(offset);
  }

  bool isCartesian(const PositionOffset& offset) {
    return detail::holds_alternative<CartesianPositionOffset>(offset);
  }

}  // namespace adm
