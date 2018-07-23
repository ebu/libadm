#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/utilities/comparator.hpp"

#define BOOST_TEST_MODULE AudioChannelFormat
#include <boost/test/included/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(audio_channel_format) {
  using namespace adm;
  // Basic tests
  {
    auto audioChannelFormat =
        AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                   TypeDefinition::DIRECT_SPEAKERS);

    audioChannelFormat->set(AudioChannelFormatId(
        TypeDefinition::DIRECT_SPEAKERS, AudioChannelFormatIdValue(1)));
    audioChannelFormat->set(AudioChannelFormatName("MyNewChannelFormat"));
    audioChannelFormat->set(Frequency(LowPass(120.f)));

    BOOST_TEST(audioChannelFormat->has<AudioChannelFormatId>());
    BOOST_TEST(audioChannelFormat->has<AudioChannelFormatName>());
    BOOST_TEST(audioChannelFormat->has<TypeDescriptor>());
    BOOST_TEST(audioChannelFormat->has<Frequency>());

    BOOST_TEST(
        audioChannelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioChannelFormat->get<AudioChannelFormatId>()
                   .get<AudioChannelFormatIdValue>() == 1u);
    BOOST_TEST(audioChannelFormat->get<AudioChannelFormatName>() ==
               "MyNewChannelFormat");
    BOOST_TEST(audioChannelFormat->get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioChannelFormat->get<Frequency>().get<LowPass>() == 120.f);

    audioChannelFormat->unset<Frequency>();

    BOOST_TEST(audioChannelFormat->has<Frequency>() == false);
  }
  // Sorting tests
  {
    auto audioChannelFormat =
        AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                   TypeDefinition::DIRECT_SPEAKERS);
    audioChannelFormat->add(
        AudioBlockFormatDirectSpeakers(Rtime(std::chrono::seconds(2))));
    audioChannelFormat->add(
        AudioBlockFormatDirectSpeakers(Rtime(std::chrono::seconds(1))));
    audioChannelFormat->add(
        AudioBlockFormatDirectSpeakers(Rtime(std::chrono::seconds(3))));

    // no guarentee about the order of audioBlockFormats is made, but we
    // can sort them if we want to.
    std::sort(
        audioChannelFormat->getElements<AudioBlockFormatDirectSpeakers>()
            .begin(),
        audioChannelFormat->getElements<AudioBlockFormatDirectSpeakers>().end(),
        CompareRtimeLess());

    auto blockFormats =
        audioChannelFormat->getElements<AudioBlockFormatDirectSpeakers>();
    BOOST_TEST(blockFormats.size() == 3);
    Rtime previousRtime(std::chrono::seconds(0));
    for (auto &blockFormat : blockFormats) {
      BOOST_CHECK(previousRtime < blockFormat.get<Rtime>());
      previousRtime = blockFormat.get<Rtime>();
    }
  }

  // Get timespan tests
  // removed functionality, disabled for now but left as reference,
  // should be tested again using iterators and potential (free) helper function
  /*
  {
    auto audioChannelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("MyChannelFormat"), TypeDefinition::OBJECTS);
    for (int i = 0; i < 100; ++i) {
      audioChannelFormat->add(AudioBlockFormatObjects::create(
          SphericalPosition(), Rtime(std::chrono::seconds(i))));
    }
    auto blockFormats =
        audioChannelFormat->getElementsInTimespan<AudioBlockFormatObjects>(
            std::chrono::seconds(10), std::chrono::seconds(15));
    BOOST_CHECK(blockFormats[0]->get<Rtime>() == std::chrono::seconds(10));
    BOOST_CHECK(blockFormats[1]->get<Rtime>() == std::chrono::seconds(11));
    BOOST_CHECK(blockFormats[2]->get<Rtime>() == std::chrono::seconds(12));
    BOOST_CHECK(blockFormats[3]->get<Rtime>() == std::chrono::seconds(13));
    BOOST_CHECK(blockFormats[4]->get<Rtime>() == std::chrono::seconds(14));
    BOOST_CHECK(blockFormats[4]->get<Rtime>() == std::chrono::seconds(14));
    BOOST_CHECK(blockFormats[5]->get<Rtime>() == std::chrono::seconds(15));
  }
  */
}
