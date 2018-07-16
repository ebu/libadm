#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_block_format_objects.hpp"

#define BOOST_TEST_MODULE AudioBlockFormatObjects
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_block_format_objects,
                     *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    AudioBlockFormatObjects blockFormat{SphericalPosition()};

    BOOST_TEST(blockFormat.has<AudioBlockFormatId>() == true);
    BOOST_TEST(blockFormat.has<Rtime>() == true);
    BOOST_TEST(blockFormat.has<Duration>() == false);
    BOOST_TEST(blockFormat.has<Lstart>() == false);
    BOOST_TEST(blockFormat.has<Lduration>() == false);
    BOOST_TEST(blockFormat.has<InitializeBlock>() == false);
    BOOST_TEST(blockFormat.has<Cartesian>() == true);
    BOOST_TEST(blockFormat.has<Width>() == true);
    BOOST_TEST(blockFormat.has<Height>() == true);
    BOOST_TEST(blockFormat.has<Depth>() == true);
    BOOST_TEST(blockFormat.has<Gain>() == true);
    BOOST_TEST(blockFormat.has<Diffuse>() == true);
    BOOST_TEST(blockFormat.has<ChannelLock>() == true);
    BOOST_TEST(blockFormat.has<ObjectDivergence>() == true);
    BOOST_TEST(blockFormat.has<JumpPosition>() == true);
    BOOST_TEST(blockFormat.has<ScreenRef>() == true);
    BOOST_TEST(blockFormat.has<Importance>() == true);

    BOOST_TEST(blockFormat.isDefault<AudioBlockFormatId>() == false);
    BOOST_TEST(blockFormat.isDefault<Rtime>() == true);
    BOOST_TEST(blockFormat.isDefault<Duration>() == false);
    BOOST_TEST(blockFormat.isDefault<Lstart>() == false);
    BOOST_TEST(blockFormat.isDefault<Lduration>() == false);
    BOOST_TEST(blockFormat.isDefault<InitializeBlock>() == false);
    BOOST_TEST(blockFormat.isDefault<Cartesian>() == true);
    BOOST_TEST(blockFormat.isDefault<Width>() == true);
    BOOST_TEST(blockFormat.isDefault<Height>() == true);
    BOOST_TEST(blockFormat.isDefault<Depth>() == true);
    BOOST_TEST(blockFormat.isDefault<Gain>() == true);
    BOOST_TEST(blockFormat.isDefault<Diffuse>() == true);
    BOOST_TEST(blockFormat.isDefault<ChannelLock>() == true);
    BOOST_TEST(blockFormat.isDefault<ObjectDivergence>() == true);
    BOOST_TEST(blockFormat.isDefault<JumpPosition>() == true);
    BOOST_TEST(blockFormat.isDefault<ScreenRef>() == true);
    BOOST_TEST(blockFormat.isDefault<Importance>() == true);

    BOOST_CHECK(blockFormat.get<Rtime>() == std::chrono::seconds(0));
    BOOST_TEST(blockFormat.get<Width>() == 0.f);
    BOOST_TEST(blockFormat.get<Height>() == 0.f);
    BOOST_TEST(blockFormat.get<Depth>() == 0.f);
    BOOST_TEST(blockFormat.get<Gain>() == 1.f);
    BOOST_TEST(blockFormat.get<Diffuse>() == 0.f);
    BOOST_TEST(blockFormat.get<ChannelLock>().get<ChannelLockFlag>() == false);
    BOOST_TEST(blockFormat.get<ObjectDivergence>().get<Divergence>() == 0.f);
    BOOST_TEST(blockFormat.get<ObjectDivergence>().get<AzimuthRange>() == 45.f);
    BOOST_TEST(blockFormat.get<ObjectDivergence>().get<PositionRange>() == 0.f);
    BOOST_TEST(blockFormat.get<JumpPosition>().get<JumpPositionFlag>() ==
               false);
    BOOST_CHECK(
        blockFormat.get<JumpPosition>().get<InterpolationLength>().get() ==
        std::chrono::seconds(0));

    blockFormat.set(Rtime(std::chrono::seconds(2)));
    blockFormat.set(Duration(std::chrono::seconds(10)));
    blockFormat.set(Lstart(std::chrono::seconds(0)));
    blockFormat.set(Lduration(std::chrono::seconds(10)));
    blockFormat.set(InitializeBlock(true));
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

    BOOST_CHECK(blockFormat.get<Rtime>() == std::chrono::seconds(2));
    BOOST_CHECK(blockFormat.get<Duration>() == std::chrono::seconds(10));
    BOOST_CHECK(blockFormat.get<Lstart>() == std::chrono::seconds(0));
    BOOST_CHECK(blockFormat.get<Lduration>() == std::chrono::seconds(10));
    BOOST_CHECK(blockFormat.get<InitializeBlock>() == true);
    BOOST_TEST(blockFormat.get<Cartesian>() == false);
    BOOST_TEST(blockFormat.get<Width>() == 45.f);
    BOOST_TEST(blockFormat.get<Height>() == 20.f);
    BOOST_TEST(blockFormat.get<Depth>() == 0.2f);
    BOOST_TEST(blockFormat.get<Gain>() == 2.5f);
    BOOST_TEST(blockFormat.get<Diffuse>() == 0.5f);
    BOOST_TEST(blockFormat.get<ChannelLock>().get<ChannelLockFlag>() == true);
    BOOST_TEST(blockFormat.get<ObjectDivergence>().get<Divergence>().get() ==
               0.2);
    BOOST_TEST(blockFormat.get<JumpPosition>().get<JumpPositionFlag>() == true);
    BOOST_CHECK(blockFormat.get<JumpPosition>().get<InterpolationLength>() ==
                std::chrono::milliseconds(20));
    BOOST_TEST(blockFormat.get<ScreenRef>() == true);
    BOOST_TEST(blockFormat.get<Importance>() == 10);

    BOOST_TEST(blockFormat.isDefault<AudioBlockFormatId>() == false);
    BOOST_TEST(blockFormat.isDefault<Rtime>() == false);
    BOOST_TEST(blockFormat.isDefault<Duration>() == false);
    BOOST_TEST(blockFormat.isDefault<Lstart>() == false);
    BOOST_TEST(blockFormat.isDefault<Lduration>() == false);
    BOOST_TEST(blockFormat.isDefault<InitializeBlock>() == false);
    BOOST_TEST(blockFormat.isDefault<Cartesian>() == false);
    BOOST_TEST(blockFormat.isDefault<Width>() == false);
    BOOST_TEST(blockFormat.isDefault<Height>() == false);
    BOOST_TEST(blockFormat.isDefault<Depth>() == false);
    BOOST_TEST(blockFormat.isDefault<Gain>() == false);
    BOOST_TEST(blockFormat.isDefault<Diffuse>() == false);
    BOOST_TEST(blockFormat.isDefault<ChannelLock>() == false);
    BOOST_TEST(blockFormat.isDefault<ObjectDivergence>() == false);
    BOOST_TEST(blockFormat.isDefault<JumpPosition>() == false);
    BOOST_TEST(blockFormat.isDefault<ScreenRef>() == false);
    BOOST_TEST(blockFormat.isDefault<Importance>() == false);

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();
    blockFormat.unset<Lstart>();
    blockFormat.unset<Lduration>();
    blockFormat.unset<InitializeBlock>();
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

    BOOST_TEST(blockFormat.isDefault<AudioBlockFormatId>() == false);
    BOOST_TEST(blockFormat.isDefault<Rtime>() == true);
    BOOST_TEST(blockFormat.isDefault<Duration>() == false);
    BOOST_TEST(blockFormat.isDefault<Lstart>() == false);
    BOOST_TEST(blockFormat.isDefault<Lduration>() == false);
    BOOST_TEST(blockFormat.isDefault<InitializeBlock>() == false);
    BOOST_TEST(blockFormat.isDefault<Cartesian>() == true);
    BOOST_TEST(blockFormat.isDefault<Width>() == true);
    BOOST_TEST(blockFormat.isDefault<Height>() == true);
    BOOST_TEST(blockFormat.isDefault<Depth>() == true);
    BOOST_TEST(blockFormat.isDefault<Gain>() == true);
    BOOST_TEST(blockFormat.isDefault<Diffuse>() == true);
    BOOST_TEST(blockFormat.isDefault<ChannelLock>() == true);
    BOOST_TEST(blockFormat.isDefault<ObjectDivergence>() == true);
    BOOST_TEST(blockFormat.isDefault<JumpPosition>() == true);
    BOOST_TEST(blockFormat.isDefault<ScreenRef>() == true);
    BOOST_TEST(blockFormat.isDefault<Importance>() == true);
  }
}
