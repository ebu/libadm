#include "adm/elements/position_offset.hpp"

namespace adm {

  // ---- SPHERICAL POSITION OFFSET ---- //

  // ---- Constructor ---- //
  SphericalPositionOffset::SphericalPositionOffset(Azimuth azimuth,
                                                   Elevation elevation,
                                                   DistanceOffset distance)
      : azimuth_(azimuth), elevation_(elevation), distance_(distance){};

  // ---- Getter ---- //
  Azimuth SphericalPositionOffset::get(
      detail::ParameterTraits<Azimuth>::tag) const {
    return azimuth_;
  }
  Elevation SphericalPositionOffset::get(
      detail::ParameterTraits<Elevation>::tag) const {
    return elevation_;
  }
  DistanceOffset SphericalPositionOffset::get(
      detail::ParameterTraits<DistanceOffset>::tag) const {
    return distance_;
  }

  // ---- Has ---- //
  bool SphericalPositionOffset::has(
      detail::ParameterTraits<Azimuth>::tag) const {
    return true;
  }
  bool SphericalPositionOffset::has(
      detail::ParameterTraits<Elevation>::tag) const {
    return true;
  }
  bool SphericalPositionOffset::has(
      detail::ParameterTraits<DistanceOffset>::tag) const {
    return true;
  }

  // ---- Setter ---- //
  void SphericalPositionOffset::set(Azimuth azimuth) { azimuth_ = azimuth; }
  void SphericalPositionOffset::set(Elevation elevation) {
    elevation_ = elevation;
  }
  void SphericalPositionOffset::set(DistanceOffset distance) {
    distance_ = distance;
  }

  // ---- CARTESIAN POSITION ---- //

  // ---- Constructor ---- //
  CartesianPositionOffset::CartesianPositionOffset(X x, Y y, Z z)
      : x_(x), y_(y), z_(z){};

  // ---- Getter ---- //
  X CartesianPositionOffset::get(detail::ParameterTraits<X>::tag) const {
    return x_;
  }
  Y CartesianPositionOffset::get(detail::ParameterTraits<Y>::tag) const {
    return y_;
  }
  Z CartesianPositionOffset::get(detail::ParameterTraits<Z>::tag) const {
    return z_;
  }

  // ---- Has ---- //
  bool CartesianPositionOffset::has(detail::ParameterTraits<X>::tag) const {
    return true;
  }
  bool CartesianPositionOffset::has(detail::ParameterTraits<Y>::tag) const {
    return true;
  }
  bool CartesianPositionOffset::has(detail::ParameterTraits<Z>::tag) const {
    return true;
  }

  // ---- Setter ---- //
  void CartesianPositionOffset::set(X x) { x_ = x; }
  void CartesianPositionOffset::set(Y y) { y_ = y; }
  void CartesianPositionOffset::set(Z z) { z_ = z; }

  // ---- FREE FUNCTIONS ---- //

  bool isSpherical(const PositionOffset& offset) {
    if (offset.which() == 0) {
      return true;
    } else {
      return false;
    }
  }

  bool isCartesian(const PositionOffset& offset) {
    if (offset.which() == 1) {
      return true;
    } else {
      return false;
    }
  }

}  // namespace adm
