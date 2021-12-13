#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_direct_speakers.hpp"

TEST_CASE("DirectSpeakers block format common subelements") {
  using namespace adm;
  AudioBlockFormatDirectSpeakers blockFormat;

  REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
  REQUIRE(blockFormat.has<Rtime>() == true);
  REQUIRE(blockFormat.has<Duration>() == false);
  REQUIRE(blockFormat.has<SpeakerLabels>() == false);

  REQUIRE(blockFormat.isDefault<Rtime>() == true);

  auto defaultRtime = std::chrono::seconds{0};
  REQUIRE(blockFormat.get<Rtime>().get() == defaultRtime);

  auto rTime = std::chrono::seconds{1};
  auto duration = std::chrono::seconds{10};
  auto label = std::string("M+30");

  blockFormat.set(Rtime(rTime));
  blockFormat.set(Duration(duration));
  blockFormat.add(SpeakerLabel(label));

  REQUIRE(blockFormat.has<AudioBlockFormatId>() == true);
  REQUIRE(blockFormat.has<Rtime>() == true);
  REQUIRE(blockFormat.has<Duration>() == true);
  REQUIRE(blockFormat.has<SpeakerLabels>() == true);

  REQUIRE(blockFormat.isDefault<Rtime>() == false);
  REQUIRE(blockFormat.isDefault<Duration>() == false);

  REQUIRE(blockFormat.get<Rtime>().get() == rTime);
  REQUIRE(blockFormat.get<Duration>().get() == duration);
  REQUIRE(*blockFormat.get<SpeakerLabels>().begin() == label);

  blockFormat.unset<Rtime>();
  blockFormat.unset<Duration>();
  blockFormat.unset<SpeakerLabels>();

  REQUIRE(blockFormat.has<Rtime>() == true);
  REQUIRE(blockFormat.has<Duration>() == false);
  REQUIRE(blockFormat.has<SpeakerLabels>() == false);

  REQUIRE(blockFormat.isDefault<Rtime>() == true);

  REQUIRE(blockFormat.get<Rtime>().get() == defaultRtime);
}

TEST_CASE("DirectSpeakers block format with Spherical coordinates") {
  using namespace adm;
  {
    AudioBlockFormatDirectSpeakers blockFormat;
    auto defaultPosition = SphericalSpeakerPosition{};

    REQUIRE(blockFormat.has<SphericalSpeakerPosition>() == true);
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Azimuth>() ==
            defaultPosition.get<Azimuth>());
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Elevation>() ==
            defaultPosition.get<Elevation>());

    auto speakerPosition =
        SphericalSpeakerPosition(Azimuth(30), Elevation(10), Distance(0.5));
    blockFormat.set(speakerPosition);
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Azimuth>() ==
            Approx(30));
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Elevation>() ==
            Approx(10));
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Distance>() ==
            Approx(0.5));
  }
}

TEST_CASE("DirectSpeakers block format with Cartesian coordinates") {
  using namespace adm;
  auto speakerPosition = CartesianSpeakerPosition{X{-1}, Y{1}, Z{0}};
  auto blockFormat = AudioBlockFormatDirectSpeakers{};

  REQUIRE(!blockFormat.has<CartesianSpeakerPosition>());
  blockFormat.set(speakerPosition);
  REQUIRE(blockFormat.has<CartesianSpeakerPosition>());
  auto retrievedPosition = blockFormat.get<CartesianSpeakerPosition>();
  REQUIRE(retrievedPosition.get<X>() == speakerPosition.get<X>());
  REQUIRE(retrievedPosition.get<Y>() == speakerPosition.get<Y>());
  REQUIRE(retrievedPosition.get<Z>() == speakerPosition.get<Z>());
}
