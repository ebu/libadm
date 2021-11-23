#include "adm/elements/position_offset.hpp"
#include "adm/detail/print_helper.hpp"

namespace adm {

  namespace detail {
    template class DefaultParameter<AzimuthOffset>;
    template class DefaultParameter<ElevationOffset>;
    template class DefaultParameter<DistanceOffset>;

    template class DefaultParameter<XOffset>;
    template class DefaultParameter<YOffset>;
    template class DefaultParameter<ZOffset>;
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
    return offset.type() == typeid(SphericalPositionOffset);
  }

  bool isCartesian(const PositionOffset& offset) {
    return offset.type() == typeid(CartesianPositionOffset);
  }

}  // namespace adm
