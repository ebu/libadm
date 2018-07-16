#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/serialized/frame_header.hpp"
#include "adm/serialized/transport_track_format.hpp"
#include "adm/serialized/transport_track_format_id.hpp"

#define BOOST_TEST_MODULE TransportTrackFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(transport_track_format) {
  using namespace adm;
  // Attributes / Elements
  {
    TransportTrackFormat transportTrackFormat;

    BOOST_TEST(transportTrackFormat.has<TransportId>() == true);
    BOOST_TEST(transportTrackFormat.has<TransportName>() == false);
    BOOST_TEST(transportTrackFormat.has<NumTracks>() == true);
    BOOST_TEST(transportTrackFormat.has<NumIds>() == true);

    transportTrackFormat.set(TransportName("MyTransportTrackFormat"));
    BOOST_TEST(transportTrackFormat.has<TransportName>() == true);

    BOOST_TEST(
        transportTrackFormat.get<TransportId>().get<TransportIdValue>() == 0u);
    BOOST_TEST(transportTrackFormat.get<TransportName>() ==
               "MyTransportTrackFormat");
    BOOST_TEST(transportTrackFormat.get<NumTracks>().get() == 0u);
    BOOST_TEST(transportTrackFormat.get<NumIds>().get() == 0u);

    AudioTrack audioTrack_0(TrackId{0});
    audioTrack_0.add(parseAudioTrackUidId("ATU_00000001"));
    audioTrack_0.add(parseAudioTrackUidId("ATU_00000002"));
    AudioTrack audioTrack_1(TrackId{1});
    audioTrack_1.add(parseAudioTrackUidId("ATU_00000003"));
    AudioTrack audioTrack_2(TrackId{2});
    audioTrack_2.add(parseAudioTrackUidId("ATU_00000004"));
    transportTrackFormat.add(audioTrack_0);
    transportTrackFormat.add(audioTrack_1);
    transportTrackFormat.add(audioTrack_2);

    BOOST_TEST(transportTrackFormat.get<NumTracks>().get() == 3u);
    BOOST_TEST(transportTrackFormat.get<NumIds>().get() == 4u);

    transportTrackFormat.unset<TransportName>();
    transportTrackFormat.clearAudioTracks();

    BOOST_TEST(transportTrackFormat.has<TransportName>() == false);
    BOOST_TEST(transportTrackFormat.get<NumTracks>().get() == 0u);
    BOOST_TEST(transportTrackFormat.get<NumIds>().get() == 0u);
  }
}
