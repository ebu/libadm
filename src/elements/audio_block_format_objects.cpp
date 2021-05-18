#include "adm/elements/audio_block_format_objects.hpp"

namespace adm {

  // ---- Defaults ---- //
  namespace {
    const ChannelLock channelLockDefault{};
    const ObjectDivergence objectDivergenceDefault{};
    const JumpPosition jumpPositionDefault{};
    const ScreenRef screenRefDefault{false};
  }  // namespace

  namespace detail {
    template class RequiredParameter<AudioBlockFormatId>;
    template class DefaultParameter<Width>;
    template class DefaultParameter<Height>;
    template class DefaultParameter<Depth>;
    template class DefaultParameter<Diffuse>;
    template class DefaultParameter<HeadphoneVirtualise>;
  }  // namespace detail

  // ---- Getter ---- //
  Cartesian AudioBlockFormatObjects::get(
      detail::ParameterTraits<Cartesian>::tag) const {
    if (cartesian_ != boost::none) {
      return cartesian_.get();
    } else {
      if (has<SphericalPosition>()) {
        return Cartesian(false);
      } else {
        return Cartesian(true);
      }
    }
  }
  Position AudioBlockFormatObjects::get(
      detail::ParameterTraits<detail::ParameterTraits<Position>>::tag) const {
    if (has<SphericalPosition>()) {
      return Position(sphericalPosition_.get());
    } else {
      return Position(cartesianPosition_.get());
    }
  }
  SphericalPosition AudioBlockFormatObjects::get(
      detail::ParameterTraits<SphericalPosition>::tag) const {
    return sphericalPosition_.get();
  }
  CartesianPosition AudioBlockFormatObjects::get(
      detail::ParameterTraits<CartesianPosition>::tag) const {
    return cartesianPosition_.get();
  }
  ScreenEdgeLock AudioBlockFormatObjects::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.get();
  }
  ChannelLock AudioBlockFormatObjects::get(
      detail::ParameterTraits<ChannelLock>::tag) const {
    return boost::get_optional_value_or(channelLock_, channelLockDefault);
  }
  ObjectDivergence AudioBlockFormatObjects::get(
      detail::ParameterTraits<ObjectDivergence>::tag) const {
    return boost::get_optional_value_or(objectDivergence_,
                                        objectDivergenceDefault);
  }
  JumpPosition AudioBlockFormatObjects::get(
      detail::ParameterTraits<JumpPosition>::tag) const {
    return boost::get_optional_value_or(jumpPosition_, jumpPositionDefault);
  }
  ScreenRef AudioBlockFormatObjects::get(
      detail::ParameterTraits<ScreenRef>::tag) const {
    return boost::get_optional_value_or(screenRef_, screenRefDefault);
  }

  // ---- Has ---- //
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<Cartesian>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<detail::ParameterTraits<Position>>::tag) const {
    return sphericalPosition_ != boost::none ||
           cartesianPosition_ != boost::none;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<SphericalPosition>::tag) const {
    return sphericalPosition_ != boost::none;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<CartesianPosition>::tag) const {
    return cartesianPosition_ != boost::none;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != boost::none;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ChannelLock>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ObjectDivergence>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<JumpPosition>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ScreenRef>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Cartesian>::tag) const {
    return cartesian_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<ChannelLock>::tag) const {
    return channelLock_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<ObjectDivergence>::tag) const {
    return objectDivergence_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<JumpPosition>::tag) const {
    return jumpPosition_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<ScreenRef>::tag) const {
    return screenRef_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatObjects::set(Cartesian cartesian) {
    cartesian_ = cartesian;

    if (cartesian.get()) {
      sphericalPosition_ = boost::none;
      if (!cartesianPosition_) cartesianPosition_ = CartesianPosition();
    } else {
      cartesianPosition_ = boost::none;
      if (!sphericalPosition_) sphericalPosition_ = SphericalPosition();
    }
  }
  void AudioBlockFormatObjects::set(Position position) {
    if (position.which() == 0) {
      set(boost::get<SphericalPosition>(position));
    } else if (position.which() == 1) {
      set(boost::get<CartesianPosition>(position));
    }
  }
  void AudioBlockFormatObjects::set(SphericalPosition position) {
    sphericalPosition_ = position;
    cartesianPosition_ = boost::none;
    if (cartesian_ != boost::none) {
      cartesian_ = Cartesian(false);
    }
  }
  void AudioBlockFormatObjects::set(CartesianPosition position) {
    cartesianPosition_ = position;
    sphericalPosition_ = boost::none;
    cartesian_ = Cartesian(true);
  }
  void AudioBlockFormatObjects::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = screenEdgeLock;
  }
  void AudioBlockFormatObjects::set(ChannelLock channelLock) {
    channelLock_ = channelLock;
  }
  void AudioBlockFormatObjects::set(ObjectDivergence objectDivergence) {
    objectDivergence_ = objectDivergence;
  }
  void AudioBlockFormatObjects::set(JumpPosition jumpPosition) {
    jumpPosition_ = jumpPosition;
  }
  void AudioBlockFormatObjects::set(ScreenRef screenRef) {
    screenRef_ = screenRef;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Cartesian>::tag) {
    cartesian_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<detail::ParameterTraits<Position>>::tag) {
    sphericalPosition_ = boost::none;
    cartesianPosition_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<SphericalPosition>::tag) {
    sphericalPosition_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<CartesianPosition>::tag) {
    cartesianPosition_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ChannelLock>::tag) {
    channelLock_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ObjectDivergence>::tag) {
    objectDivergence_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<JumpPosition>::tag) {
    jumpPosition_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<ScreenRef>::tag) {
    screenRef_ = boost::none;
  }

}  // namespace adm
