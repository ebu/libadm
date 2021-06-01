#include <catch2/catch.hpp>
#include "adm/elements/headphone_virtualise.hpp"
#include "helper/parameter_checks.hpp"

TEST_CASE("headphone_virtualise") {
  using namespace adm;
  {
    SECTION("Default initialisation") {
      using namespace adm_test;
      check_defaulted_param<Bypass>(HeadphoneVirtualise{}, hasDefaultOf(false),
                                    canBeSetTo(true));
      check_defaulted_param<DirectToReverberantRatio>(
          HeadphoneVirtualise{}, hasDefaultOf(130.0f), canBeSetTo(20.0f));
    }

    SECTION("Explicit initialisation") {
      auto hpv =
          HeadphoneVirtualise(Bypass(true), DirectToReverberantRatio(60));
      REQUIRE(hpv.has<Bypass>() == true);
      REQUIRE(hpv.has<DirectToReverberantRatio>() == true);
      REQUIRE(hpv.isDefault<Bypass>() == false);
      REQUIRE(hpv.isDefault<DirectToReverberantRatio>() == false);
      REQUIRE(hpv.get<Bypass>() == true);
      REQUIRE(hpv.get<DirectToReverberantRatio>() == 60);
    }
  }
}