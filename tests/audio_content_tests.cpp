#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_object.hpp"

#define BOOST_TEST_MODULE AudioContent
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_content) {
  using namespace adm;
  {
    auto audioContent = AudioContent::create(AudioContentName("MyContent"));
    audioContent->set(AudioContentId(AudioContentIdValue(1)));
    audioContent->set(AudioContentName("MyNewContent"));
    audioContent->set(AudioContentLanguage("de"));
    audioContent->set(LoudnessMetadata());
    audioContent->set(Dialogue::NON_DIALOGUE);

    BOOST_TEST(audioContent->has<AudioContentId>() == true);
    BOOST_TEST(audioContent->has<AudioContentName>() == true);
    BOOST_TEST(audioContent->has<AudioContentLanguage>() == true);
    BOOST_TEST(audioContent->has<LoudnessMetadata>() == true);
    BOOST_TEST(audioContent->has<DialogueId>() == true);
    BOOST_TEST(audioContent->has<NonDialogueContentKind>() == true);
    BOOST_TEST(audioContent->has<DialogueContentKind>() == false);
    BOOST_TEST(audioContent->has<MixedContentKind>() == false);

    BOOST_TEST(audioContent->get<AudioContentId>().get<AudioContentIdValue>() ==
               1u);
    BOOST_TEST(audioContent->get<AudioContentName>() == "MyNewContent");
    BOOST_TEST(audioContent->get<AudioContentLanguage>() == "de");
    BOOST_TEST(audioContent->get<LoudnessMetadata>().method == "");
    BOOST_TEST(audioContent->get<LoudnessMetadata>().recType == "");
    BOOST_TEST(audioContent->get<LoudnessMetadata>().correctionType == "");
    BOOST_TEST(audioContent->get<DialogueId>() == Dialogue::NON_DIALOGUE);

    audioContent->unset<AudioContentLanguage>();
    audioContent->unset<LoudnessMetadata>();
    audioContent->unset<DialogueId>();

    BOOST_TEST(audioContent->has<AudioContentLanguage>() == false);
    BOOST_TEST(audioContent->has<LoudnessMetadata>() == false);
    BOOST_TEST(audioContent->has<DialogueId>() == false);
    BOOST_TEST(audioContent->has<NonDialogueContentKind>() == false);
    BOOST_TEST(audioContent->has<DialogueContentKind>() == false);
    BOOST_TEST(audioContent->has<MixedContentKind>() == false);
  }
  // References
  {
    auto audioContent = AudioContent::create(AudioContentName("MyContent"));

    auto referencedAudioObject =
        AudioObject::create(AudioObjectName("MyReferencedObject"));

    // add references
    audioContent->addReference(referencedAudioObject);
    audioContent->addReference(referencedAudioObject);
    BOOST_TEST(audioContent->getReferences<AudioObject>().size() == 1);

    // remove references
    audioContent->removeReference(referencedAudioObject);
    BOOST_TEST(audioContent->getReferences<AudioObject>().size() == 0);

    // clear references
    audioContent->addReference(referencedAudioObject);
    audioContent->clearReferences<AudioObject>();
    BOOST_TEST(audioContent->getReferences<AudioObject>().size() == 0);
  }
}

BOOST_AUTO_TEST_CASE(audio_content_dialogue_interdependencies) {
  using namespace adm;

  auto audioContent = AudioContent::create(AudioContentName("MyContent"));

  audioContent->set(DialogueContent::VOICEOVER);
  BOOST_TEST(audioContent->get<DialogueId>() == Dialogue::DIALOGUE);

  // setting this to the (automatically) set value should not change the content
  // kind
  audioContent->set(Dialogue::DIALOGUE);
  BOOST_TEST(audioContent->get<DialogueContentKind>() ==
             DialogueContent::VOICEOVER);

  // but setting it to a different value does
  audioContent->set(Dialogue::NON_DIALOGUE);
  BOOST_TEST(audioContent->get<NonDialogueContentKind>() ==
             NonDialogueContent::UNDEFINED);
  BOOST_TEST(!audioContent->has<DialogueContentKind>());
  BOOST_TEST(!audioContent->has<MixedContentKind>());

  // this must also work for the NonDialog type
  audioContent->set(NonDialogueContent::MUSIC);
  BOOST_TEST(audioContent->get<DialogueId>() == Dialogue::NON_DIALOGUE);
  audioContent->set(Dialogue::NON_DIALOGUE);
  BOOST_TEST(audioContent->get<NonDialogueContentKind>() ==
             NonDialogueContent::MUSIC);

  audioContent->set(Dialogue::MIXED);
  BOOST_TEST(audioContent->get<MixedContentKind>() == MixedContent::UNDEFINED);
  BOOST_TEST(!audioContent->has<DialogueContentKind>());
  BOOST_TEST(!audioContent->has<NonDialogueContentKind>());

  // this must also work for the Mixed type
  audioContent->set(MixedContent::HEARING_IMPAIRED);
  BOOST_TEST(audioContent->get<DialogueId>() == Dialogue::MIXED);
  audioContent->set(Dialogue::MIXED);
  BOOST_TEST(audioContent->get<MixedContentKind>() ==
             MixedContent::HEARING_IMPAIRED);

  audioContent->set(Dialogue::DIALOGUE);
  BOOST_TEST(audioContent->get<DialogueContentKind>() ==
             DialogueContent::UNDEFINED);
  BOOST_TEST(!audioContent->has<MixedContentKind>());
  BOOST_TEST(!audioContent->has<NonDialogueContentKind>());
}
