#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_objects.hpp"
#include "helper/parameter_checks.hpp"
#include "helper/ostream_operators.hpp"

using namespace adm;

TEST_CASE("audio_block_format_objects_parameters") {
  using namespace adm_test;
  using std::chrono::seconds;
  AudioBlockFormatObjects blockFormat{SphericalPosition()};
  // see adm_id_tests.cpp for more detailed ID testing, this is just to check
  // expected parameter behaviour on AudioBlockFormatObjects
  SECTION("AudioBlockFormatID") {
    AudioBlockFormatId defaultId{};
    AudioBlockFormatId customId{AudioBlockFormatIdValue{1}};
    check_required_param<AudioBlockFormatId>(
        blockFormat, hasDefaultOf(defaultId), canBeSetTo(customId));
  }
  SECTION("Rtime") {
    check_defaulted_param<Rtime>(blockFormat, hasDefaultOf(seconds{0}),
                                 canBeSetTo(seconds{2}));
  }
  SECTION("Duration") {
    check_optional_param<Duration>(blockFormat, canBeSetTo(seconds{10}));
  }
  SECTION("Width") {
    check_defaulted_param<Width>(blockFormat, hasDefaultOf(.0f),
                                 canBeSetTo(45.f));
  }
  SECTION("Height") {
    check_defaulted_param<Height>(blockFormat, hasDefaultOf(.0f),
                                  canBeSetTo(20.0f));
  }
  SECTION("Depth") {
    check_defaulted_param<Depth>(blockFormat, hasDefaultOf(.0f),
                                 canBeSetTo(.2f));
  }
  // TODO move to common tests
  SECTION("Gain") {
    check_defaulted_param<Gain>(blockFormat,
                                hasDefaultOf(Gain::fromLinear(1.0f)),
                                canBeSetTo(Gain::fromLinear(2.5f)));
  }
  SECTION("Diffuse") {
    check_defaulted_param<Diffuse>(blockFormat, hasDefaultOf(0.0f),
                                   canBeSetTo(0.5f));
  }
  SECTION("ChannelLock") {
    auto defaultChannelLock = ChannelLock{};
    auto customChannelLock =
        ChannelLock{ChannelLockFlag{true}, MaxDistance{2.0f}};
    check_defaulted_param<ChannelLock>(blockFormat,
                                       hasDefaultOf(defaultChannelLock),
                                       canBeSetTo(customChannelLock));
  }
  SECTION("ChannelLock sub-elements") {
    auto channelLock = ChannelLock{};
    SECTION("ChannelLockFlag") {
      check_defaulted_param<ChannelLockFlag>(channelLock, hasDefaultOf(false),
                                             canBeSetTo(true));
    }
    SECTION("MaxDistance") {
      check_optional_param<MaxDistance>(channelLock, canBeSetTo(2.0f));
    }
  }
  SECTION("ObjectDivergence") {
    auto defaultDivergence = ObjectDivergence{};
    auto customDivergence = ObjectDivergence{Divergence{0.2f}};
    check_defaulted_param<ObjectDivergence>(blockFormat,
                                            hasDefaultOf(defaultDivergence),
                                            canBeSetTo(customDivergence));
  }
  SECTION("ObjectDivergence sub-elements") {
    auto objectDivergence = ObjectDivergence{};
    SECTION("Divergence") {
      check_defaulted_param<Divergence>(objectDivergence, hasDefaultOf(0.0f),
                                        canBeSetTo(1.0f));
    }
    SECTION("Azimuth Range") {
      check_defaulted_param<AzimuthRange>(objectDivergence, hasDefaultOf(45.0f),
                                          canBeSetTo(1.0f));
    }
    SECTION("Position Range") {
      check_defaulted_param<PositionRange>(objectDivergence, hasDefaultOf(0.f),
                                           canBeSetTo(1.0f));
    }
  }
  SECTION("JumpPosition") {
    auto defaultJumpPosition = JumpPosition{};
    auto customJumpPosition = JumpPosition{JumpPositionFlag{true}};
    check_defaulted_param<JumpPosition>(blockFormat,
                                        hasDefaultOf(defaultJumpPosition),
                                        canBeSetTo(customJumpPosition));
  }
  SECTION("JumpPosition sub-elements") {
    auto jumpPosition = JumpPosition{};
    check_defaulted_param<JumpPositionFlag>(jumpPosition, hasDefaultOf(false),
                                            canBeSetTo(true));
    check_defaulted_param<InterpolationLength>(
        jumpPosition, hasDefaultOf(seconds(0)), canBeSetTo(seconds(20)));
  }
  SECTION("ScreenRef") {
    check_defaulted_param<ScreenRef>(blockFormat, hasDefaultOf(false),
                                     canBeSetTo(true));
  }
  // TODO move to common tests
  SECTION("Importance") {
    check_defaulted_param<Importance>(blockFormat, hasDefaultOf(10),
                                      canBeSetTo(1));
  }
  SECTION("Cartesian") {
    check_defaulted_param<Cartesian>(blockFormat, hasDefaultOf(false),
                                     canBeSetTo(true));
  }
}

