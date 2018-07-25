#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_format.hpp"

TEST_CASE("audio_stream_format") {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);

    audioStreamFormat->set(AudioStreamFormatId(TypeDefinition::OBJECTS,
                                               AudioStreamFormatIdValue(1)));
    audioStreamFormat->set(AudioStreamFormatName("MyNewStreamFormat"));

    REQUIRE(audioStreamFormat->has<AudioStreamFormatId>());
    REQUIRE(audioStreamFormat->has<AudioStreamFormatName>());
    REQUIRE(audioStreamFormat->has<FormatDescriptor>());
    REQUIRE(
        audioStreamFormat->get<AudioStreamFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::OBJECTS);

    REQUIRE(audioStreamFormat->get<AudioStreamFormatId>()
                .get<AudioStreamFormatIdValue>() == 1u);
    REQUIRE(audioStreamFormat->get<AudioStreamFormatName>() ==
            "MyNewStreamFormat");
    REQUIRE(audioStreamFormat->get<FormatDescriptor>() ==
            FormatDefinition::PCM);
  }
  // References
  {
    auto audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
    auto audioChannelFormat =
        AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                   TypeDefinition::DIRECT_SPEAKERS);
    auto audioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyPackFormat"), TypeDefinition::DIRECT_SPEAKERS);
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

    // set references
    audioStreamFormat->setReference(audioChannelFormat);
    audioStreamFormat->setReference(audioPackFormat);
    audioStreamFormat->addReference(
        std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
    audioStreamFormat->addReference(
        std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
    REQUIRE(audioStreamFormat->getReference<AudioChannelFormat>() ==
            audioChannelFormat);
    REQUIRE(audioStreamFormat->getReference<AudioPackFormat>() ==
            audioPackFormat);
    REQUIRE(audioStreamFormat->getAudioTrackFormatReferences().size() == 1);

    // remove references
    audioStreamFormat->removeReference<AudioChannelFormat>();
    audioStreamFormat->removeReference<AudioPackFormat>();
    audioStreamFormat->removeReference(audioTrackFormat);
    REQUIRE(audioStreamFormat->getReference<AudioChannelFormat>() == nullptr);
    REQUIRE(audioStreamFormat->getReference<AudioPackFormat>() == nullptr);
    REQUIRE(audioStreamFormat->getAudioTrackFormatReferences().size() == 0);

    // clear references
    audioStreamFormat->addReference(
        std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
    audioStreamFormat->clearReferences<AudioTrackFormat>();
    REQUIRE(audioStreamFormat->getAudioTrackFormatReferences().size() == 0);
  }
}
