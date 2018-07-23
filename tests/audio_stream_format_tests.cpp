#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_format.hpp"

#define BOOST_TEST_MODULE AudioStreamFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_stream_format) {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);

    audioStreamFormat->set(AudioStreamFormatId(TypeDefinition::OBJECTS,
                                               AudioStreamFormatIdValue(1)));
    audioStreamFormat->set(AudioStreamFormatName("MyNewStreamFormat"));

    BOOST_TEST(audioStreamFormat->has<AudioStreamFormatId>());
    BOOST_TEST(audioStreamFormat->has<AudioStreamFormatName>());
    BOOST_TEST(audioStreamFormat->has<FormatDescriptor>());
    BOOST_TEST(
        audioStreamFormat->get<AudioStreamFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::OBJECTS);

    BOOST_TEST(audioStreamFormat->get<AudioStreamFormatId>()
                   .get<AudioStreamFormatIdValue>() == 1u);
    BOOST_TEST(audioStreamFormat->get<AudioStreamFormatName>() ==
               "MyNewStreamFormat");
    BOOST_TEST(audioStreamFormat->get<FormatDescriptor>() ==
               FormatDefinition::PCM);
  }
  // References
  {
    auto audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
    auto audioChannelFormat =
        AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                   TypeDefinition::DIRECT_SPEAKERS);
    auto audioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyPackFormat"), TypeDefinition::DIRECT_SPEAKERS);
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

    // set references
    audioStreamFormat->setReference(audioChannelFormat);
    audioStreamFormat->setReference(audioPackFormat);
    audioStreamFormat->addReference(
        std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
    audioStreamFormat->addReference(
        std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
    BOOST_TEST(audioStreamFormat->getReference<AudioChannelFormat>() ==
               audioChannelFormat);
    BOOST_TEST(audioStreamFormat->getReference<AudioPackFormat>() ==
               audioPackFormat);
    BOOST_TEST(audioStreamFormat->getAudioTrackFormatReferences().size() == 1);

    // remove references
    audioStreamFormat->removeReference<AudioChannelFormat>();
    audioStreamFormat->removeReference<AudioPackFormat>();
    audioStreamFormat->removeReference(audioTrackFormat);
    BOOST_CHECK(audioStreamFormat->getReference<AudioChannelFormat>() ==
                nullptr);
    BOOST_CHECK(audioStreamFormat->getReference<AudioPackFormat>() == nullptr);
    BOOST_TEST(audioStreamFormat->getAudioTrackFormatReferences().size() == 0);

    // clear references
    audioStreamFormat->addReference(
        std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
    audioStreamFormat->clearReferences<AudioTrackFormat>();
    BOOST_TEST(audioStreamFormat->getAudioTrackFormatReferences().size() == 0);
  }
}