TEST_CASE(
    "with an audio_block_format_objects with spherical position and default "
    "cartesian flag") {
  AudioBlockFormatObjects block{SphericalPosition{Azimuth{10.0f}}};
  SECTION("setting Cartesian true") {
    block.set(Cartesian{true});
    SECTION("causes cartesian position to be set") {
      REQUIRE(block.has<SphericalPosition>() == false);
      REQUIRE(block.has<CartesianPosition>() == true);
    }
    SECTION("and then unsetting") {
      block.unset<Cartesian>();
      SECTION("causes spherical position to be set") {
        REQUIRE(block.has<CartesianPosition>() == false);
        REQUIRE(block.has<SphericalPosition>() == true);
      }
    }
  }
  SECTION("setting Cartesian false") {
    block.set(Cartesian{false});
    SECTION("does not change position") {
      REQUIRE(block.has<SphericalPosition>());
      REQUIRE(block.get<SphericalPosition>().get<Azimuth>() == 10.0f);
    }
    SECTION("and then unsetting") {
      block.unset<Cartesian>();
      SECTION("does not change position") {
        REQUIRE(block.has<SphericalPosition>());
        REQUIRE(block.get<SphericalPosition>().get<Azimuth>() == 10.0f);
      }
    }
  }
}

TEST_CASE("audio_block_format_objects spherical") {
  SphericalPosition pos(Azimuth(10.0f), Elevation(0.0f), Distance(1.0f));

  // explicitly set cartesian to default -> no change to position
  {
    AudioBlockFormatObjects block_format(pos, Cartesian(false));
    REQUIRE(block_format.get<SphericalPosition>().get<Azimuth>().get() ==
            10.0f);
    REQUIRE(block_format.has<CartesianPosition>() == false);
    REQUIRE(block_format.get<Cartesian>().get() == false);
    REQUIRE(block_format.isDefault<Cartesian>() == false);
  }

  // explicitly set cartesian to non-default -> default position of right type
  {
    AudioBlockFormatObjects block_format(pos, Cartesian(true));
    REQUIRE(block_format.has<SphericalPosition>() == false);
    REQUIRE(block_format.has<CartesianPosition>() == true);
    REQUIRE(block_format.get<Cartesian>().get() == true);
    REQUIRE(block_format.isDefault<Cartesian>() == false);
  }

  // no set of cartesian -> cartesian is default
  {
    AudioBlockFormatObjects block_format(pos);
    REQUIRE(block_format.get<SphericalPosition>().get<Azimuth>().get() ==
            10.0f);
    REQUIRE(block_format.has<CartesianPosition>() == false);
    REQUIRE(block_format.get<Cartesian>().get() == false);
    REQUIRE(block_format.isDefault<Cartesian>() == true);
  }
}

TEST_CASE("audio_block_format_objects cartesian") {
  CartesianPosition pos(X(0.1f), Y(0.2f), Z(0.3f));

  // explicitly set cartesian to default -> no change to position
  {
    AudioBlockFormatObjects block_format(pos, Cartesian(true));
    REQUIRE(block_format.get<CartesianPosition>().get<X>().get() == 0.1f);
    REQUIRE(block_format.has<SphericalPosition>() == false);
    REQUIRE(block_format.get<Cartesian>().get() == true);
    REQUIRE(block_format.isDefault<Cartesian>() == false);
  }

  // explicitly set cartesian to non-default -> default position of right type
  {
    AudioBlockFormatObjects block_format(pos, Cartesian(false));
    REQUIRE(block_format.has<CartesianPosition>() == false);
    REQUIRE(block_format.has<SphericalPosition>() == true);
    REQUIRE(block_format.get<Cartesian>().get() == false);
    REQUIRE(block_format.isDefault<Cartesian>() == false);
  }

  // no set of cartesian -> still not default
  {
    AudioBlockFormatObjects block_format(pos);
    REQUIRE(block_format.get<CartesianPosition>().get<X>().get() == 0.1f);
    REQUIRE(block_format.has<SphericalPosition>() == false);
    REQUIRE(block_format.get<Cartesian>().get() == true);
    REQUIRE(block_format.isDefault<Cartesian>() == false);
  }
}
