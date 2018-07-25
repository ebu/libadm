#include <catch2/catch.hpp>
#include "adm/elements/channel_lock.hpp"

TEST_CASE("channel_lock") {
  using namespace adm;
  {
    ChannelLock channelLock;
    REQUIRE(channelLock.has<ChannelLockFlag>() == true);
    REQUIRE(channelLock.has<MaxDistance>() == false);

    REQUIRE(channelLock.get<ChannelLockFlag>() == false);

    channelLock.set(ChannelLockFlag(true));
    channelLock.set(MaxDistance(1.5f));

    REQUIRE(channelLock.get<ChannelLockFlag>() == true);
    REQUIRE(channelLock.get<MaxDistance>() == 1.5f);

    REQUIRE(channelLock.isDefault<ChannelLockFlag>() == false);

    channelLock.unset<ChannelLockFlag>();
    channelLock.unset<MaxDistance>();

    REQUIRE(channelLock.isDefault<ChannelLockFlag>() == true);

    REQUIRE(isEnabled(channelLock) == false);
    enable(channelLock);
    REQUIRE(isEnabled(channelLock) == true);
    disable(channelLock);
    REQUIRE(isEnabled(channelLock) == false);
  }
}
