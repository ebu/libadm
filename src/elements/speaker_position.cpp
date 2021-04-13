#include "adm/elements/speaker_position.hpp"

namespace adm {
  // ---- Defaults ---- //
  namespace {
    // N.B. these defaults are not currently specified in 2076 for
    // directspeaker block formats, but these sub-elements are optional,
    // so some default is required for rendering. These defaults match
    // those used in audioobject type audioBlockFormats
    const Distance defaultDistance = Distance{1.f};
    const Z defaultZ = Z{0.f};
  }  // namespace

  // ---- Getter ---- //
  X CartesianSpeakerPosition::get(detail::ParameterTraits<X>::tag) const {
    return x_;
  }
  XMin CartesianSpeakerPosition::get(detail::ParameterTraits<XMin>::tag) const {
    return xMin_.get();
  }
  XMax CartesianSpeakerPosition::get(detail::ParameterTraits<XMax>::tag) const {
    return xMax_.get();
  }
  Y CartesianSpeakerPosition::get(detail::ParameterTraits<Y>::tag) const {
    return y_;
  }
  YMin CartesianSpeakerPosition::get(detail::ParameterTraits<YMin>::tag) const {
    return yMin_.get();
  }
  YMax CartesianSpeakerPosition::get(detail::ParameterTraits<YMax>::tag) const {
    return yMax_.get();
  }
  Z CartesianSpeakerPosition::get(detail::ParameterTraits<Z>::tag) const {
    return boost::get_optional_value_or(z_, defaultZ);
  }
  ZMin CartesianSpeakerPosition::get(detail::ParameterTraits<ZMin>::tag) const {
    return zMin_.get();
  }
  ZMax CartesianSpeakerPosition::get(detail::ParameterTraits<ZMax>::tag) const {
    return zMax_.get();
  }
  ScreenEdgeLock CartesianSpeakerPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.get();
  }

  // ---- Has ---- //
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<X>::tag) const {
    return true;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<XMin>::tag) const {
    return xMin_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<XMax>::tag) const {
    return xMax_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<Y>::tag) const {
    return true;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<YMin>::tag) const {
    return yMin_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<YMax>::tag) const {
    return yMax_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<Z>::tag) const {
    return z_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<ZMin>::tag) const {
    return zMin_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(detail::ParameterTraits<ZMax>::tag) const {
    return zMax_ != boost::none;
  }
  bool CartesianSpeakerPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != boost::none;
  }

  bool CartesianSpeakerPosition::isDefault(
      detail::ParameterTraits<Z>::tag) const {
    return !has<Z>();
  }

  // ---- Setter ---- //
  void CartesianSpeakerPosition::set(X x) { x_ = x; }
  void CartesianSpeakerPosition::set(XMin xMin) { xMin_ = xMin; }
  void CartesianSpeakerPosition::set(XMax xMax) { xMax_ = xMax; }
  void CartesianSpeakerPosition::set(Y y) { y_ = y; }
  void CartesianSpeakerPosition::set(YMin yMin) { yMin_ = yMin; }
  void CartesianSpeakerPosition::set(YMax yMax) { yMax_ = yMax; }
  void CartesianSpeakerPosition::set(Z z) { z_ = z; }
  void CartesianSpeakerPosition::set(ZMin zMin) { zMin_ = zMin; }
  void CartesianSpeakerPosition::set(ZMax zMax) { zMax_ = zMax; }
  void CartesianSpeakerPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = ScreenEdgeLock(screenEdgeLock);
  }

  // ---- Unsetter ---- //
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<XMin>::tag) {
    xMin_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<XMax>::tag) {
    xMax_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<YMin>::tag) {
    yMin_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<YMax>::tag) {
    yMax_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<Z>::tag) {
    z_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<ZMin>::tag) {
    zMin_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(detail::ParameterTraits<ZMax>::tag) {
    zMax_ = boost::none;
  }
  void CartesianSpeakerPosition::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = boost::none;
  }

  // ---- Getter ---- //
  Azimuth SphericalSpeakerPosition::get(
      detail::ParameterTraits<Azimuth>::tag) const {
    return azimuth_;
  }
  AzimuthMin SphericalSpeakerPosition::get(
      detail::ParameterTraits<AzimuthMin>::tag) const {
    return azimuthMin_.get();
  }
  AzimuthMax SphericalSpeakerPosition::get(
      detail::ParameterTraits<AzimuthMax>::tag) const {
    return azimuthMax_.get();
  }
  Elevation SphericalSpeakerPosition::get(
      detail::ParameterTraits<Elevation>::tag) const {
    return elevation_;
  }
  ElevationMin SphericalSpeakerPosition::get(
      detail::ParameterTraits<ElevationMin>::tag) const {
    return elevationMin_.get();
  }
  ElevationMax SphericalSpeakerPosition::get(
      detail::ParameterTraits<ElevationMax>::tag) const {
    return elevationMax_.get();
  }
  Distance SphericalSpeakerPosition::get(
      detail::ParameterTraits<Distance>::tag) const {
    return boost::get_optional_value_or(distance_, defaultDistance);
  }
  DistanceMin SphericalSpeakerPosition::get(
      detail::ParameterTraits<DistanceMin>::tag) const {
    return distanceMin_.get();
  }
  DistanceMax SphericalSpeakerPosition::get(
      detail::ParameterTraits<DistanceMax>::tag) const {
    return distanceMax_.get();
  }
  ScreenEdgeLock SphericalSpeakerPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.get();
  }

  // ---- Has ---- //
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<Azimuth>::tag) const {
    return true;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<AzimuthMin>::tag) const {
    return azimuthMin_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<AzimuthMax>::tag) const {
    return azimuthMax_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<Elevation>::tag) const {
    return true;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<ElevationMin>::tag) const {
    return elevationMin_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<ElevationMax>::tag) const {
    return elevationMax_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<Distance>::tag) const {
    return distance_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<DistanceMin>::tag) const {
    return distanceMin_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<DistanceMax>::tag) const {
    return distanceMax_ != boost::none;
  }
  bool SphericalSpeakerPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != boost::none;
  }

  bool SphericalSpeakerPosition::isDefault(
      detail::ParameterTraits<Distance>::tag) const {
    return !has<Distance>();
  }

  // ---- Setter ---- //
  void SphericalSpeakerPosition::set(Azimuth azimuth) { azimuth_ = azimuth; }
  void SphericalSpeakerPosition::set(AzimuthMin azimuthMin) {
    azimuthMin_ = azimuthMin;
  }
  void SphericalSpeakerPosition::set(AzimuthMax azimuthMax) {
    azimuthMax_ = azimuthMax;
  }
  void SphericalSpeakerPosition::set(Elevation elevation) {
    elevation_ = elevation;
  }
  void SphericalSpeakerPosition::set(ElevationMin elevationMin) {
    elevationMin_ = elevationMin;
  }
  void SphericalSpeakerPosition::set(ElevationMax elevationMax) {
    elevationMax_ = elevationMax;
  }
  void SphericalSpeakerPosition::set(Distance distance) {
    distance_ = distance;
  }
  void SphericalSpeakerPosition::set(DistanceMin distanceMin) {
    distanceMin_ = distanceMin;
  }
  void SphericalSpeakerPosition::set(DistanceMax distanceMax) {
    distanceMax_ = distanceMax;
  }
  void SphericalSpeakerPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = ScreenEdgeLock(screenEdgeLock);
  }

  // ---- Unsetter ---- //
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<AzimuthMin>::tag) {
    azimuthMin_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<AzimuthMax>::tag) {
    azimuthMax_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<ElevationMin>::tag) {
    elevationMin_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<ElevationMax>::tag) {
    elevationMax_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(detail::ParameterTraits<Distance>::tag) {
    distance_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<DistanceMin>::tag) {
    distanceMin_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<DistanceMax>::tag) {
    distanceMax_ = boost::none;
  }
  void SphericalSpeakerPosition::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = boost::none;
  }

}  // namespace adm
