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
    // seconds with five decimals, and up to nine when the value needs them
    // (the Dolby Atmos Master ADM Profile's 0.005208 is not 0.00521)
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(length.get())
                  .count();
    std::stringstream ss;
    if (ns < 0) {
      ns = -ns;
      ss << '-';
    }
    auto fraction = ns % 1000000000;
    int precision = 9;
    while (fraction % 10 == 0 && precision > 5) {
      fraction /= 10;
      precision--;
    }
    ss << ns / 1000000000 << '.' << std::setw(precision) << std::setfill('0')
       << fraction;
    return ss.str();
  }

}  // namespace adm
