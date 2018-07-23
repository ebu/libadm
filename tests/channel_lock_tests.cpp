#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/channel_lock.hpp"

#define BOOST_TEST_MODULE ChannelLock
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(channel_lock, *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    ChannelLock channelLock;
    BOOST_TEST(channelLock.has<ChannelLockFlag>() == true);
    BOOST_TEST(channelLock.has<MaxDistance>() == false);

    BOOST_TEST(channelLock.get<ChannelLockFlag>() == false);

    channelLock.set(ChannelLockFlag(true));
    channelLock.set(MaxDistance(1.5f));

    BOOST_TEST(channelLock.get<ChannelLockFlag>() == true);
    BOOST_TEST(channelLock.get<MaxDistance>() == 1.5f);

    BOOST_TEST(channelLock.isDefault<ChannelLockFlag>() == false);

    channelLock.unset<ChannelLockFlag>();
    channelLock.unset<MaxDistance>();

    BOOST_TEST(channelLock.isDefault<ChannelLockFlag>() == true);

    BOOST_TEST(isEnabled(channelLock) == false);
    enable(channelLock);
    BOOST_TEST(isEnabled(channelLock) == true);
    disable(channelLock);
    BOOST_TEST(isEnabled(channelLock) == false);
  }
}
