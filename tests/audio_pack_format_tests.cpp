#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_pack_format.hpp"

#define BOOST_TEST_MODULE AudioPackFormat
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_pack_format) {
  using namespace adm;
  // Attributes / Elements
  {
    auto packFormat =
        AudioPackFormat::create(AudioPackFormatName("MyPackFormat"),
                                parseTypeDefinition("DirectSpeakers"));
    packFormat->set(AudioPackFormatId(TypeDefinition::DIRECT_SPEAKERS,
                                      AudioPackFormatIdValue(1)));
    packFormat->set(AudioPackFormatName("MyNewPackFormat"));
    packFormat->set(Importance(5));
    packFormat->set(AbsoluteDistance(4.5));

    BOOST_TEST(packFormat->has<AudioPackFormatId>());
    BOOST_TEST(packFormat->has<AudioPackFormatName>());
    BOOST_TEST(packFormat->has<TypeDescriptor>());
    BOOST_TEST(packFormat->has<Importance>());
    BOOST_TEST(packFormat->has<AbsoluteDistance>());

    BOOST_TEST(packFormat->get<AudioPackFormatId>().get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(
        packFormat->get<AudioPackFormatId>().get<AudioPackFormatIdValue>() ==
        1u);
    BOOST_TEST(packFormat->get<AudioPackFormatName>() == "MyNewPackFormat");
    BOOST_TEST(packFormat->get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(packFormat->get<Importance>() == 5);
    BOOST_TEST(packFormat->get<AbsoluteDistance>() == 4.5);

    packFormat->unset<Importance>();
    packFormat->unset<AbsoluteDistance>();

    BOOST_TEST(!packFormat->has<Importance>());
    BOOST_TEST(!packFormat->has<AbsoluteDistance>());
  }
  // References
  {
    auto audioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyPackFormat"), TypeDefinition::OBJECTS);

    auto referencedAudioPackFormat = AudioPackFormat::create(
        AudioPackFormatName("MyReferencedPackFormat"), TypeDefinition::OBJECTS);
    auto referencedAudioChannelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("MyChannelFormat"), TypeDefinition::OBJECTS);

    // add references
    audioPackFormat->addReference(referencedAudioChannelFormat);
    audioPackFormat->addReference(referencedAudioChannelFormat);
    audioPackFormat->addReference(referencedAudioPackFormat);
    audioPackFormat->addReference(referencedAudioPackFormat);

    BOOST_TEST(audioPackFormat->getReferences<AudioChannelFormat>().size() ==
               1);
    BOOST_TEST(audioPackFormat->getReferences<AudioPackFormat>().size() == 1);

    // remove references
    audioPackFormat->removeReference(referencedAudioChannelFormat);
    audioPackFormat->removeReference(referencedAudioPackFormat);

    BOOST_TEST(audioPackFormat->getReferences<AudioChannelFormat>().size() ==
               0);
    BOOST_TEST(audioPackFormat->getReferences<AudioPackFormat>().size() == 0);

    // clear references
    audioPackFormat->addReference(referencedAudioChannelFormat);
    audioPackFormat->addReference(referencedAudioPackFormat);
    audioPackFormat->clearReferences<AudioChannelFormat>();
    audioPackFormat->clearReferences<AudioPackFormat>();
    BOOST_TEST(audioPackFormat->getReferences<AudioChannelFormat>().size() ==
               0);
    BOOST_TEST(audioPackFormat->getReferences<AudioPackFormat>().size() == 0);

    // reference cycles AudioObjects
    auto audioPackFormat1 = AudioPackFormat::create(
        AudioPackFormatName("First"), TypeDefinition::OBJECTS);
    auto audioPackFormat2 = AudioPackFormat::create(
        AudioPackFormatName("Second"), TypeDefinition::OBJECTS);
    auto audioPackFormat3 = AudioPackFormat::create(
        AudioPackFormatName("Third"), TypeDefinition::OBJECTS);

    BOOST_CHECK_THROW(audioPackFormat1->addReference(audioPackFormat1),
                      std::runtime_error);
    audioPackFormat1->addReference(audioPackFormat2);
    audioPackFormat2->addReference(audioPackFormat3);
    BOOST_CHECK_THROW(audioPackFormat3->addReference(audioPackFormat1),
                      std::runtime_error);
  }
}
