#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/screen_edge_lock.hpp"

#define BOOST_TEST_MODULE ScreenEdgeLock
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(screen_edge_lock) {
  using namespace adm;
  {
    ScreenEdge leftEdge("left");
    ScreenEdge rightEdge("right");
    ScreenEdge topEdge("top");
    ScreenEdge bottomEdge("bottom");
    BOOST_CHECK_THROW(ScreenEdge("center"), InvalidStringError);
  }
  {
    HorizontalEdge leftEdge("left");
    HorizontalEdge rightEdge("right");
    BOOST_CHECK_THROW(ScreenEdge("center"), InvalidStringError);
  }
  {
    VerticalEdge topEdge("top");
    VerticalEdge bottomEdge("bottom");
    BOOST_CHECK_THROW(ScreenEdge("center"), InvalidStringError);
  }
  {
    ScreenEdgeLock screenEdgeLock;
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == false);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == false);
  }
  {
    ScreenEdgeLock screenEdgeLock(HorizontalEdge("right"));
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == true);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == false);
    BOOST_TEST(screenEdgeLock.get<HorizontalEdge>() == "right");
  }
  {
    ScreenEdgeLock screenEdgeLock(HorizontalEdge("right"), VerticalEdge("top"));
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == true);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == true);
    BOOST_TEST(screenEdgeLock.get<HorizontalEdge>() == "right");
    BOOST_TEST(screenEdgeLock.get<VerticalEdge>() == "top");
  }
  {
    ScreenEdgeLock screenEdgeLock(VerticalEdge("top"), HorizontalEdge("right"));
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == true);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == true);
    BOOST_TEST(screenEdgeLock.get<HorizontalEdge>() == "right");
    BOOST_TEST(screenEdgeLock.get<VerticalEdge>() == "top");
  }
  {
    ScreenEdgeLock screenEdgeLock(ScreenEdge("right"));
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == true);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == false);
    BOOST_TEST(screenEdgeLock.get<HorizontalEdge>() == "right");
  }
  {
    ScreenEdgeLock screenEdgeLock(ScreenEdge("top"));
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == false);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == true);
    BOOST_TEST(screenEdgeLock.get<VerticalEdge>() == "top");
  }
  {
    ScreenEdgeLock screenEdgeLock(ScreenEdge("top"), ScreenEdge("right"));
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == true);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == true);
    BOOST_TEST(screenEdgeLock.get<HorizontalEdge>() == "right");
    BOOST_TEST(screenEdgeLock.get<VerticalEdge>() == "top");
    screenEdgeLock.unset<HorizontalEdge>();
    screenEdgeLock.unset<VerticalEdge>();
    BOOST_TEST(screenEdgeLock.has<HorizontalEdge>() == false);
    BOOST_TEST(screenEdgeLock.has<VerticalEdge>() == false);
  }
}
