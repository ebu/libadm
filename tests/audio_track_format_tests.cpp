#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_format.hpp"

#define BOOST_TEST_MODULE AudioTrackFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_track_format) {
  using namespace adm;
  // Attributes / Elements
  {
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

    audioTrackFormat->set(AudioTrackFormatId(TypeDefinition::OBJECTS,
                                             AudioTrackFormatIdValue(1),
                                             AudioTrackFormatIdCounter(1)));
    audioTrackFormat->set(AudioTrackFormatName("MyNewTrackFormat"));

    BOOST_TEST(audioTrackFormat->has<AudioTrackFormatId>());
    BOOST_TEST(audioTrackFormat->has<AudioTrackFormatName>());
    BOOST_TEST(audioTrackFormat->has<FormatDescriptor>());
    BOOST_TEST(
        audioTrackFormat->get<AudioTrackFormatId>().get<TypeDescriptor>() ==
        TypeDefinition::OBJECTS);
    BOOST_TEST(audioTrackFormat->get<AudioTrackFormatId>()
                   .get<AudioTrackFormatIdValue>() == 1u);
    BOOST_TEST(audioTrackFormat->get<AudioTrackFormatId>()
                   .get<AudioTrackFormatIdCounter>() == 1u);
    BOOST_TEST(audioTrackFormat->get<AudioTrackFormatName>() ==
               "MyNewTrackFormat");
    BOOST_TEST(audioTrackFormat->get<FormatDescriptor>() ==
               FormatDefinition::PCM);
  }

  // References
  {
    auto audioTrackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
    auto audioStreamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);

    // set references
    audioTrackFormat->setReference(audioStreamFormat);
    BOOST_TEST(audioTrackFormat->getReference<AudioStreamFormat>() ==
               audioStreamFormat);
    // remove references
    audioTrackFormat->removeReference<AudioStreamFormat>();
    BOOST_CHECK(audioTrackFormat->getReference<AudioStreamFormat>() == nullptr);
  }
}

BOOST_AUTO_TEST_CASE(auto_back_reference) {
  using namespace adm;
  auto audioTrackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
  auto audioTrackFormat2 = AudioTrackFormat::create(
      AudioTrackFormatName("SecondTrackFormat"), FormatDefinition::PCM);
  auto audioStreamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);

  // set references on AudioTrackFormat
  audioTrackFormat->setReference(audioStreamFormat);
  BOOST_TEST(audioTrackFormat->getReference<AudioStreamFormat>() ==
             audioStreamFormat);
  auto trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  BOOST_TEST(trackRefs.size() == 1);
  BOOST_TEST(trackRefs[0].lock() == audioTrackFormat);

  // remove reference from AudioTrackFormat
  audioTrackFormat->removeReference<AudioStreamFormat>();
  BOOST_TEST(audioTrackFormat->getReference<AudioStreamFormat>() ==
             std::shared_ptr<AudioStreamFormat>(nullptr));
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  BOOST_TEST(trackRefs.empty());

  // add references to AudioStreamFormat
  audioStreamFormat->addReference(
      std::weak_ptr<AudioTrackFormat>(audioTrackFormat));
  audioStreamFormat->addReference(
      std::weak_ptr<AudioTrackFormat>(audioTrackFormat2));
  BOOST_TEST(audioTrackFormat->getReference<AudioStreamFormat>() ==
             audioStreamFormat);
  BOOST_TEST(audioTrackFormat2->getReference<AudioStreamFormat>() ==
             audioStreamFormat);
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  BOOST_TEST(trackRefs.size() == 2);
  BOOST_TEST(trackRefs[0].lock() == audioTrackFormat);
  BOOST_TEST(trackRefs[1].lock() == audioTrackFormat2);

  // remove reference from AudioStreamFormat
  audioStreamFormat->removeReference(audioTrackFormat);
  BOOST_TEST(audioTrackFormat->getReference<AudioStreamFormat>() ==
             std::shared_ptr<AudioStreamFormat>(nullptr));
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  BOOST_TEST(trackRefs.size() == 1);
  BOOST_TEST(trackRefs[0].lock() == audioTrackFormat2);

  audioStreamFormat->removeReference(audioTrackFormat2);
  BOOST_TEST(audioTrackFormat->getReference<AudioStreamFormat>() ==
             std::shared_ptr<AudioStreamFormat>(nullptr));
  trackRefs = audioStreamFormat->getAudioTrackFormatReferences();
  BOOST_TEST(trackRefs.empty());
}
