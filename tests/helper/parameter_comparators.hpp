#pragma once

namespace detail {
  template <typename ParameterT, typename ElementT>
  bool compareOptionalParameter(ElementT const& lhs, ElementT const& rhs) {
    // if one has the parameter and the other doesn't they're not equal
    if (lhs.template has<ParameterT>() != rhs.template has<ParameterT>()) {
      return false;
    }
    // either both have it or neither does. If neither, they're equal.
    if (!lhs.template has<ParameterT>()) {
      return true;
    }
    // otherwise both have the parameter, so compare.
    return lhs.template get<ParameterT>() == rhs.template get<ParameterT>();
  }

  template <typename FirstParam, typename... Params>
  struct Comparator {
    template <typename ElementT>
    static bool compareAll(ElementT const& lhs, ElementT const& rhs) {
      return (compareOptionalParameter<FirstParam>(lhs, rhs) &&
              Comparator<Params...>::compareAll(lhs, rhs));
    }
  };
  template <typename FirstParam>
  struct Comparator<FirstParam> {
    template <typename ElementT>
    static bool compareAll(ElementT const& lhs, ElementT const& rhs) {
      return compareOptionalParameter<FirstParam>(lhs, rhs);
    }
  };
  template <typename... Params, typename ElementT>
  bool compareOptionals(ElementT const& lhs, ElementT const& rhs) {
    return Comparator<Params...>::compareAll(lhs, rhs);
  }
}  // namespace detail

namespace adm {
  bool operator==(ChannelLock const& lhs, ChannelLock const& rhs) {
    using ::detail::compareOptionals;
    return compareOptionals<ChannelLockFlag, MaxDistance>(lhs, rhs);
  }
  bool operator==(ObjectDivergence const& lhs, ObjectDivergence const& rhs) {
    using ::detail::compareOptionals;
    return compareOptionals<Divergence, AzimuthRange, PositionRange>(lhs, rhs);
  }
  bool operator==(JumpPosition const& lhs, JumpPosition const& rhs) {
    using ::detail::compareOptionals;
    return compareOptionals<JumpPositionFlag, InterpolationLength>(lhs, rhs);
  }
  bool operator==(HeadphoneVirtualise const& lhs,
                  HeadphoneVirtualise const& rhs) {
    using ::detail::compareOptionals;
    return compareOptionals<Bypass, DirectToReverberantRatio>(lhs, rhs);
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
