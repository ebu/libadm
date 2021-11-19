#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_object.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_track_uid.hpp"
#include "adm/errors.hpp"
#include "helper/parameter_checks.hpp"
#include "helper/ostream_operators.hpp"

using namespace adm;
using namespace adm_test;

TEST_CASE("audio_object parameter checks") {
  using namespace std::chrono_literals;

  std::string const name{"MyObject"};
  auto audioObject = AudioObject::create(AudioObjectName(name));
  SECTION("AudioObjectId") {
    check_required_param<AudioObjectId>(audioObject,
                                        hasDefaultOf(AudioObjectIdValue()),
                                        canBeSetTo(AudioObjectIdValue(1u)));
  }
  SECTION("AudioObjectName") {
    check_required_param<AudioObjectName>(
        audioObject, hasDefaultOf(AudioObjectName(name)),
        canBeSetTo(AudioObjectName("SomethingElse")));
  }
  SECTION("Start") {
    check_defaulted_param<Start>(audioObject, hasDefaultOf(Start(0s)),
                                 canBeSetTo(Start(10s)));
  }
  SECTION("Duration") {
    check_optional_param<Duration>(audioObject, canBeSetTo(Duration(10s)));
  }
  SECTION("Dialogue") {
    check_optional_param<DialogueId>(audioObject,
                                     canBeSetTo(Dialogue::NON_DIALOGUE));
  }
  SECTION("Importance") {
    check_optional_param<Importance>(audioObject, canBeSetTo(10));
  }
  SECTION("Interact") {
    check_optional_param<Interact>(audioObject, canBeSetTo(false));
  }
  SECTION("DisableDucking") {
    check_optional_param<DisableDucking>(audioObject, canBeSetTo(true));
  }
  SECTION("AudioObjectInteraction") {
    check_optional_param<AudioObjectInteraction>(
        audioObject, canBeSetTo(OnOffInteract(true)));
  }
  SECTION("Labels") {
    check_vector_param<Labels>(audioObject,
                               canBeSetTo(Labels{Label(LabelValue("ohai"))}));
  }
  SECTION("AudioComplimentaryGroupLabels") {
    check_vector_param<AudioComplementaryObjectGroupLabels>(
        audioObject,
        canBeSetTo(AudioComplementaryObjectGroupLabels{
            AudioComplementaryObjectGroupLabel{Label{LabelValue("ohai")}}}));
  }
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
