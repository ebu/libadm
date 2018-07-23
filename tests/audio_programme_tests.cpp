#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_programme.hpp"

#define BOOST_TEST_MODULE AudioProgramme
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_programme) {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioProgramme =
        AudioProgramme::create(AudioProgrammeName("MyProgramme"));
    audioProgramme->set(AudioProgrammeId(AudioProgrammeIdValue(1)));
    audioProgramme->set(AudioProgrammeLanguage("de"));
    audioProgramme->set(Start(std::chrono::seconds(0)));
    audioProgramme->set(End(std::chrono::seconds(10)));
    audioProgramme->set(LoudnessMetadata());
    audioProgramme->set(MaxDuckingDepth(-30));
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // audioProgramme->set(AudioProgrammeReferenceScreen());

    BOOST_TEST(audioProgramme->has<AudioProgrammeId>());
    BOOST_TEST(audioProgramme->has<AudioProgrammeName>());
    BOOST_TEST(audioProgramme->has<AudioProgrammeLanguage>());
    BOOST_TEST(audioProgramme->has<Start>());
    BOOST_TEST(audioProgramme->has<End>());
    BOOST_TEST(audioProgramme->has<LoudnessMetadata>());
    BOOST_TEST(audioProgramme->has<MaxDuckingDepth>());
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // BOOST_TEST(audioProgramme->has<AudioProgrammeReferenceScreen>());

    BOOST_TEST(
        audioProgramme->get<AudioProgrammeId>().get<AudioProgrammeIdValue>() ==
        1u);
    BOOST_TEST(audioProgramme->get<AudioProgrammeName>() == "MyProgramme");
    BOOST_TEST(audioProgramme->get<AudioProgrammeLanguage>() == "de");
    BOOST_CHECK(audioProgramme->get<Start>() == std::chrono::seconds(0));
    BOOST_CHECK(audioProgramme->get<End>() == std::chrono::seconds(10));
    BOOST_TEST(audioProgramme->get<LoudnessMetadata>().method == "");
    BOOST_TEST(audioProgramme->get<LoudnessMetadata>().recType == "");
    BOOST_TEST(audioProgramme->get<LoudnessMetadata>().correctionType == "");
    BOOST_TEST(audioProgramme->get<MaxDuckingDepth>() == -30);
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // BOOST_TEST(audioProgramme->get<AudioProgrammeReferenceScreen>() == ???);

    audioProgramme->unset<AudioProgrammeLanguage>();
    audioProgramme->unset<Start>();
    audioProgramme->unset<End>();
    audioProgramme->unset<LoudnessMetadata>();
    audioProgramme->unset<MaxDuckingDepth>();
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // audioProgramme->unset<AudioProgrammeReferenceScreen>();

    BOOST_TEST(!audioProgramme->has<AudioProgrammeLanguage>());
    BOOST_TEST(audioProgramme->has<Start>());
    BOOST_TEST(!audioProgramme->has<End>());
    BOOST_TEST(!audioProgramme->has<LoudnessMetadata>());
    BOOST_TEST(!audioProgramme->has<MaxDuckingDepth>());
    // NOTE: AudioProgrammeReferenceScreen is not yet implemented.
    // BOOST_TEST(!audioProgramme->has<AudioProgrammeReferenceScreen>());
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
    BOOST_TEST(audioProgramme->getReferences<AudioContent>().size() == 1);

    // remove references
    audioProgramme->removeReference(referencedAudioContent);
    BOOST_TEST(audioProgramme->getReferences<AudioContent>().size() == 0);

    // clear references
    audioProgramme->addReference(referencedAudioContent);
    audioProgramme->clearReferences<AudioContent>();
    BOOST_TEST(audioProgramme->getReferences<AudioContent>().size() == 0);
  }
}
