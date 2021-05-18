#include <catch2/catch.hpp>
#include <limits>
#include <adm/elements/gain.hpp>

using namespace adm;

TEST_CASE("Gain linear") {
  Gain g = Gain::fromLinear(10.0);
  CHECK(g.isLinear());
  CHECK(!g.isDb());
  CHECK(g.asLinear() == 10.0);
  CHECK(g.asDb() == Approx(20.0));
}

TEST_CASE("Gain linear 0") {
  Gain g = Gain::fromLinear(0.0);
  CHECK(g.asDb() == -std::numeric_limits<double>::infinity());
}

TEST_CASE("Gain Db") {
  Gain g = Gain::fromDb(20.0);
  CHECK(g.isDb());
  CHECK(!g.isLinear());
  CHECK(g.asDb() == 20.0);
  CHECK(g.asLinear() == Approx(10.0));
}

TEST_CASE("Gain Db -inf") {
  Gain g = Gain::fromDb(-std::numeric_limits<double>::infinity());
  CHECK(g.asLinear() == 0.0);
}

TEST_CASE("Gain NamedType compatibility") {
  Gain g(1.5);
  CHECK(g.isLinear());
  CHECK(g.get() == 1.5);
  CHECK(*g == 1.5);
}
