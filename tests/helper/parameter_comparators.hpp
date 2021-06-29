#pragma once
#include <adm/detail/optional_comparison.hpp>
#include <adm/elements_fwd.hpp>
#include <adm/elements/loudness_metadata.hpp>

namespace adm {
  bool operator==(ChannelLock const& lhs, ChannelLock const& rhs) {
    using detail::compareOptionals;
    return compareOptionals<ChannelLockFlag, MaxDistance>(lhs, rhs);
  }
  bool operator==(ObjectDivergence const& lhs, ObjectDivergence const& rhs) {
    using detail::compareOptionals;
    return compareOptionals<Divergence, AzimuthRange, PositionRange>(lhs, rhs);
  }
  bool operator==(JumpPosition const& lhs, JumpPosition const& rhs) {
    using detail::compareOptionals;
    return compareOptionals<JumpPositionFlag, InterpolationLength>(lhs, rhs);
  }
  bool operator==(HeadphoneVirtualise const& lhs,
                  HeadphoneVirtualise const& rhs) {
    using detail::compareOptionals;
    return compareOptionals<Bypass, DirectToReverberantRatio>(lhs, rhs);
  }
  bool operator==(LoudnessMetadata const& lhs, LoudnessMetadata const& rhs) {
    using detail::compareOptionals;
    return compareOptionals<LoudnessMethod, LoudnessRecType,
                            LoudnessCorrectionType, IntegratedLoudness,
                            LoudnessRange, MaxTruePeak, MaxMomentary,
                            MaxShortTerm, DialogueLoudness>(lhs, rhs);
  }
  bool operator==(Gain const& lhs, Gain const& rhs) {
    if (lhs.isDb() != rhs.isDb()) {
      return false;
    }
    if (lhs.isDb()) {
      return lhs.asDb() == rhs.asDb();
    }
    return lhs.asLinear() == rhs.asLinear();
  }
}  // namespace adm
