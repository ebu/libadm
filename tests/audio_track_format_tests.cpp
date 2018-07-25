#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_format.hpp"

TEST_CASE("audio_track_format") {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

    audioTrackFormat->set(AudioTrackFormatId(TypeDefinition::OBJECTS,
                                             AudioTrackFormatIdValue(1),
                                             AudioTrackFormatIdCounter(1)));
    audioTrackFormat->set(AudioTrackFormatName("MyNewTrackFormat"));

    REQUIRE(audioTrackFormat->has<AudioTrackFormatId>());
    REQUIRE(audioTrackFormat->has<AudioTrackFormatName>());
    REQUIRE(audioTrackFormat->has<FormatDescriptor>());
    REQUIRE(audioTrackFormat->get<AudioTrackFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::OBJECTS);
    REQUIRE(audioTrackFormat->get<AudioTrackFormatId>()
                .get<AudioTrackFormatIdValue>() == 1u);
    REQUIRE(audioTrackFormat->get<AudioTrackFormatId>()
                .get<AudioTrackFormatIdCounter>() == 1u);
    REQUIRE(audioTrackFormat->get<AudioTrackFormatName>() ==
            "MyNewTrackFormat");
    REQUIRE(audioTrackFormat->get<FormatDescriptor>() == FormatDefinition::PCM);
  }

  // References
  {
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
    auto audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);

    // set references
    audioTrackFormat->setReference(audioStreamFormat);
    REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() ==
            audioStreamFormat);
    // remove references
    audioTrackFormat->removeReference<AudioStreamFormat>();
    REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() == nullptr);
  }
}

TEST_CASE("auto_back_reference") {
  using namespace adm;
  auto audioTrackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
  auto audioTrackFormat2 = AudioTrackFormat::create(
      AudioTrackFormatName("SecondTrackFormat"), FormatDefinition::PCM);
  auto audioStreamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);

  // set references on AudioTrackFormat
  audioTrackFormat->setReference(audioStreamFormat);
  REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() ==
          audioStreamFormat);
  auto trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  REQUIRE(trackRefs.size() == 1);
  REQUIRE(trackRefs[0].lock() == audioTrackFormat);

  // remove reference from AudioTrackFormat
  audioTrackFormat->removeReference<AudioStreamFormat>();
  REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() ==
          std::shared_ptr<AudioStreamFormat>(nullptr));
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  REQUIRE(trackRefs.empty());

  // add references to AudioStreamFormat
  audioStreamFormat->addReference(
      std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
  audioStreamFormat->addReference(
      std::weak_ptr<AudioTrackFormat>(audioTrackFormat2));
  REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() ==
          audioStreamFormat);
  REQUIRE(audioTrackFormat2->getReference<AudioStreamFormat>() ==
          audioStreamFormat);
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  REQUIRE(trackRefs.size() == 2);
  REQUIRE(trackRefs[0].lock() == audioTrackFormat);
  REQUIRE(trackRefs[1].lock() == audioTrackFormat2);

  // remove reference from AudioStreamFormat
  audioStreamFormat->removeReference(audioTrackFormat);
  REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() ==
          std::shared_ptr<AudioStreamFormat>(nullptr));
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  REQUIRE(trackRefs.size() == 1);
  REQUIRE(trackRefs[0].lock() == audioTrackFormat2);

  audioStreamFormat->removeReference(audioTrackFormat2);
  REQUIRE(audioTrackFormat->getReference<AudioStreamFormat>() ==
          std::shared_ptr<AudioStreamFormat>(nullptr));
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  REQUIRE(trackRefs.empty());
}
