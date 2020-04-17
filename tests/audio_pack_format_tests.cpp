#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_pack_format.hpp"

TEST_CASE("audio_pack_format") {
  using namespace adm;
  // Attributes / Elements
  {
    auto packFormat =
        AudioPackFormat::create(AudioPackFormatName("MyPackFormat"),
                                parseTypeDefinition("DirectSpeakers"));
    packFormat->set(AudioPackFormatId(TypeDefinition::DIRECT_SPEAKERS,
                                      AudioPackFormatIdValue(1)));
    packFormat->set(AudioPackFormatName("MyNewPackFormat"));
    packFormat->set(Importance(5));
    packFormat->set(AbsoluteDistance(4.5));

    REQUIRE(packFormat->has<AudioPackFormatId>());
    REQUIRE(packFormat->has<AudioPackFormatName>());
    REQUIRE(packFormat->has<TypeDescriptor>());
    REQUIRE(packFormat->has<Importance>());
    REQUIRE(packFormat->has<AbsoluteDistance>());

    REQUIRE(packFormat->get<AudioPackFormatId>().get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(
        packFormat->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
        1u);
    REQUIRE(packFormat->get<AudioPackFormatName>() == "MyNewPackFormat");
    REQUIRE(packFormat->get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(packFormat->get<Importance>() == 5);
    REQUIRE(packFormat->get<AbsoluteDistance>() == 4.5);

    packFormat->unset<Importance>();
    packFormat->unset<AbsoluteDistance>();

    REQUIRE(!packFormat->has<Importance>());
    REQUIRE(!packFormat->has<AbsoluteDistance>());
  }
  // References
  {
    auto audioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyPackFormat"), TypeDefinition::OBJECTS);

    auto referencedAudioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyReferencedPackFormat"), TypeDefinition::OBJECTS);
    auto referencedAudioChannelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("MyChannelFormat"), TypeDefinition::OBJECTS);

    // add references
    audioPackFormat->addReference(referencedAudioChannelFormat);
    audioPackFormat->addReference(referencedAudioChannelFormat);
    audioPackFormat->addReference(referencedAudioPackFormat);
    audioPackFormat->addReference(referencedAudioPackFormat);

    REQUIRE(audioPackFormat->getReferences<AudioChannelFormat>().size() == 1);
    REQUIRE(audioPackFormat->getReferences<AudioPackFormat>().size() == 1);

    // remove references
    audioPackFormat->removeReference(referencedAudioChannelFormat);
    audioPackFormat->removeReference(referencedAudioPackFormat);

    REQUIRE(audioPackFormat->getReferences<AudioChannelFormat>().size() == 0);
    REQUIRE(audioPackFormat->getReferences<AudioPackFormat>().size() == 0);

    // clear references
    audioPackFormat->addReference(referencedAudioChannelFormat);
    audioPackFormat->addReference(referencedAudioPackFormat);
    audioPackFormat->clearReferences<AudioChannelFormat>();
    audioPackFormat->clearReferences<AudioPackFormat>();
    REQUIRE(audioPackFormat->getReferences<AudioChannelFormat>().size() == 0);
    REQUIRE(audioPackFormat->getReferences<AudioPackFormat>().size() == 0);

    // reference cycles AudioObjects
    auto audioPackFormat1 = AudioPackFormat::create(
        AudioPackFormatName("First"), TypeDefinition::OBJECTS);
    auto audioPackFormat2 = AudioPackFormat::create(
        AudioPackFormatName("Second"), TypeDefinition::OBJECTS);
    auto audioPackFormat3 = AudioPackFormat::create(
        AudioPackFormatName("Third"), TypeDefinition::OBJECTS);

    REQUIRE_THROWS_AS(audioPackFormat1->addReference(audioPackFormat1),
                      std::runtime_error);
    audioPackFormat1->addReference(audioPackFormat2);
    audioPackFormat2->addReference(audioPackFormat3);
    REQUIRE_THROWS_AS(audioPackFormat3->addReference(audioPackFormat1),
                      std::runtime_error);
  }

  // check Hoa fails when constructed directly
  {
    REQUIRE_THROWS_AS(
        AudioPackFormat::create(AudioPackFormatName("MyPackFormat"),
                                parseTypeDefinition("HOA")),
        std::invalid_argument);
  }
}
