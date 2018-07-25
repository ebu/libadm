#include <catch2/catch.hpp>
#include "adm/elements/frequency.hpp"

TEST_CASE("frequency") {
  using namespace adm;
  {
    Frequency frequency;
    REQUIRE(frequency.has<HighPass>() == false);
    REQUIRE(frequency.has<LowPass>() == false);

    REQUIRE(frequency.isDefault<HighPass>() == false);
    REQUIRE(frequency.isDefault<LowPass>() == false);

    frequency.set(HighPass(300.f));
    frequency.set(LowPass(3300.f));

    REQUIRE(frequency.get<HighPass>() == Approx(300.f));
    REQUIRE(frequency.get<LowPass>() == Approx(3300.f));

    REQUIRE(frequency.isDefault<HighPass>() == false);
    REQUIRE(frequency.isDefault<LowPass>() == false);

    frequency.unset<HighPass>();
    frequency.unset<LowPass>();
  }

  // lowpass
  {
    Frequency frequency(LowPass(120.f));
    REQUIRE(isLowPass(frequency) == true);
    REQUIRE(isHighPass(frequency) == false);
    REQUIRE(isBandPass(frequency) == false);
  }

  // highpass
  {
    Frequency frequency(HighPass(120.f));
    REQUIRE(isLowPass(frequency) == false);
    REQUIRE(isHighPass(frequency) == true);
    REQUIRE(isBandPass(frequency) == false);
  }

  // bandpass
  {
    Frequency frequency(HighPass(120.f), LowPass(3000.f));
    REQUIRE(isLowPass(frequency) == false);
    REQUIRE(isHighPass(frequency) == false);
    REQUIRE(isBandPass(frequency) == true);
  }
}
