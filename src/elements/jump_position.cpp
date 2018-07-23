#include "adm/elements/jump_position.hpp"

#include <iomanip>

namespace adm {

  // ---- Defaults ---- //
  const JumpPositionFlag JumpPosition::jumpPositionFlagDefault_ =
      JumpPositionFlag(false);
  const InterpolationLength JumpPosition::interpolationLengthDefault_ =
      InterpolationLength(std::chrono::seconds(0));

  // ---- Getter ---- //
  JumpPositionFlag JumpPosition::get(
      detail::ParameterTraits<JumpPositionFlag>::tag) const {
    return boost::get_optional_value_or(jumpPositionFlag_,
                                        jumpPositionFlagDefault_);
  }
  InterpolationLength JumpPosition::get(
      detail::ParameterTraits<InterpolationLength>::tag) const {
    return boost::get_optional_value_or(interpolationLength_,
                                        interpolationLengthDefault_);
  }

  // ---- Has ---- //
  bool JumpPosition::has(detail::ParameterTraits<JumpPositionFlag>::tag) const {
    return true;
  }
  bool JumpPosition::has(
      detail::ParameterTraits<InterpolationLength>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool JumpPosition::isDefault(
      detail::ParameterTraits<JumpPositionFlag>::tag) const {
    return jumpPositionFlag_ == boost::none;
  }
  bool JumpPosition::isDefault(
      detail::ParameterTraits<InterpolationLength>::tag) const {
    return interpolationLength_ == boost::none;
  }

  // ---- Setter ---- //
  void JumpPosition::set(JumpPositionFlag jumpPositionFlag) {
    jumpPositionFlag_ = jumpPositionFlag;
  }
  void JumpPosition::set(InterpolationLength interpolationLength) {
    interpolationLength_ = interpolationLength;
  }

  // ---- Unsetter ---- //
  void JumpPosition::unset(detail::ParameterTraits<JumpPositionFlag>::tag) {
    jumpPositionFlag_ = boost::none;
  }
  void JumpPosition::unset(detail::ParameterTraits<InterpolationLength>::tag) {
    interpolationLength_ = boost::none;
  }

  // ---- Free Functions ---- //
  bool isEnabled(JumpPosition &jumpPosition) {
    return jumpPosition.get<JumpPositionFlag>().get();
  }
  void enable(JumpPosition &jumpPosition) {
    return jumpPosition.set(JumpPositionFlag(true));
  }
  void disable(JumpPosition &jumpPosition) {
    return jumpPosition.set(JumpPositionFlag(false));
  }
  InterpolationLength parseInterpolationLength(const std::string &length) {
    auto floatTime = std::chrono::duration<float>(stof(length));
    return InterpolationLength(
        std::chrono::duration_cast<std::chrono::nanoseconds>(floatTime));
  }

  std::string formatInterpolationLength(const InterpolationLength length) {
    std::stringstream ss;
    ss << std::setprecision(5) << std::fixed
       << std::chrono::duration_cast<std::chrono::duration<float>>(length.get())
              .count();
    return ss.str();
  }

}  // namespace adm
