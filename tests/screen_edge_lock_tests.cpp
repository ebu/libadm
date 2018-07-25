#include <catch2/catch.hpp>
#include "adm/elements/screen_edge_lock.hpp"

TEST_CASE("screen_edge_lock") {
  using namespace adm;
  {
    ScreenEdge leftEdge("left");
    ScreenEdge rightEdge("right");
    ScreenEdge topEdge("top");
    ScreenEdge bottomEdge("bottom");
    REQUIRE_THROWS_AS(ScreenEdge("center"), InvalidStringError);
  }
  {
    HorizontalEdge leftEdge("left");
    HorizontalEdge rightEdge("right");
    REQUIRE_THROWS_AS(ScreenEdge("center"), InvalidStringError);
  }
  {
    VerticalEdge topEdge("top");
    VerticalEdge bottomEdge("bottom");
    REQUIRE_THROWS_AS(ScreenEdge("center"), InvalidStringError);
  }
  {
    ScreenEdgeLock screenEdgeLock;
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == false);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == false);
  }
  {
    ScreenEdgeLock screenEdgeLock(HorizontalEdge("right"));
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == true);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == false);
    REQUIRE(screenEdgeLock.get<HorizontalEdge>() == "right");
  }
  {
    ScreenEdgeLock screenEdgeLock(HorizontalEdge("right"), VerticalEdge("top"));
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == true);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == true);
    REQUIRE(screenEdgeLock.get<HorizontalEdge>() == "right");
    REQUIRE(screenEdgeLock.get<VerticalEdge>() == "top");
  }
  {
    ScreenEdgeLock screenEdgeLock(VerticalEdge("top"), HorizontalEdge("right"));
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == true);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == true);
    REQUIRE(screenEdgeLock.get<HorizontalEdge>() == "right");
    REQUIRE(screenEdgeLock.get<VerticalEdge>() == "top");
  }
  {
    ScreenEdgeLock screenEdgeLock(ScreenEdge("right"));
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == true);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == false);
    REQUIRE(screenEdgeLock.get<HorizontalEdge>() == "right");
  }
  {
    ScreenEdgeLock screenEdgeLock(ScreenEdge("top"));
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == false);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == true);
    REQUIRE(screenEdgeLock.get<VerticalEdge>() == "top");
  }
  {
    ScreenEdgeLock screenEdgeLock(ScreenEdge("top"), ScreenEdge("right"));
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == true);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == true);
    REQUIRE(screenEdgeLock.get<HorizontalEdge>() == "right");
    REQUIRE(screenEdgeLock.get<VerticalEdge>() == "top");
    screenEdgeLock.unset<HorizontalEdge>();
    screenEdgeLock.unset<VerticalEdge>();
    REQUIRE(screenEdgeLock.has<HorizontalEdge>() == false);
    REQUIRE(screenEdgeLock.has<VerticalEdge>() == false);
  }
}
