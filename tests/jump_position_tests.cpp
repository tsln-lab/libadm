#include <catch2/catch.hpp>
#include "adm/elements/jump_position.hpp"

TEST_CASE("jump_position") {
  using namespace adm;
  {
    JumpPosition jumpPosition;

    REQUIRE(jumpPosition.has<JumpPositionFlag>() == true);
    REQUIRE(jumpPosition.has<InterpolationLength>() == true);

    REQUIRE(jumpPosition.isDefault<JumpPositionFlag>() == true);
    REQUIRE(jumpPosition.isDefault<InterpolationLength>() == true);

    REQUIRE(jumpPosition.get<JumpPositionFlag>() == false);
    REQUIRE(jumpPosition.get<InterpolationLength>().get() ==
            std::chrono::seconds(0));

    jumpPosition.set(JumpPositionFlag(true));
    jumpPosition.set(InterpolationLength(std::chrono::milliseconds(1500)));

    REQUIRE(jumpPosition.get<JumpPositionFlag>() == true);
    REQUIRE(jumpPosition.get<InterpolationLength>().get() ==
            std::chrono::milliseconds(1500));

    REQUIRE(jumpPosition.isDefault<JumpPositionFlag>() == false);
    REQUIRE(jumpPosition.isDefault<InterpolationLength>() == false);

    jumpPosition.unset<JumpPositionFlag>();
    jumpPosition.unset<InterpolationLength>();

    REQUIRE(jumpPosition.isDefault<JumpPositionFlag>() == true);
    REQUIRE(jumpPosition.isDefault<InterpolationLength>() == true);

    REQUIRE(isEnabled(jumpPosition) == false);
    enable(jumpPosition);
    REQUIRE(isEnabled(jumpPosition) == true);
    disable(jumpPosition);
    REQUIRE(isEnabled(jumpPosition) == false);
  }
}

TEST_CASE("jump_position/format_interpolation_length") {
  using namespace adm;
  REQUIRE(formatInterpolationLength(InterpolationLength(
              std::chrono::seconds(0))) == "0.00000");
  REQUIRE(formatInterpolationLength(InterpolationLength(
              std::chrono::milliseconds(1500))) == "1.50000");
  REQUIRE(formatInterpolationLength(InterpolationLength(
              std::chrono::microseconds(5208))) == "0.005208");
  REQUIRE(formatInterpolationLength(InterpolationLength(
              std::chrono::nanoseconds(123456789))) == "0.123456789");
}
