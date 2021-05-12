#include "adm/elements/position_offset.hpp"

namespace adm {

  // ---- SPHERICAL POSITION OFFSET ---- //

  // ---- Constructor ---- //
  SphericalPositionOffset::SphericalPositionOffset(AzimuthOffset azimuth,
                                                   ElevationOffset elevation,
                                                   DistanceOffset distance)
      : azimuth_(azimuth), elevation_(elevation), distance_(distance){};

  // ---- Getter ---- //
  AzimuthOffset SphericalPositionOffset::get(
      detail::ParameterTraits<AzimuthOffset>::tag) const {
    return azimuth_;
  }
  ElevationOffset SphericalPositionOffset::get(
      detail::ParameterTraits<ElevationOffset>::tag) const {
    return elevation_;
  }
  DistanceOffset SphericalPositionOffset::get(
      detail::ParameterTraits<DistanceOffset>::tag) const {
    return distance_;
  }

  // ---- Has ---- //
  bool SphericalPositionOffset::has(
      detail::ParameterTraits<AzimuthOffset>::tag) const {
    return true;
  }
  bool SphericalPositionOffset::has(
      detail::ParameterTraits<ElevationOffset>::tag) const {
    return true;
  }
  bool SphericalPositionOffset::has(
      detail::ParameterTraits<DistanceOffset>::tag) const {
    return true;
  }

  // ---- Setter ---- //
  void SphericalPositionOffset::set(AzimuthOffset azimuth) {
    azimuth_ = azimuth;
  }
  void SphericalPositionOffset::set(ElevationOffset elevation) {
    elevation_ = elevation;
  }
  void SphericalPositionOffset::set(DistanceOffset distance) {
    distance_ = distance;
  }

  // ---- CARTESIAN POSITION ---- //

  // ---- Constructor ---- //
  CartesianPositionOffset::CartesianPositionOffset(XOffset x, YOffset y,
                                                   ZOffset z)
      : x_(x), y_(y), z_(z){};

  // ---- Getter ---- //
  XOffset CartesianPositionOffset::get(
      detail::ParameterTraits<XOffset>::tag) const {
    return x_;
  }
  YOffset CartesianPositionOffset::get(
      detail::ParameterTraits<YOffset>::tag) const {
    return y_;
  }
  ZOffset CartesianPositionOffset::get(
      detail::ParameterTraits<ZOffset>::tag) const {
    return z_;
  }

  // ---- Has ---- //
  bool CartesianPositionOffset::has(
      detail::ParameterTraits<XOffset>::tag) const {
    return true;
  }
  bool CartesianPositionOffset::has(
      detail::ParameterTraits<YOffset>::tag) const {
    return true;
  }
  bool CartesianPositionOffset::has(
      detail::ParameterTraits<ZOffset>::tag) const {
    return true;
  }

  // ---- Setter ---- //
  void CartesianPositionOffset::set(XOffset x) { x_ = x; }
  void CartesianPositionOffset::set(YOffset y) { y_ = y; }
  void CartesianPositionOffset::set(ZOffset z) { z_ = z; }

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
