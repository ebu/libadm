#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_hoa.hpp"

TEST_CASE("audio_block_format_hoa") {
  using namespace adm;
  {
    AudioBlockFormatHoa blockFormat;

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));

    blockFormat.set(Rtime(std::chrono::seconds(1)));
    blockFormat.set(Duration(std::chrono::seconds(10)));

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == true);

    REQUIRE(blockFormat.isDefault<Rtime>() == false);
    REQUIRE(blockFormat.isDefault<Duration>() == false);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(1));
    REQUIRE(blockFormat.get<Duration>().get() == std::chrono::seconds(10));

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();

    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));
  }
}
