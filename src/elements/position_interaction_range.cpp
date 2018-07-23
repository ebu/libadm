#include "adm/elements/position_interaction_range.hpp"

namespace adm {

  // ---- SPHERICAL POSITION ---- //

  // ---- Getter ---- //
  AzimuthInteractionMin PositionInteractionRange::get(
      detail::ParameterTraits<AzimuthInteractionMin>::tag) const {
    return azimuthMin_.get();
  }
  AzimuthInteractionMax PositionInteractionRange::get(
      detail::ParameterTraits<AzimuthInteractionMax>::tag) const {
    return azimuthMax_.get();
  }
  ElevationInteractionMin PositionInteractionRange::get(
      detail::ParameterTraits<ElevationInteractionMin>::tag) const {
    return elevationMin_.get();
  }
  ElevationInteractionMax PositionInteractionRange::get(
      detail::ParameterTraits<ElevationInteractionMax>::tag) const {
    return elevationMax_.get();
  }
  DistanceInteractionMin PositionInteractionRange::get(
      detail::ParameterTraits<DistanceInteractionMin>::tag) const {
    return distanceMin_.get();
  }
  DistanceInteractionMax PositionInteractionRange::get(
      detail::ParameterTraits<DistanceInteractionMax>::tag) const {
    return distanceMax_.get();
  }

  // ---- Has ---- //
  bool PositionInteractionRange::has(
      detail::ParameterTraits<AzimuthInteractionMin>::tag) const {
    return azimuthMin_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<AzimuthInteractionMax>::tag) const {
    return azimuthMax_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<ElevationInteractionMin>::tag) const {
    return elevationMin_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<ElevationInteractionMax>::tag) const {
    return elevationMax_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<DistanceInteractionMin>::tag) const {
    return distanceMin_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<DistanceInteractionMax>::tag) const {
    return distanceMax_ != boost::none;
  }

  // ---- isDefault ---- //
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<AzimuthInteractionMin>::tag) const {
    return azimuthMin_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<AzimuthInteractionMax>::tag) const {
    return azimuthMax_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<ElevationInteractionMin>::tag) const {
    return elevationMin_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<ElevationInteractionMax>::tag) const {
    return elevationMax_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<DistanceInteractionMin>::tag) const {
    return distanceMin_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<DistanceInteractionMax>::tag) const {
    return distanceMax_ == boost::none;
  }

  // ---- Setter ---- //
  void PositionInteractionRange::set(AzimuthInteractionMin azimuthMin) {
    azimuthMin_ = azimuthMin;
  }
  void PositionInteractionRange::set(AzimuthInteractionMax azimuthMax) {
    azimuthMax_ = azimuthMax;
  }
  void PositionInteractionRange::set(ElevationInteractionMin elevationMin) {
    elevationMin_ = elevationMin;
  }
  void PositionInteractionRange::set(ElevationInteractionMax elevationMax) {
    elevationMax_ = elevationMax;
  }
  void PositionInteractionRange::set(DistanceInteractionMin distanceMin) {
    distanceMin_ = distanceMin;
  }
  void PositionInteractionRange::set(DistanceInteractionMax distanceMax) {
    distanceMax_ = distanceMax;
  }

  // ---- Unsetter ---- //
  void PositionInteractionRange::unset(
      detail::ParameterTraits<AzimuthInteractionMin>::tag) {
    azimuthMin_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<AzimuthInteractionMax>::tag) {
    azimuthMax_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<ElevationInteractionMin>::tag) {
    elevationMin_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<ElevationInteractionMax>::tag) {
    elevationMax_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<DistanceInteractionMin>::tag) {
    distanceMin_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<DistanceInteractionMax>::tag) {
    distanceMin_ = boost::none;
  }

  // ---- CARTESIAN POSITION ---- //

  // ---- Getter ---- //
  XInteractionMin PositionInteractionRange::get(
      detail::ParameterTraits<XInteractionMin>::tag) const {
    return xMin_.get();
  }
  XInteractionMax PositionInteractionRange::get(
      detail::ParameterTraits<XInteractionMax>::tag) const {
    return xMax_.get();
  }
  YInteractionMin PositionInteractionRange::get(
      detail::ParameterTraits<YInteractionMin>::tag) const {
    return yMin_.get();
  }
  YInteractionMax PositionInteractionRange::get(
      detail::ParameterTraits<YInteractionMax>::tag) const {
    return yMax_.get();
  }
  ZInteractionMin PositionInteractionRange::get(
      detail::ParameterTraits<ZInteractionMin>::tag) const {
    return zMin_.get();
  }
  ZInteractionMax PositionInteractionRange::get(
      detail::ParameterTraits<ZInteractionMax>::tag) const {
    return zMax_.get();
  }

  // ---- Has ---- //
  bool PositionInteractionRange::has(
      detail::ParameterTraits<XInteractionMin>::tag) const {
    return xMin_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<XInteractionMax>::tag) const {
    return xMax_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<YInteractionMin>::tag) const {
    return yMin_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<YInteractionMax>::tag) const {
    return yMax_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<ZInteractionMin>::tag) const {
    return zMin_ != boost::none;
  }
  bool PositionInteractionRange::has(
      detail::ParameterTraits<ZInteractionMax>::tag) const {
    return zMax_ != boost::none;
  }

  // ---- isDefault ---- //
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<XInteractionMin>::tag) const {
    return xMin_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<XInteractionMax>::tag) const {
    return xMax_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<YInteractionMin>::tag) const {
    return yMin_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<YInteractionMax>::tag) const {
    return yMax_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<ZInteractionMin>::tag) const {
    return zMin_ == boost::none;
  }
  bool PositionInteractionRange::isDefault(
      detail::ParameterTraits<ZInteractionMax>::tag) const {
    return zMax_ == boost::none;
  }

  // ---- Setter ---- //
  void PositionInteractionRange::set(XInteractionMin xMin) { xMin_ = xMin; }
  void PositionInteractionRange::set(XInteractionMax xMax) { xMax_ = xMax; }
  void PositionInteractionRange::set(YInteractionMin yMin) { yMin_ = yMin; }
  void PositionInteractionRange::set(YInteractionMax yMax) { yMax_ = yMax; }
  void PositionInteractionRange::set(ZInteractionMin zMin) { zMin_ = zMin; }
  void PositionInteractionRange::set(ZInteractionMax zMax) { zMax_ = zMax; }

  // ---- Unsetter ---- //
  void PositionInteractionRange::unset(
      detail::ParameterTraits<XInteractionMin>::tag) {
    xMin_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<XInteractionMax>::tag) {
    xMax_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<YInteractionMin>::tag) {
    yMin_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<YInteractionMax>::tag) {
    yMax_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<ZInteractionMin>::tag) {
    zMin_ = boost::none;
  }
  void PositionInteractionRange::unset(
      detail::ParameterTraits<ZInteractionMax>::tag) {
    zMax_ = boost::none;
  }

}  // namespace adm
