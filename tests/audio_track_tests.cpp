#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_track_uid.hpp"
#include "adm/serialized/audio_track.hpp"

#define BOOST_TEST_MODULE AudioTrack
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_track) {
  using namespace adm;
  // Attributes / Elements
  {
    AudioTrack audioTrack(TrackId(1));

    BOOST_TEST(audioTrack.get<TrackId>() == 1u);
    BOOST_TEST(audioTrack.has<FormatDescriptor>() == false);

    audioTrack.set(FormatDescriptor(FormatDefinition::PCM));
    BOOST_TEST(audioTrack.has<FormatDescriptor>() == true);
    BOOST_TEST(audioTrack.get<FormatDescriptor>() == FormatDefinition::PCM);

    audioTrack.unset<FormatDescriptor>();

    BOOST_TEST(audioTrack.has<FormatDescriptor>() == false);

    // add AudioTrackUidIds
    audioTrack.add(parseAudioTrackUidId("ATU_00000001"));
    audioTrack.add(parseAudioTrackUidId("ATU_00000002"));

    BOOST_TEST(audioTrack.audioTrackUidIds().size() == 2);
  }
}
