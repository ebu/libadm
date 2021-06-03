#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_object.hpp"

TEST_CASE("audio_content") {
  using namespace adm;
  {
    auto audioContent = AudioContent::create(AudioContentName("MyContent"));
    audioContent->set(AudioContentId(AudioContentIdValue(1)));
    audioContent->set(AudioContentName("MyNewContent"));
    audioContent->set(AudioContentLanguage("de"));
    audioContent->set(LoudnessMetadata());
    audioContent->set(Dialogue::NON_DIALOGUE);

    REQUIRE(audioContent->has<AudioContentId>() == true);
    REQUIRE(audioContent->has<AudioContentName>() == true);
    REQUIRE(audioContent->has<AudioContentLanguage>() == true);
    REQUIRE(audioContent->has<LoudnessMetadata>() == true);
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
    audioContent->unset<LoudnessMetadata>();
    audioContent->unset<DialogueId>();

    REQUIRE(audioContent->has<AudioContentLanguage>() == false);
    REQUIRE(audioContent->has<LoudnessMetadata>() == false);
    REQUIRE(audioContent->has<DialogueId>() == false);
    REQUIRE(audioContent->has<NonDialogueContentKind>() == false);
    REQUIRE(audioContent->has<DialogueContentKind>() == false);
    REQUIRE(audioContent->has<MixedContentKind>() == false);

    // TODO: replace with check_vector_parameter
    audioContent->add(Label(LabelValue("ohai")));
    REQUIRE(audioContent->has<Labels>());
    audioContent->remove(Label(LabelValue("ohai")));
    REQUIRE(!audioContent->has<Labels>());
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
  using namespace adm;

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
