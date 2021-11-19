#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_object.hpp"
#include "helper/parameter_checks.hpp"
#include "adm/utilities/element_io.hpp"

using namespace adm;
TEST_CASE("audio_content parameters") {
  using namespace adm_test;
  auto contentName = AudioContentName("MyContent");
  auto audioContent = AudioContent::create(AudioContentName(contentName));
  SECTION("AudioContentName") {
    check_required_param<AudioContentName>(
        audioContent, hasDefaultOf(contentName),
        canBeSetTo(AudioContentName("SomethingElse")));
  }
  SECTION("AudioContentId") {
    check_required_param<AudioContentId>(
        audioContent, hasDefaultOf(AudioContentId{}),
        canBeSetTo(AudioContentId(AudioContentIdValue(2))));
  }
  SECTION("AudioContentLanguage") {
    check_optional_param<AudioContentLanguage>(
        audioContent, canBeSetTo(AudioContentLanguage("de")));
  }
  SECTION("DialogId") {
    check_optional_param<DialogueId>(audioContent,
                                     canBeSetTo(Dialogue::NON_DIALOGUE));
  }
  SECTION("ContentKind") {
    check_optional_param<ContentKind>(
        audioContent, canBeSetTo(ContentKind{MixedContentKind{}}));
  }
  SECTION("ContentKind variants") {
    SECTION("Dialog") {
      check_optional_param<DialogueContentKind>(
          audioContent, canBeSetTo(DialogueContent::COMMENTARY));
    }
    SECTION("NonDialog") {
      check_optional_param<NonDialogueContentKind>(
          audioContent, canBeSetTo(NonDialogueContent::EFFECT));
    }
    SECTION("Mixed") {
      check_optional_param<MixedContentKind>(
          audioContent, canBeSetTo(MixedContent::HEARING_IMPAIRED));
    }
  }
  SECTION("LoudnessMetadatas") {
    LoudnessMetadatas loudness = LoudnessMetadatas{
        LoudnessMetadata{}, LoudnessMetadata{LoudnessMethod{"Guess"}}};
    SECTION("check get/set") {
      check_optional_param<LoudnessMetadatas>(audioContent,
                                              canBeSetTo(loudness));
    }
    SECTION("add/remove") {
      check_vector_param<LoudnessMetadatas>(audioContent, canBeSetTo(loudness));
    }
  }
  SECTION("Labels") {
    Labels labels{Label(LabelValue("ohai"))};
    SECTION("get/set") {
      check_optional_param<Labels>(audioContent, canBeSetTo(labels));
    }
    SECTION("add/remove") {
      check_vector_param<Labels>(audioContent, canBeSetTo(labels));
    }
  }
}

TEST_CASE("audio_content") {
  {
    auto audioContent = AudioContent::create(AudioContentName("MyContent"));
    audioContent->set(AudioContentId(AudioContentIdValue(1)));
    audioContent->set(AudioContentName("MyNewContent"));
    audioContent->set(AudioContentLanguage("de"));
    audioContent->add(LoudnessMetadata());
    audioContent->set(Dialogue::NON_DIALOGUE);

    REQUIRE(audioContent->has<AudioContentId>() == true);
    REQUIRE(audioContent->has<AudioContentName>() == true);
    REQUIRE(audioContent->has<AudioContentLanguage>() == true);
    REQUIRE(audioContent->has<LoudnessMetadatas>() == true);
    REQUIRE(audioContent->has<DialogueId>() == true);
    REQUIRE(audioContent->has<NonDialogueContentKind>() == true);
    REQUIRE(audioContent->has<DialogueContentKind>() == false);
    REQUIRE(audioContent->has<MixedContentKind>() == false);

    REQUIRE(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
            1u);
    REQUIRE(audioContent->get<AudioContentName>() == "MyNewContent");
    REQUIRE(audioContent->get<AudioContentLanguage>() == "de");
    REQUIRE(audioContent->get<DialogueId>() == Dialogue::NON_DIALOGUE);

    audioContent->unset<AudioContentLanguage>();
    audioContent->unset<LoudnessMetadatas>();
    audioContent->unset<DialogueId>();

    REQUIRE(audioContent->has<AudioContentLanguage>() == false);
    REQUIRE(audioContent->has<LoudnessMetadatas>() == false);
    REQUIRE(audioContent->has<DialogueId>() == false);
    REQUIRE(audioContent->has<NonDialogueContentKind>() == false);
    REQUIRE(audioContent->has<DialogueContentKind>() == false);
    REQUIRE(audioContent->has<MixedContentKind>() == false);
  }
  // References
  {
    auto audioContent = AudioContent::create(AudioContentName("MyContent"));

    auto referencedAudioObject =
        AudioObject::create(AudioObjectName("MyReferencedObject"));

    // add references
    audioContent->addReference(referencedAudioObject);
    audioContent->addReference(referencedAudioObject);
    REQUIRE(audioContent->getReferences<AudioObject>().size() == 1);

    // remove references
    audioContent->removeReference(referencedAudioObject);
    REQUIRE(audioContent->getReferences<AudioObject>().size() == 0);

    // clear references
    audioContent->addReference(referencedAudioObject);
    audioContent->clearReferences<AudioObject>();
    REQUIRE(audioContent->getReferences<AudioObject>().size() == 0);
  }
}

TEST_CASE("audio_content_dialogue_interdependencies") {
  auto audioContent = AudioContent::create(AudioContentName("MyContent"));

  audioContent->set(DialogueContent::VOICEOVER);
  REQUIRE(audioContent->get<DialogueId>() == Dialogue::DIALOGUE);

  // setting this to the (automatically) set value should not change the content
  // kind
  audioContent->set(Dialogue::DIALOGUE);
  REQUIRE(audioContent->get<DialogueContentKind>() ==
          DialogueContent::VOICEOVER);

  // but setting it to a different value does
  audioContent->set(Dialogue::NON_DIALOGUE);
  REQUIRE(audioContent->get<NonDialogueContentKind>() ==
          NonDialogueContent::UNDEFINED);
  REQUIRE(!audioContent->has<DialogueContentKind>());
  REQUIRE(!audioContent->has<MixedContentKind>());

  // this must also work for the NonDialog type
  audioContent->set(NonDialogueContent::MUSIC);
  REQUIRE(audioContent->get<DialogueId>() == Dialogue::NON_DIALOGUE);
  audioContent->set(Dialogue::NON_DIALOGUE);
  REQUIRE(audioContent->get<NonDialogueContentKind>() ==
          NonDialogueContent::MUSIC);

  audioContent->set(Dialogue::MIXED);
  REQUIRE(audioContent->get<MixedContentKind>() == MixedContent::UNDEFINED);
  REQUIRE(!audioContent->has<DialogueContentKind>());
  REQUIRE(!audioContent->has<NonDialogueContentKind>());

  // this must also work for the Mixed type
  audioContent->set(MixedContent::HEARING_IMPAIRED);
  REQUIRE(audioContent->get<DialogueId>() == Dialogue::MIXED);
  audioContent->set(Dialogue::MIXED);
  REQUIRE(audioContent->get<MixedContentKind>() ==
          MixedContent::HEARING_IMPAIRED);

  audioContent->set(Dialogue::DIALOGUE);
  REQUIRE(audioContent->get<DialogueContentKind>() ==
          DialogueContent::UNDEFINED);
  REQUIRE(!audioContent->has<MixedContentKind>());
  REQUIRE(!audioContent->has<NonDialogueContentKind>());
}
