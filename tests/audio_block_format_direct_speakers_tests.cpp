#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_block_format_direct_speakers.hpp"

#define BOOST_TEST_MODULE AudioBlockFormatDirectSpeakers
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_block_format_direct_speakers,
                     *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  {
    AudioBlockFormatDirectSpeakers blockFormat;
    auto speakerPosition = SpeakerPosition(Azimuth(30), Elevation(10));

    BOOST_TEST(blockFormat.has<AudioBlockFormatId>() == true);
    BOOST_TEST(blockFormat.has<Rtime>() == true);
    BOOST_TEST(blockFormat.has<Duration>() == false);
    BOOST_TEST(blockFormat.has<Lstart>() == false);
    BOOST_TEST(blockFormat.has<Lduration>() == false);
    BOOST_TEST(blockFormat.has<InitializeBlock>() == false);
    BOOST_TEST(blockFormat.has<SpeakerLabels>() == false);
    BOOST_TEST(blockFormat.has<SpeakerPosition>() == false);

    BOOST_TEST(blockFormat.isDefault<Rtime>() == true);
    BOOST_CHECK(blockFormat.get<Rtime>() == std::chrono::seconds(0));

    blockFormat.set(Rtime(std::chrono::seconds(1)));
    blockFormat.set(Duration(std::chrono::seconds(10)));
    blockFormat.set(Lstart(std::chrono::seconds(0)));
    blockFormat.set(Lduration(std::chrono::seconds(10)));
    blockFormat.set(InitializeBlock(true));
    blockFormat.set(speakerPosition);
    blockFormat.add(SpeakerLabel("M+30"));

    BOOST_TEST(blockFormat.has<AudioBlockFormatId>() == true);
    BOOST_TEST(blockFormat.has<Rtime>() == true);
    BOOST_TEST(blockFormat.has<Duration>() == true);
    BOOST_TEST(blockFormat.has<Lstart>() == true);
    BOOST_TEST(blockFormat.has<Lduration>() == true);
    BOOST_TEST(blockFormat.has<InitializeBlock>() == true);
    BOOST_TEST(blockFormat.has<SpeakerLabels>() == true);
    BOOST_TEST(blockFormat.has<SpeakerPosition>() == true);

    BOOST_TEST(blockFormat.isDefault<Rtime>() == false);
    BOOST_TEST(blockFormat.isDefault<Duration>() == false);

    BOOST_CHECK(blockFormat.get<Rtime>() == std::chrono::seconds(1));
    BOOST_CHECK(blockFormat.get<Duration>() == std::chrono::seconds(10));
    BOOST_CHECK(blockFormat.get<Lstart>() == std::chrono::seconds(0));
    BOOST_CHECK(blockFormat.get<Lduration>() == std::chrono::seconds(10));
    BOOST_CHECK(blockFormat.get<InitializeBlock>() == true);
    BOOST_TEST(*blockFormat.get<SpeakerLabels>().begin() == "M+30");
    BOOST_TEST(blockFormat.get<SpeakerPosition>().get<Azimuth>() == 30);
    BOOST_TEST(blockFormat.get<SpeakerPosition>().get<Elevation>() == 10);

    blockFormat.unset<Rtime>();
    blockFormat.unset<Duration>();
    blockFormat.unset<Lstart>();
    blockFormat.unset<Lduration>();
    blockFormat.unset<InitializeBlock>();
    blockFormat.unset<SpeakerLabels>();
    blockFormat.unset<SpeakerPosition>();

    BOOST_TEST(blockFormat.has<Rtime>() == true);
    BOOST_TEST(blockFormat.has<Duration>() == false);
    BOOST_TEST(blockFormat.has<Lstart>() == false);
    BOOST_TEST(blockFormat.has<Lduration>() == false);
    BOOST_TEST(blockFormat.has<InitializeBlock>() == false);
    BOOST_TEST(blockFormat.has<SpeakerLabels>() == false);
    BOOST_TEST(blockFormat.has<SpeakerPosition>() == false);

    BOOST_TEST(blockFormat.isDefault<Rtime>() == true);

    BOOST_CHECK(blockFormat.get<Rtime>() == std::chrono::seconds(0));
  }
}
