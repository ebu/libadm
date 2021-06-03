#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_object.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_uid.hpp"
#include "adm/errors.hpp"

TEST_CASE("audio_object_basic") {
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

  REQUIRE(audioObject->has<AudioObjectId>() == true);
  REQUIRE(audioObject->has<AudioObjectName>() == true);
  REQUIRE(audioObject->has<Start>() == true);
  REQUIRE(audioObject->has<Duration>() == true);
  REQUIRE(audioObject->has<DialogueId>() == true);
  REQUIRE(audioObject->has<Importance>() == true);
  REQUIRE(audioObject->has<Interact>() == true);
  REQUIRE(audioObject->has<DisableDucking>() == true);
  REQUIRE(audioObject->has<AudioObjectInteraction>() == true);

  REQUIRE(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() == 1u);
  REQUIRE(audioObject->get<AudioObjectName>() == "MyNewObject");
  REQUIRE(audioObject->get<Start>().get() == std::chrono::seconds(0));
  REQUIRE(audioObject->get<Duration>().get() == std::chrono::seconds(10));
  REQUIRE(audioObject->get<DialogueId>() == Dialogue::NON_DIALOGUE);
  REQUIRE(audioObject->get<Importance>() == 10);
  REQUIRE(audioObject->get<Interact>() == false);
  REQUIRE(audioObject->get<DisableDucking>() == true);
  REQUIRE(audioObject->get<AudioObjectInteraction>().get<OnOffInteract>() ==
          true);

  audioObject->unset<Start>();
  audioObject->unset<Duration>();
  audioObject->unset<DialogueId>();
  audioObject->unset<Importance>();
  audioObject->unset<Interact>();
  audioObject->unset<DisableDucking>();
  audioObject->unset<AudioObjectInteraction>();

  REQUIRE(audioObject->has<Start>() == true);
  REQUIRE(audioObject->has<Duration>() == false);
  REQUIRE(audioObject->has<DialogueId>() == false);
  REQUIRE(audioObject->has<Importance>() == false);
  REQUIRE(audioObject->has<Interact>() == false);
  REQUIRE(audioObject->has<DisableDucking>() == false);
  REQUIRE(audioObject->has<AudioObjectInteraction>() == false);

  // TODO: replace with check_vector_parameter
  audioObject->add(Label(LabelValue("ohai")));
  REQUIRE(audioObject->has<Labels>());
  audioObject->remove(Label(LabelValue("ohai")));
  REQUIRE(!audioObject->has<Labels>());

  audioObject->add(
      AudioComplementaryObjectGroupLabel(Label(LabelValue("ohai"))));
  REQUIRE(audioObject->has<AudioComplementaryObjectGroupLabels>());
  audioObject->remove(
      AudioComplementaryObjectGroupLabel(Label(LabelValue("ohai"))));
  REQUIRE(!audioObject->has<AudioComplementaryObjectGroupLabels>());
}

TEST_CASE("audio_object_references") {
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

  REQUIRE(audioObject->getReferences<AudioObject>().size() == 1);
  REQUIRE(audioObject->getReferences<AudioPackFormat>().size() == 1);
  REQUIRE(audioObject->getReferences<AudioTrackUid>().size() == 1);

  // remove references
  audioObject->removeReference(referencedAudioObject);
  audioObject->removeReference(referencedAudioPackFormat);
  audioObject->removeReference(referencedAudioTrackUid);

  REQUIRE(audioObject->getReferences<AudioObject>().size() == 0);
  REQUIRE(audioObject->getReferences<AudioPackFormat>().size() == 0);
  REQUIRE(audioObject->getReferences<AudioTrackUid>().size() == 0);

  // clear references
  audioObject->addReference(referencedAudioObject);
  audioObject->addReference(referencedAudioPackFormat);
  audioObject->addReference(referencedAudioTrackUid);
  audioObject->clearReferences<AudioObject>();
  audioObject->clearReferences<AudioPackFormat>();
  audioObject->clearReferences<AudioTrackUid>();
  REQUIRE(audioObject->getReferences<AudioObject>().size() == 0);
  REQUIRE(audioObject->getReferences<AudioPackFormat>().size() == 0);
  REQUIRE(audioObject->getReferences<AudioTrackUid>().size() == 0);
}

TEST_CASE("audio_object_reference_cycle_detection") {
  using namespace adm;
  // reference cycles AudioObjects
  auto audioObject1 = AudioObject::create(AudioObjectName("First"));
  auto audioObject2 = AudioObject::create(AudioObjectName("Second"));
  auto audioObject3 = AudioObject::create(AudioObjectName("Third"));

  REQUIRE_THROWS_AS(audioObject1->addReference(audioObject1),
                    error::AudioObjectReferenceCycle);
  audioObject1->addReference(audioObject2);
  audioObject2->addReference(audioObject3);
  REQUIRE_THROWS_AS(audioObject3->addReference(audioObject1),
                    error::AudioObjectReferenceCycle);

  // reference cycles complementary AudioObjects
  auto audioObjectComp1 = AudioObject::create(AudioObjectName("First"));
  auto audioObjectComp2 = AudioObject::create(AudioObjectName("Second"));
  auto audioObjectComp3 = AudioObject::create(AudioObjectName("Third"));

  REQUIRE_THROWS_AS(audioObjectComp1->addReference(audioObjectComp1),
                    error::AudioObjectReferenceCycle);
  audioObjectComp1->addReference(audioObjectComp2);
  audioObjectComp2->addReference(audioObjectComp3);
  REQUIRE_THROWS_AS(audioObjectComp3->addReference(audioObjectComp1),
                    error::AudioObjectReferenceCycle);
}

TEST_CASE("audio_object_complementary") {
  using namespace adm;

  auto audioObject = AudioObject::create(AudioObjectName("MyAudioObject"));
  auto compAudioObject =
      AudioObject::create(AudioObjectName("MyComplementaryAudioObject"));
  audioObject->addComplementary(compAudioObject);
  audioObject->addComplementary(compAudioObject);
  REQUIRE(audioObject->getComplementaryObjects().size() == 1);
  audioObject->removeComplementary(compAudioObject);
  REQUIRE(audioObject->getComplementaryObjects().size() == 0);
  audioObject->addComplementary(compAudioObject);
  audioObject->clearComplementaryObjects();
  REQUIRE(audioObject->getComplementaryObjects().size() == 0);
}
