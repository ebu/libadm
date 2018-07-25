#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_direct_speakers.hpp"

TEST_CASE("audio_block_format_direct_speakers") {
  using namespace adm;
  {
    AudioBlockFormatDirectSpeakers blockFormat;
    auto speakerPosition = SpeakerPosition(Azimuth(30), Elevation(10));

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);
    REQUIRE(blockFormat.has<SpeakerLabels>() == false);
    REQUIRE(blockFormat.has<SpeakerPosition>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);
    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));

    blockFormat.set(Rtime(std::chrono::seconds(1)));
    blockFormat.set(Duration(std::chrono::seconds(10)));
    blockFormat.set(speakerPosition);
    blockFormat.add(SpeakerLabel("M+30"));

    REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == true);
    REQUIRE(blockFormat.has<SpeakerLabels>() == true);
    REQUIRE(blockFormat.has<SpeakerPosition>() == true);

    REQUIRE(blockFormat.isDefault<Rtime>() == false);
    REQUIRE(blockFormat.isDefault<Duration>() == false);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(1));
    REQUIRE(blockFormat.get<Duration>().get() == std::chrono::seconds(10));
    REQUIRE(*blockFormat.get<SpeakerLabels>().begin() == "M+30");
    REQUIRE(blockFormat.get<SpeakerPosition>().get<Azimuth>() == 30);
    REQUIRE(blockFormat.get<SpeakerPosition>().get<Elevation>() == 10);

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();
    blockFormat.unset<SpeakerLabels>();
    blockFormat.unset<SpeakerPosition>();

    REQUIRE(blockFormat.has<Rtime>() == true);
    REQUIRE(blockFormat.has<Duration>() == false);
    REQUIRE(blockFormat.has<SpeakerLabels>() == false);
    REQUIRE(blockFormat.has<SpeakerPosition>() == false);

    REQUIRE(blockFormat.isDefault<Rtime>() == true);

    REQUIRE(blockFormat.get<Rtime>().get() == std::chrono::seconds(0));
  }
}
