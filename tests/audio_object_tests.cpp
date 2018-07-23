#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_object.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_uid.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE AudioObject
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_object_basic) {
  using namespace adm;

  auto audioObject = AudioObject::create(AudioObjectName("MyObject"));

  audioObject->set(AudioObjectId(AudioObjectIdValue(1)));
  audioObject->set(AudioObjectName("MyNewObject"));
  audioObject->set(Start(std::chrono::seconds(0)));
  audioObject->set(Duration(std::chrono::seconds(10)));
  audioObject->set(Dialogue::NON_DIALOGUE);
  audioObject->set(Importance(10));
  audioObject->set(Interact(false));
  audioObject->set(DisableDucking(true));
  audioObject->set(AudioObjectInteraction(OnOffInteract(true)));

  BOOST_TEST(audioObject->has<AudioObjectId>() == true);
  BOOST_TEST(audioObject->has<AudioObjectName>() == true);
  BOOST_TEST(audioObject->has<Start>() == true);
  BOOST_TEST(audioObject->has<Duration>() == true);
  BOOST_TEST(audioObject->has<DialogueId>() == true);
  BOOST_TEST(audioObject->has<Importance>() == true);
  BOOST_TEST(audioObject->has<Interact>() == true);
  BOOST_TEST(audioObject->has<DisableDucking>() == true);
  BOOST_TEST(audioObject->has<AudioObjectInteraction>() == true);

  BOOST_TEST(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() == 1u);
  BOOST_TEST(audioObject->get<AudioObjectName>() == "MyNewObject");
  BOOST_CHECK(audioObject->get<Start>() == std::chrono::seconds(0));
  BOOST_CHECK(audioObject->get<Duration>() == std::chrono::seconds(10));
  BOOST_TEST(audioObject->get<DialogueId>() == Dialogue::NON_DIALOGUE);
  BOOST_TEST(audioObject->get<Importance>() == 10);
  BOOST_TEST(audioObject->get<Interact>() == false);
  BOOST_TEST(audioObject->get<DisableDucking>() == true);
  BOOST_TEST(audioObject->get<AudioObjectInteraction>().get<OnOffInteract>() ==
             true);

  audioObject->unset<Start>();
  audioObject->unset<Duration>();
  audioObject->unset<DialogueId>();
  audioObject->unset<Importance>();
  audioObject->unset<Interact>();
  audioObject->unset<DisableDucking>();
  audioObject->unset<AudioObjectInteraction>();

  BOOST_TEST(audioObject->has<Start>() == true);
  BOOST_TEST(audioObject->has<Duration>() == false);
  BOOST_TEST(audioObject->has<DialogueId>() == false);
  BOOST_TEST(audioObject->has<Importance>() == false);
  BOOST_TEST(audioObject->has<Interact>() == false);
  BOOST_TEST(audioObject->has<DisableDucking>() == false);
  BOOST_TEST(audioObject->has<AudioObjectInteraction>() == false);
}

BOOST_AUTO_TEST_CASE(audio_object_references) {
  using namespace adm;

  auto audioObject = AudioObject::create(AudioObjectName("MyObject"));

  auto referencedAudioPackFormat = AudioPackFormat::create(
      AudioPackFormatName("MyPackFormat"), TypeDefinition::OBJECTS);
  auto referencedAudioTrackUid = AudioTrackUid::create();
  auto referencedAudioObject =
      AudioObject::create(AudioObjectName("MyReferencedObject"));
  auto referencedAudioComplementaryObject =
      AudioObject::create(AudioObjectName("MyComplementaryObject"));

  // add references
  audioObject->addReference(referencedAudioObject);
  audioObject->addReference(referencedAudioObject);
  audioObject->addReference(referencedAudioPackFormat);
  audioObject->addReference(referencedAudioPackFormat);
  audioObject->addReference(referencedAudioTrackUid);
  audioObject->addReference(referencedAudioTrackUid);

  BOOST_TEST(audioObject->getReferences<AudioObject>().size() == 1);
  BOOST_TEST(audioObject->getReferences<AudioPackFormat>().size() == 1);
  BOOST_TEST(audioObject->getReferences<AudioTrackUid>().size() == 1);

  // remove references
  audioObject->removeReference(referencedAudioObject);
  audioObject->removeReference(referencedAudioPackFormat);
  audioObject->removeReference(referencedAudioTrackUid);

  BOOST_TEST(audioObject->getReferences<AudioObject>().size() == 0);
  BOOST_TEST(audioObject->getReferences<AudioPackFormat>().size() == 0);
  BOOST_TEST(audioObject->getReferences<AudioTrackUid>().size() == 0);

  // clear references
  audioObject->addReference(referencedAudioObject);
  audioObject->addReference(referencedAudioPackFormat);
  audioObject->addReference(referencedAudioTrackUid);
  audioObject->clearReferences<AudioObject>();
  audioObject->clearReferences<AudioPackFormat>();
  audioObject->clearReferences<AudioTrackUid>();
  BOOST_TEST(audioObject->getReferences<AudioObject>().size() == 0);
  BOOST_TEST(audioObject->getReferences<AudioPackFormat>().size() == 0);
  BOOST_TEST(audioObject->getReferences<AudioTrackUid>().size() == 0);
}

BOOST_AUTO_TEST_CASE(audio_object_referenc_cycle_detection) {
  using namespace adm;
  // reference cycles AudioObjects
  auto audioObject1 = AudioObject::create(AudioObjectName("First"));
  auto audioObject2 = AudioObject::create(AudioObjectName("Second"));
  auto audioObject3 = AudioObject::create(AudioObjectName("Third"));

  BOOST_CHECK_THROW(audioObject1->addReference(audioObject1),
                    error::AudioObjectReferenceCycle);
  audioObject1->addReference(audioObject2);
  audioObject2->addReference(audioObject3);
  BOOST_CHECK_THROW(audioObject3->addReference(audioObject1),
                    error::AudioObjectReferenceCycle);

  // reference cycles complementary AudioObjects
  auto audioObjectComp1 = AudioObject::create(AudioObjectName("First"));
  auto audioObjectComp2 = AudioObject::create(AudioObjectName("Second"));
  auto audioObjectComp3 = AudioObject::create(AudioObjectName("Third"));

  BOOST_CHECK_THROW(audioObjectComp1->addReference(audioObjectComp1),
                    error::AudioObjectReferenceCycle);
  audioObjectComp1->addReference(audioObjectComp2);
  audioObjectComp2->addReference(audioObjectComp3);
  BOOST_CHECK_THROW(audioObjectComp3->addReference(audioObjectComp1),
                    error::AudioObjectReferenceCycle);
}

BOOST_AUTO_TEST_CASE(audio_object_complementary) {
  using namespace adm;

  auto audioObject = AudioObject::create(AudioObjectName("MyAudioObject"));
  auto compAudioObject =
      AudioObject::create(AudioObjectName("MyComplementaryAudioObject"));
  audioObject->addComplementary(compAudioObject);
  audioObject->addComplementary(compAudioObject);
  BOOST_TEST(audioObject->getComplementaryObjects().size() == 1);
  audioObject->removeComplementary(compAudioObject);
  BOOST_TEST(audioObject->getComplementaryObjects().size() == 0);
  audioObject->addComplementary(compAudioObject);
  audioObject->clearComplementaryObjects();
  BOOST_TEST(audioObject->getComplementaryObjects().size() == 0);
}
