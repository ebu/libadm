#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/audio_track_uid.hpp"

TEST_CASE("audio_track_uid") {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioTrackUid =
        AudioTrackUid::create(AudioTrackUidId(AudioTrackUidIdValue(1)),
                              BitDepth(16), SampleRate(44100));

    REQUIRE(audioTrackUid->get<AudioTrackUidId>().get<AudioTrackUidIdValue>() ==
            1u);
    REQUIRE(audioTrackUid->get<SampleRate>() == 44100u);
    REQUIRE(audioTrackUid->get<BitDepth>() == 16u);

    audioTrackUid->set(AudioTrackUidId(AudioTrackUidIdValue(2)));
    audioTrackUid->set(SampleRate(48000));
    audioTrackUid->set(BitDepth(24));

    REQUIRE(audioTrackUid->has<AudioTrackUidId>());
    REQUIRE(audioTrackUid->has<SampleRate>());
    REQUIRE(audioTrackUid->has<BitDepth>());

    REQUIRE(audioTrackUid->get<AudioTrackUidId>().get<AudioTrackUidIdValue>() ==
            2u);
    REQUIRE(audioTrackUid->get<SampleRate>() == 48000u);
    REQUIRE(audioTrackUid->get<BitDepth>() == 24u);

    audioTrackUid->unset<SampleRate>();
    audioTrackUid->unset<BitDepth>();

    REQUIRE(!audioTrackUid->has<SampleRate>());
    REQUIRE(!audioTrackUid->has<BitDepth>());
  }
  // References
  {
    auto audioTrackUid =
        AudioTrackUid::create(AudioTrackUidId(AudioTrackUidIdValue(1)),
                              BitDepth(16), SampleRate(44100));
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
    auto audioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyPackFormat"), TypeDefinition::DIRECT_SPEAKERS);

    // set references
    audioTrackUid->setReference(audioTrackFormat);
    audioTrackUid->setReference(audioPackFormat);
    REQUIRE(audioTrackUid->getReference<AudioTrackFormat>() ==
            audioTrackFormat);
    REQUIRE(audioTrackUid->getReference<AudioPackFormat>() == audioPackFormat);

    // remove references
    audioTrackUid->removeReference<AudioTrackFormat>();
    audioTrackUid->removeReference<AudioPackFormat>();
    REQUIRE(audioTrackUid->getReference<AudioTrackFormat>() == nullptr);
    REQUIRE(audioTrackUid->getReference<AudioPackFormat>() == nullptr);
  }
}
