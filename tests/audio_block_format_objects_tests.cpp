#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_objects.hpp"

TEST_CASE("audio_block_format_objects") {
  using namespace adm;
  {
    AudioBlockFormatObjects blockFormat{SphericalPosition()};

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);
    REQUIRE(blockFormat.has<Cartesian>() == true);
    REQUIRE(blockFormat.has<Width>() == true);
    REQUIRE(blockFormat.has<Height>() == true);
    REQUIRE(blockFormat.has<Depth>() == true);
    REQUIRE(blockFormat.has<Gain>() == true);
    REQUIRE(blockFormat.has<Diffuse>() == true);
    REQUIRE(blockFormat.has<ChannelLock>() == true);
    REQUIRE(blockFormat.has<ObjectDivergence>() == true);
    REQUIRE(blockFormat.has<JumpPosition>() == true);
    REQUIRE(blockFormat.has<ScreenRef>() == true);
    REQUIRE(blockFormat.has<Importance>() == true);

    REQUIRE(blockFormat.isDefault<AudioBlockFormatId>() == false);
    REQUIRE(blockFormat.isDefault<Rtime>() == true);
    REQUIRE(blockFormat.isDefault<Duration>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == true);
    REQUIRE(blockFormat.isDefault<Width>() == true);
    REQUIRE(blockFormat.isDefault<Height>() == true);
    REQUIRE(blockFormat.isDefault<Depth>() == true);
    REQUIRE(blockFormat.isDefault<Gain>() == true);
    REQUIRE(blockFormat.isDefault<Diffuse>() == true);
    REQUIRE(blockFormat.isDefault<ChannelLock>() == true);
    REQUIRE(blockFormat.isDefault<ObjectDivergence>() == true);
    REQUIRE(blockFormat.isDefault<JumpPosition>() == true);
    REQUIRE(blockFormat.isDefault<ScreenRef>() == true);
    REQUIRE(blockFormat.isDefault<Importance>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));
    REQUIRE(blockFormat.get<Width>() == 0.f);
    REQUIRE(blockFormat.get<Height>() == 0.f);
    REQUIRE(blockFormat.get<Depth>() == 0.f);
    REQUIRE(blockFormat.get<Gain>() == 1.f);
    REQUIRE(blockFormat.get<Diffuse>() == 0.f);
    REQUIRE(blockFormat.get<ChannelLock>().get<ChannelLockFlag>() == false);
    REQUIRE(blockFormat.get<ObjectDivergence>().get<Divergence>() == 0.f);
    REQUIRE(blockFormat.get<ObjectDivergence>().get<AzimuthRange>() == 45.f);
    REQUIRE(blockFormat.get<ObjectDivergence>().get<PositionRange>() == 0.f);
    REQUIRE(blockFormat.get<JumpPosition>().get<JumpPositionFlag>() == false);
    REQUIRE(blockFormat.get<JumpPosition>().get<InterpolationLength>().get() ==
            std::chrono::seconds(0));

    blockFormat.set(Rtime(std::chrono::seconds(2)));
    blockFormat.set(Duration(std::chrono::seconds(10)));
    blockFormat.set(Cartesian(false));
    blockFormat.set(Width(45.f));
    blockFormat.set(Height(20.f));
    blockFormat.set(Depth(0.2f));
    blockFormat.set(Gain(2.5f));
    blockFormat.set(Diffuse(0.5f));
    blockFormat.set(ChannelLock(ChannelLockFlag(true)));
    blockFormat.set(ObjectDivergence(Divergence(0.2f)));
    blockFormat.set(
        JumpPosition(JumpPositionFlag(true),
                     InterpolationLength(std::chrono::milliseconds(20))));
    blockFormat.set(ScreenRef(true));
    blockFormat.set(Importance(10));

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(2));
    REQUIRE(blockFormat.get<Duration>().get() == std::chrono::seconds(10));
    REQUIRE(blockFormat.get<Cartesian>() == false);
    REQUIRE(blockFormat.get<Width>() == 45.f);
    REQUIRE(blockFormat.get<Height>() == 20.f);
    REQUIRE(blockFormat.get<Depth>() == 0.2f);
    REQUIRE(blockFormat.get<Gain>() == 2.5f);
    REQUIRE(blockFormat.get<Diffuse>() == 0.5f);
    REQUIRE(blockFormat.get<ChannelLock>().get<ChannelLockFlag>() == true);
    REQUIRE(blockFormat.get<ObjectDivergence>().get<Divergence>().get() ==
            Approx(0.2));
    REQUIRE(blockFormat.get<JumpPosition>().get<JumpPositionFlag>() == true);
    REQUIRE(blockFormat.get<JumpPosition>().get<InterpolationLength>().get() ==
            std::chrono::milliseconds(20));
    REQUIRE(blockFormat.get<ScreenRef>() == true);
    REQUIRE(blockFormat.get<Importance>() == 10);

    REQUIRE(blockFormat.isDefault<AudioBlockFormatId>() == false);
    REQUIRE(blockFormat.isDefault<Rtime>() == false);
    REQUIRE(blockFormat.isDefault<Duration>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == false);
    REQUIRE(blockFormat.isDefault<Width>() == false);
    REQUIRE(blockFormat.isDefault<Height>() == false);
    REQUIRE(blockFormat.isDefault<Depth>() == false);
    REQUIRE(blockFormat.isDefault<Gain>() == false);
    REQUIRE(blockFormat.isDefault<Diffuse>() == false);
    REQUIRE(blockFormat.isDefault<ChannelLock>() == false);
    REQUIRE(blockFormat.isDefault<ObjectDivergence>() == false);
    REQUIRE(blockFormat.isDefault<JumpPosition>() == false);
    REQUIRE(blockFormat.isDefault<ScreenRef>() == false);
    REQUIRE(blockFormat.isDefault<Importance>() == false);

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();
    blockFormat.unset<Cartesian>();
    blockFormat.unset<Width>();
    blockFormat.unset<Height>();
    blockFormat.unset<Depth>();
    blockFormat.unset<Gain>();
    blockFormat.unset<Diffuse>();
    blockFormat.unset<ChannelLock>();
    blockFormat.unset<ObjectDivergence>();
    blockFormat.unset<JumpPosition>();
    blockFormat.unset<ScreenRef>();
    blockFormat.unset<Importance>();

    REQUIRE(blockFormat.isDefault<AudioBlockFormatId>() == false);
    REQUIRE(blockFormat.isDefault<Rtime>() == true);
    REQUIRE(blockFormat.isDefault<Duration>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == true);
    REQUIRE(blockFormat.isDefault<Width>() == true);
    REQUIRE(blockFormat.isDefault<Height>() == true);
    REQUIRE(blockFormat.isDefault<Depth>() == true);
    REQUIRE(blockFormat.isDefault<Gain>() == true);
    REQUIRE(blockFormat.isDefault<Diffuse>() == true);
    REQUIRE(blockFormat.isDefault<ChannelLock>() == true);
    REQUIRE(blockFormat.isDefault<ObjectDivergence>() == true);
    REQUIRE(blockFormat.isDefault<JumpPosition>() == true);
    REQUIRE(blockFormat.isDefault<ScreenRef>() == true);
    REQUIRE(blockFormat.isDefault<Importance>() == true);
  }
}
