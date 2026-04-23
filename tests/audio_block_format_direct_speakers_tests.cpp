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
  REQUIRE(blockFormat.has<Cartesian>() == true);

  REQUIRE(blockFormat.isDefault<Rtime>() == true);
  REQUIRE(blockFormat.isDefault<Cartesian>() == true);

  auto defaultRtime = std::chrono::seconds{0};
  REQUIRE(blockFormat.get<Rtime>().get() == defaultRtime);
  REQUIRE(blockFormat.get<Cartesian>() == false);

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
    REQUIRE(blockFormat.get<Cartesian>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == true);

    auto speakerPosition =
        SphericalSpeakerPosition(Azimuth(30), Elevation(10), Distance(0.5));
    blockFormat.set(speakerPosition);
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Azimuth>() ==
            Approx(30));
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Elevation>() ==
            Approx(10));
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Distance>() ==
            Approx(0.5));
    REQUIRE(blockFormat.get<Cartesian>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == true);
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
  REQUIRE(blockFormat.get<Cartesian>() == true);
  REQUIRE(blockFormat.isDefault<Cartesian>() == false);
}

TEST_CASE("DirectSpeakers block format cartesian interactions") {
  using namespace adm;

  SECTION("spherical speaker position does not set cartesian when unset") {
    auto blockFormat = AudioBlockFormatDirectSpeakers{};
    blockFormat.set(SphericalSpeakerPosition{Azimuth{30.0f}, Elevation{5.0f}});

    REQUIRE(blockFormat.has<SphericalSpeakerPosition>() == true);
    REQUIRE(blockFormat.has<CartesianSpeakerPosition>() == false);
    REQUIRE(blockFormat.get<Cartesian>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == true);
  }

  SECTION(
      "unsetting cartesian with cartesian position sets default spherical") {
    auto blockFormat = AudioBlockFormatDirectSpeakers{};
    blockFormat.set(CartesianSpeakerPosition{X{0.8f}, Y{-0.3f}, Z{0.2f}});

    REQUIRE(blockFormat.has<CartesianSpeakerPosition>() == true);
    REQUIRE(blockFormat.get<Cartesian>() == true);

    blockFormat.unset<Cartesian>();

    REQUIRE(blockFormat.has<SphericalSpeakerPosition>() == true);
    REQUIRE(blockFormat.has<CartesianSpeakerPosition>() == false);
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Azimuth>() ==
            Approx(0.0f));
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Elevation>() ==
            Approx(0.0f));
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().has<Distance>() ==
            false);
    REQUIRE(blockFormat.get<Cartesian>() == false);
    REQUIRE(blockFormat.isDefault<Cartesian>() == true);
  }

  SECTION(
      "setting cartesian true with spherical position sets default cartesian") {
    auto blockFormat = AudioBlockFormatDirectSpeakers{};
    blockFormat.set(SphericalSpeakerPosition{Azimuth{10.0f}, Elevation{15.0f},
                                             Distance{1.0f}});

    REQUIRE(blockFormat.has<SphericalSpeakerPosition>() == true);
    REQUIRE(blockFormat.get<SphericalSpeakerPosition>().get<Azimuth>() ==
            Approx(10.0f));

    blockFormat.set(Cartesian{true});

    REQUIRE(blockFormat.has<CartesianSpeakerPosition>() == true);
    REQUIRE(blockFormat.has<SphericalSpeakerPosition>() == false);
    REQUIRE(blockFormat.get<CartesianSpeakerPosition>().get<X>() ==
            Approx(0.0f));
    REQUIRE(blockFormat.get<CartesianSpeakerPosition>().get<Y>() ==
            Approx(0.0f));
    REQUIRE(blockFormat.get<CartesianSpeakerPosition>().has<Z>() == false);
    REQUIRE(blockFormat.get<Cartesian>() == true);
    REQUIRE(blockFormat.isDefault<Cartesian>() == false);
  }
}
