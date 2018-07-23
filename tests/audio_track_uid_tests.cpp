#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/audio_track_uid.hpp"

#define BOOST_TEST_MODULE AudioTrackUid
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_track_uid) {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioTrackUid =
        AudioTrackUid::create(AudioTrackUidId(AudioTrackUidIdValue(1)),
                              BitDepth(16), SampleRate(44100));

    BOOST_TEST(
        audioTrackUid->get<AudioTrackUidId>().get<AudioTrackUidIdValue>() ==
        1u);
    BOOST_TEST(audioTrackUid->get<SampleRate>() == 44100u);
    BOOST_TEST(audioTrackUid->get<BitDepth>() == 16u);

    audioTrackUid->set(AudioTrackUidId(AudioTrackUidIdValue(2)));
    audioTrackUid->set(SampleRate(48000));
    audioTrackUid->set(BitDepth(24));

    BOOST_TEST(audioTrackUid->has<AudioTrackUidId>());
    BOOST_TEST(audioTrackUid->has<SampleRate>());
    BOOST_TEST(audioTrackUid->has<BitDepth>());

    BOOST_TEST(
        audioTrackUid->get<AudioTrackUidId>().get<AudioTrackUidIdValue>() ==
        2u);
    BOOST_TEST(audioTrackUid->get<SampleRate>() == 48000u);
    BOOST_TEST(audioTrackUid->get<BitDepth>() == 24u);

    audioTrackUid->unset<SampleRate>();
    audioTrackUid->unset<BitDepth>();

    BOOST_TEST(!audioTrackUid->has<SampleRate>());
    BOOST_TEST(!audioTrackUid->has<BitDepth>());
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
    BOOST_TEST(audioTrackUid->getReference<AudioTrackFormat>() ==
               audioTrackFormat);
    BOOST_TEST(audioTrackUid->getReference<AudioPackFormat>() ==
               audioPackFormat);

    // remove references
    audioTrackUid->removeReference<AudioTrackFormat>();
    audioTrackUid->removeReference<AudioPackFormat>();
    BOOST_CHECK(audioTrackUid->getReference<AudioTrackFormat>() == nullptr);
    BOOST_CHECK(audioTrackUid->getReference<AudioPackFormat>() == nullptr);
  }
}
