#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_binaural.hpp"

TEST_CASE("audio_block_format_binaural") {
  using namespace adm;
  {
    AudioBlockFormatBinaural blockFormat;

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);
    REQUIRE(blockFormat.has<HeadphoneVirtualise>() == true);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);
    REQUIRE(blockFormat.isDefault<HeadphoneVirtualise>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));
    REQUIRE(blockFormat.get<HeadphoneVirtualise>().get<Bypass>() == false);
    REQUIRE(blockFormat.get<HeadphoneVirtualise>()
                .get<DirectToReverberantRatio>() == 130);

    blockFormat.set(Rtime(std::chrono::seconds(1)));
    blockFormat.set(Duration(std::chrono::seconds(10)));
    blockFormat.set(
        HeadphoneVirtualise(Bypass(true), DirectToReverberantRatio(60)));

    REQUIRE(blockFormat.isDefault<Rtime>() == false);
    REQUIRE(blockFormat.isDefault<Duration>() == false);
    REQUIRE(blockFormat.isDefault<HeadphoneVirtualise>() == false);

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == true);
    REQUIRE(blockFormat.get<HeadphoneVirtualise>().get<Bypass>() == true);
    REQUIRE(blockFormat.get<HeadphoneVirtualise>()
                .get<DirectToReverberantRatio>() == 60);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(1));
    REQUIRE(blockFormat.get<Duration>().get() == std::chrono::seconds(10));

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();
    blockFormat.unset<HeadphoneVirtualise>();

    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);
    REQUIRE(blockFormat.isDefault<HeadphoneVirtualise>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));
  }
}
