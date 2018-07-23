#include "adm/elements/audio_block_format_objects.hpp"

namespace adm {

  // ---- Defaults ---- //
  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
    const Width widthDefault{0.f};
    const Height heightDefault{0.f};
    const Depth depthDefault{0.f};
    const Gain gainDefault{1.f};
    const Diffuse diffuseDefault{0.f};
    const ChannelLock channelLockDefault{};
    const ObjectDivergence objectDivergenceDefault{};
    const JumpPosition jumpPositionDefault{};
    const ScreenRef screenRefDefault{false};
    const Importance importanceDefault{10};
  }  // namespace

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatObjects::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatObjects::get(
      detail::ParameterTraits<Rtime>::tag) const {
    return boost::get_optional_value_or(rtime_, rtimeDefault);
  }
  Duration AudioBlockFormatObjects::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }
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
  Width AudioBlockFormatObjects::get(
      detail::ParameterTraits<Width>::tag) const {
    return boost::get_optional_value_or(width_, widthDefault);
  }
  Height AudioBlockFormatObjects::get(
      detail::ParameterTraits<Height>::tag) const {
    return boost::get_optional_value_or(height_, heightDefault);
  }
  Depth AudioBlockFormatObjects::get(
      detail::ParameterTraits<Depth>::tag) const {
    return boost::get_optional_value_or(depth_, depthDefault);
  }
  ScreenEdgeLock AudioBlockFormatObjects::get(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_.get();
  }
  Gain AudioBlockFormatObjects::get(detail::ParameterTraits<Gain>::tag) const {
    return boost::get_optional_value_or(gain_, gainDefault);
  }
  Diffuse AudioBlockFormatObjects::get(
      detail::ParameterTraits<Diffuse>::tag) const {
    return boost::get_optional_value_or(diffuse_, diffuseDefault);
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
  Importance AudioBlockFormatObjects::get(
      detail::ParameterTraits<Importance>::tag) const {
    return boost::get_optional_value_or(importance_, importanceDefault);
  }

  // ---- Has ---- //
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }
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
  bool AudioBlockFormatObjects::has(detail::ParameterTraits<Width>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<Height>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(detail::ParameterTraits<Depth>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<ScreenEdgeLock>::tag) const {
    return screenEdgeLock_ != boost::none;
  }
  bool AudioBlockFormatObjects::has(detail::ParameterTraits<Gain>::tag) const {
    return true;
  }
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<Diffuse>::tag) const {
    return true;
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
  bool AudioBlockFormatObjects::has(
      detail::ParameterTraits<Importance>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return rtime_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Cartesian>::tag) const {
    return cartesian_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Width>::tag) const {
    return width_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Height>::tag) const {
    return height_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Depth>::tag) const {
    return depth_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Gain>::tag) const {
    return gain_ == boost::none;
  }
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Diffuse>::tag) const {
    return diffuse_ == boost::none;
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
  bool AudioBlockFormatObjects::isDefault(
      detail::ParameterTraits<Importance>::tag) const {
    return importance_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatObjects::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatObjects::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatObjects::set(Duration duration) { duration_ = duration; }
  void AudioBlockFormatObjects::set(Cartesian cartesian) {
    if (isDefault<Cartesian>() || get<Cartesian>() != cartesian) {
      cartesian_ = cartesian;
      if (cartesian == true) {
        set(CartesianPosition());
      } else {
        set(SphericalPosition());
      }
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
    unset<CartesianPosition>();
    if (!isDefault<Cartesian>()) {
      set(Cartesian(false));
    }
  }
  void AudioBlockFormatObjects::set(CartesianPosition position) {
    cartesianPosition_ = position;
    unset<SphericalPosition>();
    set(Cartesian(true));
  }
  void AudioBlockFormatObjects::set(Width width) { width_ = width; }
  void AudioBlockFormatObjects::set(Height height) { height_ = height; }
  void AudioBlockFormatObjects::set(Depth depth) { depth_ = depth; }
  void AudioBlockFormatObjects::set(ScreenEdgeLock screenEdgeLock) {
    screenEdgeLock_ = screenEdgeLock;
  }
  void AudioBlockFormatObjects::set(Gain gain) { gain_ = gain; }
  void AudioBlockFormatObjects::set(Diffuse diffuse) { diffuse_ = diffuse; }
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
  void AudioBlockFormatObjects::set(Importance importance) {
    importance_ = importance;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }
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
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Width>::tag) {
    width_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Height>::tag) {
    height_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Depth>::tag) {
    depth_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<ScreenEdgeLock>::tag) {
    screenEdgeLock_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Gain>::tag) {
    gain_ = boost::none;
  }
  void AudioBlockFormatObjects::unset(detail::ParameterTraits<Diffuse>::tag) {
    diffuse_ = boost::none;
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
  void AudioBlockFormatObjects::unset(
      detail::ParameterTraits<Importance>::tag) {
    importance_ = boost::none;
  }

}  // namespace adm
