#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/utilities/comparator.hpp"

TEST_CASE("audio_channel_format") {
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

    REQUIRE(audioChannelFormat->has<AudioChannelFormatId>());
    REQUIRE(audioChannelFormat->has<AudioChannelFormatName>());
    REQUIRE(audioChannelFormat->has<TypeDescriptor>());
    REQUIRE(audioChannelFormat->has<Frequency>());

    REQUIRE(
        audioChannelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioChannelFormat->get<AudioChannelFormatId>()
                .get<AudioChannelFormatIdValue>() == 1u);
    REQUIRE(audioChannelFormat->get<AudioChannelFormatName>() ==
            "MyNewChannelFormat");
    REQUIRE(audioChannelFormat->get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioChannelFormat->get<Frequency>().get<LowPass>().get() ==
            Approx(120.f));

    audioChannelFormat->unset<Frequency>();

    REQUIRE(audioChannelFormat->has<Frequency>() == false);
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
    REQUIRE(blockFormats.size() == 3);
    Rtime previousRtime(std::chrono::seconds(0));
    for (auto &blockFormat : blockFormats) {
      REQUIRE(previousRtime.get() < blockFormat.get<Rtime>().get());
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
