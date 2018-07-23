#include "adm/elements/speaker_position.hpp"

namespace adm {

  // ---- Getter ---- //
  Azimuth SpeakerPosition::get(detail::ParameterTraits<Azimuth>::tag) const {
    return azimuth_;
  }
  AzimuthMin SpeakerPosition::get(
      detail::ParameterTraits<AzimuthMin>::tag) const {
    return azimuthMin_.get();
  }
  AzimuthMax SpeakerPosition::get(
      detail::ParameterTraits<AzimuthMax>::tag) const {
    return azimuthMax_.get();
  }
  Elevation SpeakerPosition::get(
      detail::ParameterTraits<Elevation>::tag) const {
    return elevation_;
  }
  ElevationMin SpeakerPosition::get(
      detail::ParameterTraits<ElevationMin>::tag) const {
    return elevationMin_.get();
  }
  ElevationMax SpeakerPosition::get(
      detail::ParameterTraits<ElevationMax>::tag) const {
    return elevationMax_.get();
  }
  Distance SpeakerPosition::get(detail::ParameterTraits<Distance>::tag) const {
    return distance_.get();
  }
  DistanceMin SpeakerPosition::get(
      detail::ParameterTraits<DistanceMin>::tag) const {
    return distanceMin_.get();
  }
  DistanceMax SpeakerPosition::get(
      detail::ParameterTraits<DistanceMax>::tag) const {
    return distanceMax_.get();
  }
  ScreenEdgeLock SpeakerPosition::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.get();
  }

  // ---- Has ---- //
  bool SpeakerPosition::has(detail::ParameterTraits<Azimuth>::tag) const {
    return true;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<AzimuthMin>::tag) const {
    return azimuthMin_ != boost::none;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<AzimuthMax>::tag) const {
    return azimuthMax_ != boost::none;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<Elevation>::tag) const {
    return true;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<ElevationMin>::tag) const {
    return elevationMin_ != boost::none;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<ElevationMax>::tag) const {
    return elevationMax_ != boost::none;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<Distance>::tag) const {
    return distance_ != boost::none;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<DistanceMin>::tag) const {
    return distanceMin_ != boost::none;
  }
  bool SpeakerPosition::has(detail::ParameterTraits<DistanceMax>::tag) const {
    return distanceMax_ != boost::none;
  }
  bool SpeakerPosition::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != boost::none;
  }

  // ---- Setter ---- //
  void SpeakerPosition::set(Azimuth azimuth) { azimuth_ = azimuth; }
  void SpeakerPosition::set(AzimuthMin azimuthMin) { azimuthMin_ = azimuthMin; }
  void SpeakerPosition::set(AzimuthMax azimuthMax) { azimuthMax_ = azimuthMax; }
  void SpeakerPosition::set(Elevation elevation) { elevation_ = elevation; }
  void SpeakerPosition::set(ElevationMin elevationMin) {
    elevationMin_ = elevationMin;
  }
  void SpeakerPosition::set(ElevationMax elevationMax) {
    elevationMax_ = elevationMax;
  }
  void SpeakerPosition::set(Distance distance) { distance_ = distance; }
  void SpeakerPosition::set(DistanceMin distanceMin) {
    distanceMin_ = distanceMin;
  }
  void SpeakerPosition::set(DistanceMax distanceMax) {
    distanceMax_ = distanceMax;
  }
  void SpeakerPosition::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = ScreenEdgeLock(screenEdgeLock);
  }

  // ---- Unsetter ---- //
  void SpeakerPosition::unset(detail::ParameterTraits<AzimuthMin>::tag) {
    azimuthMin_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<AzimuthMax>::tag) {
    azimuthMax_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<ElevationMin>::tag) {
    elevationMin_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<ElevationMax>::tag) {
    elevationMax_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<Distance>::tag) {
    distance_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<DistanceMin>::tag) {
    distanceMin_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<DistanceMax>::tag) {
    distanceMax_ = boost::none;
  }
  void SpeakerPosition::unset(detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = boost::none;
  }

}  // namespace adm
