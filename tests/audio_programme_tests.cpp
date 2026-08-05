#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements/audio_programme.hpp"
#include "adm/elements/loudness_renderer.hpp"
#include "helper/parameter_checks.hpp"
#include "helper/ostream_operators.hpp"

using namespace adm;
using namespace adm_test;

TEST_CASE("audio_programme parameters") {
  using std::chrono::seconds;
  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  SECTION("AudioProgrammeName") {
    check_required_param<AudioProgrammeName>(audioProgramme,
                                             hasDefaultOf("MyProgramme"),
                                             canBeSetTo("SomethingElse"));
  }
  SECTION("AudioProgrammeId") {
    check_required_param<AudioProgrammeId>(
        audioProgramme, hasDefaultOf(AudioProgrammeIdValue{}),
        canBeSetTo(AudioProgrammeIdValue(2u)));
  }
  SECTION("AudioProgrammeLanguage") {
    check_optional_param<AudioProgrammeLanguage>(audioProgramme,
                                                 canBeSetTo("fr"));
  }
  SECTION("Start") {
    check_defaulted_param<Start>(audioProgramme, hasDefaultOf(seconds{0}),
                                 canBeSetTo(seconds{1}));
  }
  SECTION("End") {
    check_optional_param<End>(audioProgramme, canBeSetTo(seconds(10)));
  }
  SECTION("LoudnessMetadatas") {
    LoudnessMetadatas loudness = LoudnessMetadatas{
        LoudnessMetadata{}, LoudnessMetadata{LoudnessMethod{"Guess"}}};
    SECTION("check get/set") {
      check_optional_param<LoudnessMetadatas>(audioProgramme,
                                              canBeSetTo(loudness));
    }
    SECTION("add/remove") {
      check_vector_param<LoudnessMetadatas>(audioProgramme,
                                            canBeSetTo(loudness));
    }
  }
  SECTION("MaxDuckingDepth") {
    check_optional_param<MaxDuckingDepth>(audioProgramme, canBeSetTo(-30.0));
  }
  SECTION("Labels") {
    Labels labels{Label{LabelValue{"ohai"}}};
    SECTION("get/set") {
      check_optional_param<Labels>(audioProgramme, canBeSetTo(labels));
    }
    check_vector_param<Labels>(audioProgramme, canBeSetTo(labels));
  }
}

TEST_CASE("audio_programme references") {
  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("MyProgramme"));

  auto referencedAudioContent =
      AudioContent::create(AudioContentName("MyContent"));

  // add references
  audioProgramme->addReference(referencedAudioContent);
  audioProgramme->addReference(referencedAudioContent);
  REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 1);

  // remove references
  audioProgramme->removeReference(referencedAudioContent);
  REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 0);

  // clear references
  audioProgramme->addReference(referencedAudioContent);
  audioProgramme->clearReferences<AudioContent>();
  REQUIRE(audioProgramme->getReferences<AudioContent>().size() == 0);
}

TEST_CASE("audio_programme_loudness_metadata_add_parentage") {
  auto document = Document::create();
  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  document->add(audioProgramme);

  auto object = AudioObject::create(AudioObjectName("MyObject"));
  LoudnessRenderer renderer;
  renderer.addReference(object);
  LoudnessMetadata metadata;
  metadata.set(renderer);

  REQUIRE(audioProgramme->add(metadata));
  auto storedMetadata = audioProgramme->get<LoudnessMetadatas>().front();
  auto storedRenderer = storedMetadata.get<LoudnessRenderer>();
  REQUIRE(storedRenderer.getParent().lock() == document);
  REQUIRE(object->getParent().lock() == document);
}
