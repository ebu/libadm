#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_programme.hpp"

TEST_CASE("audio_programme") {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioProgramme =
        AudioProgramme::create(AudioProgrammeName("MyProgramme"));
    audioProgramme->set(AudioProgrammeId(AudioProgrammeIdValue(1)));
    audioProgramme->set(AudioProgrammeLanguage("de"));
    audioProgramme->set(Start(std::chrono::seconds(0)));
    audioProgramme->set(End(std::chrono::seconds(10)));
    audioProgramme->add(LoudnessMetadata());
    audioProgramme->set(MaxDuckingDepth(-30));
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // audioProgramme->set(AudioProgrammeReferenceScreen());

    REQUIRE(audioProgramme->has<AudioProgrammeId>());
    REQUIRE(audioProgramme->has<AudioProgrammeName>());
    REQUIRE(audioProgramme->has<AudioProgrammeLanguage>());
    REQUIRE(audioProgramme->has<Start>());
    REQUIRE(audioProgramme->has<End>());
    REQUIRE(audioProgramme->has<LoudnessMetadatas>());
    REQUIRE(audioProgramme->has<MaxDuckingDepth>());
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // REQUIRE(audioProgramme->has<AudioProgrammeReferenceScreen>());

    REQUIRE(
        audioProgramme->get<AudioProgrammeId>().get<AudioProgrammeIdValue>() ==
        1u);
    REQUIRE(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
    REQUIRE(audioProgramme->get<AudioProgrammeLanguage>() == "de");
    REQUIRE(audioProgramme->get<Start>().get() == std::chrono::seconds(0));
    REQUIRE(audioProgramme->get<End>().get() == std::chrono::seconds(10));
    REQUIRE(audioProgramme->get<MaxDuckingDepth>() == -30);
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // REQUIRE(audioProgramme->get<AudioProgrammeReferenceScreen>() == ???);

    audioProgramme->unset<AudioProgrammeLanguage>();
    audioProgramme->unset<Start>();
    audioProgramme->unset<End>();
    audioProgramme->unset<LoudnessMetadatas>();
    audioProgramme->unset<MaxDuckingDepth>();
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // audioProgramme->unset<AudioProgrammeReferenceScreen>();

    REQUIRE(!audioProgramme->has<AudioProgrammeLanguage>());
    REQUIRE(audioProgramme->has<Start>());
    REQUIRE(!audioProgramme->has<End>());
    REQUIRE(!audioProgramme->has<LoudnessMetadatas>());
    REQUIRE(!audioProgramme->has<MaxDuckingDepth>());
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // REQUIRE(!audioProgramme->has<AudioProgrammeReferenceScreen>());
  }
  // References
  {
    auto audioProgramme =
        AudioProgramme::create(AudioProgrammeName("MyProgramme"));

    auto referencedAudioContent =
        AudioContent::create(AudioContentName("MyContent"));

    // add references
    audioProgramme->addReference(referencedAudioContent);
    audioProgramme->addReference(referencedAudioContent);
    REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 1);

    // remove references
    audioProgramme->removeReference(referencedAudioContent);
    REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 0);

    // clear references
    audioProgramme->addReference(referencedAudioContent);
    audioProgramme->clearReferences<AudioContent>();
    REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 0);
  }
}
