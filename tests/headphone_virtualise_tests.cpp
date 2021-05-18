#include <catch2/catch.hpp>
#include "adm/elements/headphone_virtualise.hpp"

TEST_CASE("headphone_virtualise") {
  using namespace adm;
  {
    auto hpv = HeadphoneVirtualise(Bypass(true), DirectToReverberantRatio(60));

    REQUIRE(hpv.has<Bypass>() == true);
    REQUIRE(hpv.has<DirectToReverberantRatio>() == true);
    REQUIRE(hpv.isDefault<Bypass>() == false);
    REQUIRE(hpv.isDefault<DirectToReverberantRatio>() == false);
    REQUIRE(hpv.get<Bypass>() == true);
    REQUIRE(hpv.get<DirectToReverberantRatio>() == 60);

    hpv.unset<Bypass>();
    hpv.unset<DirectToReverberantRatio>();

    REQUIRE(hpv.isDefault<Bypass>() == true);
    REQUIRE(hpv.isDefault<DirectToReverberantRatio>() == true);
    REQUIRE(hpv.get<Bypass>() == false);
    REQUIRE(hpv.get<DirectToReverberantRatio>() == 130);

    hpv.set(Bypass(false));
    hpv.set(DirectToReverberantRatio(130));

    REQUIRE(hpv.isDefault<Bypass>() == false);
    REQUIRE(hpv.isDefault<DirectToReverberantRatio>() == false);
    REQUIRE(hpv.get<Bypass>() == false);
    REQUIRE(hpv.get<DirectToReverberantRatio>() == 130);
  }
}