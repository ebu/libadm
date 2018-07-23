#include "adm/elements/channel_lock.hpp"

namespace adm {

  // ---- Defaults ---- //
  const ChannelLockFlag ChannelLock::channelLockFlagDefault_ =
      ChannelLockFlag(false);

  // ---- Getter ---- //
  ChannelLockFlag ChannelLock::get(
      detail::ParameterTraits<ChannelLockFlag>::tag) const {
    return boost::get_optional_value_or(channelLockFlag_,
                                        channelLockFlagDefault_);
  }
  MaxDistance ChannelLock::get(
      detail::ParameterTraits<MaxDistance>::tag) const {
    return maxDistance_.get();
  }

  // ---- Has ---- //
  bool ChannelLock::has(detail::ParameterTraits<ChannelLockFlag>::tag) const {
    return true;
  }
  bool ChannelLock::has(detail::ParameterTraits<MaxDistance>::tag) const {
    return maxDistance_ != boost::none;
  }

  // ---- isDefault ---- //
  bool ChannelLock::isDefault(
      detail::ParameterTraits<ChannelLockFlag>::tag) const {
    return channelLockFlag_ == boost::none;
  }

  // ---- Setter ---- //
  void ChannelLock::set(ChannelLockFlag channelLockFlag) {
    channelLockFlag_ = channelLockFlag;
  }
  void ChannelLock::set(MaxDistance maxDistance) { maxDistance_ = maxDistance; }

  // ---- Unsetter ---- //
  void ChannelLock::unset(detail::ParameterTraits<ChannelLockFlag>::tag) {
    channelLockFlag_ = boost::none;
  }
  void ChannelLock::unset(detail::ParameterTraits<MaxDistance>::tag) {
    maxDistance_ = boost::none;
  }

  // ---- Free Functions ---- //
  bool isEnabled(ChannelLock &channelLock) {
    return channelLock.get<ChannelLockFlag>().get();
  }
  void enable(ChannelLock &channelLock) {
    return channelLock.set(ChannelLockFlag(true));
  }
  void disable(ChannelLock &channelLock) {
    return channelLock.set(ChannelLockFlag(false));
  }

}  // namespace adm
