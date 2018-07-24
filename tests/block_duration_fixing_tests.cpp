#include <catch2/catch.hpp>

#include <adm/utilities/block_duration_assignment.hpp>
#include <adm/elements.hpp>
#include <adm/document.hpp>

#include <memory>

struct BaseSceneFixture {
  BaseSceneFixture() {
    using namespace adm;
    document = Document::create();
    programme = AudioProgramme::create(AudioProgrammeName{"main"});
    content = AudioContent::create(AudioContentName{"main"});
    programme->addReference(content);
    object1 = AudioObject::create(AudioObjectName{"object1"});
    content->addReference(object1);
    pack1 = AudioPackFormat::create(AudioPackFormatName{"pack1"},
                                    TypeDefinition::OBJECTS);
    object1->addReference(pack1);
    channel1 = AudioChannelFormat::create(AudioChannelFormatName{"channel1"},
                                          TypeDefinition::OBJECTS);
    pack1->addReference(channel1);

    document->add(programme);
  }

  std::shared_ptr<adm::Document> document;
  std::shared_ptr<adm::AudioProgramme> programme;
  std::shared_ptr<adm::AudioContent> content;
  std::shared_ptr<adm::AudioObject> object1;
  std::shared_ptr<adm::AudioPackFormat> pack1;
  std::shared_ptr<adm::AudioChannelFormat> channel1;
};

TEST_CASE_METHOD(BaseSceneFixture, "no_durations") {
  using namespace adm;
  REQUIRE(document->getElements<AudioProgramme>().size() == 1);
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  updateBlockFormatDurations(document, std::chrono::seconds(5));

  auto blocks1 = channel1->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks1[0].has<Duration>());

  CHECK(blocks1[0].get<Duration>().get() == std::chrono::milliseconds(100));

  REQUIRE(blocks1[1].has<Duration>());
  CHECK(blocks1[1].get<Duration>().get() ==
        std::chrono::seconds(5) - std::chrono::milliseconds(100));
}

TEST_CASE_METHOD(BaseSceneFixture, "duration_from_programme") {
  using namespace adm;
  programme->set(Start{std::chrono::seconds(0)});
  programme->set(End{std::chrono::seconds(20)});

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  updateBlockFormatDurations(document);

  auto blocks1 = channel1->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks1[0].has<Duration>());
  CHECK(blocks1[0].get<Duration>().get() == std::chrono::milliseconds(100));

  REQUIRE(blocks1[1].has<Duration>());
  CHECK(blocks1[1].get<Duration>().get() ==
        std::chrono::seconds(20) - std::chrono::milliseconds(100));
}
