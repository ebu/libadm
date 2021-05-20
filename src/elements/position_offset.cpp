#include "adm/elements/position_offset.hpp"

namespace adm {

  // ---- SPHERICAL POSITION OFFSET ---- //

  // ---- Constructor ---- //

  // ---- Defaults ---- //
  const AzimuthOffset SphericalPositionOffset::azimuthDefault_ =
      AzimuthOffset(0.f);
  const ElevationOffset SphericalPositionOffset::elevationDefault_ =
      ElevationOffset(0.f);
  const DistanceOffset SphericalPositionOffset::distanceDefault_ =
      DistanceOffset(0.f);

  // ---- Getter ---- //
  AzimuthOffset SphericalPositionOffset::get(
      detail::ParameterTraits<AzimuthOffset>::tag) const {
    return boost::get_optional_value_or(azimuth_, azimuthDefault_);
  }
  ElevationOffset SphericalPositionOffset::get(
      detail::ParameterTraits<ElevationOffset>::tag) const {
    return boost::get_optional_value_or(elevation_, elevationDefault_);
  }
  DistanceOffset SphericalPositionOffset::get(
      detail::ParameterTraits<DistanceOffset>::tag) const {
    return boost::get_optional_value_or(distance_, distanceDefault_);
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

  // ---- isDefault ---- //
  bool SphericalPositionOffset::isDefault(
      detail::ParameterTraits<AzimuthOffset>::tag) const {
    return azimuth_ == boost::none;
  }
  bool SphericalPositionOffset::isDefault(
      detail::ParameterTraits<ElevationOffset>::tag) const {
    return elevation_ == boost::none;
  }
  bool SphericalPositionOffset::isDefault(
      detail::ParameterTraits<DistanceOffset>::tag) const {
    return distance_ == boost::none;
  }

  // --- Unsetter --- //
  void SphericalPositionOffset::unset(
      detail::ParameterTraits<AzimuthOffset>::tag) {
    azimuth_ = boost::none;
  }
  void SphericalPositionOffset::unset(
      detail::ParameterTraits<ElevationOffset>::tag) {
    elevation_ = boost::none;
  }
  void SphericalPositionOffset::unset(
      detail::ParameterTraits<DistanceOffset>::tag) {
    distance_ = boost::none;
  }

  // ---- CARTESIAN POSITION OFFSET ---- //

  // ---- Constructor ---- //

  // ---- Defaults ---- //
  const XOffset CartesianPositionOffset::xDefault_ = XOffset(0.f);
  const YOffset CartesianPositionOffset::yDefault_ = YOffset(0.f);
  const ZOffset CartesianPositionOffset::zDefault_ = ZOffset(0.f);

  // ---- Getter ---- //
  XOffset CartesianPositionOffset::get(
      detail::ParameterTraits<XOffset>::tag) const {
    return boost::get_optional_value_or(x_, xDefault_);
  }
  YOffset CartesianPositionOffset::get(
      detail::ParameterTraits<YOffset>::tag) const {
    return boost::get_optional_value_or(y_, yDefault_);
  }
  ZOffset CartesianPositionOffset::get(
      detail::ParameterTraits<ZOffset>::tag) const {
    return boost::get_optional_value_or(z_, zDefault_);
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

  // ---- isDefault ---- //
  bool CartesianPositionOffset::isDefault(
      detail::ParameterTraits<XOffset>::tag) const {
    return x_ == boost::none;
  }
  bool CartesianPositionOffset::isDefault(
      detail::ParameterTraits<YOffset>::tag) const {
    return y_ == boost::none;
  }
  bool CartesianPositionOffset::isDefault(
      detail::ParameterTraits<ZOffset>::tag) const {
    return z_ == boost::none;
  }

  // --- Unsetter --- //
  void CartesianPositionOffset::unset(detail::ParameterTraits<XOffset>::tag) {
    x_ = boost::none;
  }
  void CartesianPositionOffset::unset(detail::ParameterTraits<YOffset>::tag) {
    y_ = boost::none;
  }
  void CartesianPositionOffset::unset(detail::ParameterTraits<ZOffset>::tag) {
    z_ = boost::none;
  }

  // ---- FREE FUNCTIONS ---- //

  bool isSpherical(const PositionOffset& offset) {
    return offset.type() == typeid(SphericalPositionOffset);
  }

  bool isCartesian(const PositionOffset& offset) {
    return offset.type() == typeid(CartesianPositionOffset);
  }

}  // namespace adm
