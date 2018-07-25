#include <catch2/catch.hpp>
#include "adm/elements/dialogue.hpp"

TEST_CASE("dialogue") {
  using namespace adm;
  // NonDialogueContent
  {
    NonDialogueContentKind undefined(NonDialogueContent::UNDEFINED);
    REQUIRE(undefined == NonDialogueContent::UNDEFINED);
    NonDialogueContentKind music(NonDialogueContent::MUSIC);
    REQUIRE(music == NonDialogueContent::MUSIC);
    NonDialogueContentKind effect(NonDialogueContent::EFFECT);
    REQUIRE(effect == NonDialogueContent::EFFECT);
  }
  // DialogueContent
  {
    DialogueContentKind undefined(DialogueContent::UNDEFINED);
    REQUIRE(undefined == DialogueContent::UNDEFINED);
    DialogueContentKind dialogue(DialogueContent::DIALOGUE);
    REQUIRE(dialogue == DialogueContent::DIALOGUE);
    DialogueContentKind voiceover(DialogueContent::VOICEOVER);
    REQUIRE(voiceover == DialogueContent::VOICEOVER);
    DialogueContentKind spokenSubtitles(DialogueContent::SPOKEN_SUBTITLE);
    REQUIRE(spokenSubtitles == DialogueContent::SPOKEN_SUBTITLE);
    DialogueContentKind audioDescription(DialogueContent::AUDIO_DESCRIPTION);
    REQUIRE(audioDescription == DialogueContent::AUDIO_DESCRIPTION);
    DialogueContentKind commentary(DialogueContent::COMMENTARY);
    REQUIRE(commentary == DialogueContent::COMMENTARY);
    DialogueContentKind emergency(DialogueContent::EMERGENCY);
    REQUIRE(emergency == DialogueContent::EMERGENCY);
  }
  // MixedContent
  {
    MixedContentKind undefined(MixedContent::UNDEFINED);
    REQUIRE(undefined == MixedContent::UNDEFINED);
    MixedContentKind completeMain(MixedContent::COMPLETE_MAIN);
    REQUIRE(completeMain == MixedContent::COMPLETE_MAIN);
    MixedContentKind mixed(MixedContent::MIXED);
    REQUIRE(mixed == MixedContent::MIXED);
    MixedContentKind hearingImpaired(MixedContent::HEARING_IMPAIRED);
    REQUIRE(hearingImpaired == MixedContent::HEARING_IMPAIRED);
  }
}
