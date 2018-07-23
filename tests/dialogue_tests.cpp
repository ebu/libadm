#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/dialogue.hpp"

#define BOOST_TEST_MODULE Dialogue
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(dialogue) {
  using namespace adm;
  // NonDialogueContent
  {
    NonDialogueContentKind undefined(NonDialogueContent::UNDEFINED);
    BOOST_TEST(undefined == NonDialogueContent::UNDEFINED);
    NonDialogueContentKind music(NonDialogueContent::MUSIC);
    BOOST_TEST(music == NonDialogueContent::MUSIC);
    NonDialogueContentKind effect(NonDialogueContent::EFFECT);
    BOOST_TEST(effect == NonDialogueContent::EFFECT);
  }
  // DialogueContent
  {
    DialogueContentKind undefined(DialogueContent::UNDEFINED);
    BOOST_TEST(undefined == DialogueContent::UNDEFINED);
    DialogueContentKind dialogue(DialogueContent::DIALOGUE);
    BOOST_TEST(dialogue == DialogueContent::DIALOGUE);
    DialogueContentKind voiceover(DialogueContent::VOICEOVER);
    BOOST_TEST(voiceover == DialogueContent::VOICEOVER);
    DialogueContentKind spokenSubtitles(DialogueContent::SPOKEN_SUBTITLE);
    BOOST_TEST(spokenSubtitles == DialogueContent::SPOKEN_SUBTITLE);
    DialogueContentKind audioDescription(DialogueContent::AUDIO_DESCRIPTION);
    BOOST_TEST(audioDescription == DialogueContent::AUDIO_DESCRIPTION);
    DialogueContentKind commentary(DialogueContent::COMMENTARY);
    BOOST_TEST(commentary == DialogueContent::COMMENTARY);
    DialogueContentKind emergency(DialogueContent::EMERGENCY);
    BOOST_TEST(emergency == DialogueContent::EMERGENCY);
  }
  // MixedContent
  {
    MixedContentKind undefined(MixedContent::UNDEFINED);
    BOOST_TEST(undefined == MixedContent::UNDEFINED);
    MixedContentKind completeMain(MixedContent::COMPLETE_MAIN);
    BOOST_TEST(completeMain == MixedContent::COMPLETE_MAIN);
    MixedContentKind mixed(MixedContent::MIXED);
    BOOST_TEST(mixed == MixedContent::MIXED);
    MixedContentKind hearingImpaired(MixedContent::HEARING_IMPAIRED);
    BOOST_TEST(hearingImpaired == MixedContent::HEARING_IMPAIRED);
  }
}
