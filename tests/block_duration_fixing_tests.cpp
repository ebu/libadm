#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>

#include <adm/utilities/block_duration_assignment.hpp>
#include <adm/elements.hpp>
#include <adm/document.hpp>
#include <adm/errors.hpp>
#include <memory>

struct BaseSceneFixture {
  BaseSceneFixture() {
    using namespace adm;
    document = Document::create();
    programme = AudioProgramme::create(AudioProgrammeName{"main"});
    content1 = AudioContent::create(AudioContentName{"main"});
    programme->addReference(content1);
    object1 = AudioObject::create(AudioObjectName{"object1"});
    content1->addReference(object1);
    pack1 = AudioPackFormat::create(AudioPackFormatName{"pack1"},
                                    TypeDefinition::OBJECTS);
    object1->addReference(pack1);
    channel1 = AudioChannelFormat::create(AudioChannelFormatName{"channel1"},
                                          TypeDefinition::OBJECTS);
    pack1->addReference(channel1);

    document->add(programme);

    content2 = AudioContent::create(AudioContentName{"secondary"});
    object2 = AudioObject::create(AudioObjectName{"object2"});
    content2->addReference(object2);
    pack2 = AudioPackFormat::create(AudioPackFormatName{"pack2"},
                                    TypeDefinition::OBJECTS);
    object2->addReference(pack2);
    channel2 = AudioChannelFormat::create(AudioChannelFormatName{"channel2"},
                                          TypeDefinition::OBJECTS);
    pack2->addReference(channel2);
    document->add(content2);
  }

  std::shared_ptr<adm::Document> document;
  std::shared_ptr<adm::AudioProgramme> programme;
  std::shared_ptr<adm::AudioContent> content1;
  std::shared_ptr<adm::AudioObject> object1;
  std::shared_ptr<adm::AudioPackFormat> pack1;
  std::shared_ptr<adm::AudioChannelFormat> channel1;
  std::shared_ptr<adm::AudioContent> content2;
  std::shared_ptr<adm::AudioObject> object2;
  std::shared_ptr<adm::AudioPackFormat> pack2;
  std::shared_ptr<adm::AudioChannelFormat> channel2;
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

  auto secondProgramme = AudioProgramme::create(AudioProgrammeName("second"));
  secondProgramme->set(Start{std::chrono::seconds(10)});
  secondProgramme->set(End{std::chrono::seconds(20)});
  secondProgramme->addReference(content2);

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  channel2->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel2->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(410)}));

  updateBlockFormatDurations(document);

  auto blocks1 = channel1->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks1[0].has<Duration>());
  REQUIRE(blocks1[0].get<Duration>().get() == std::chrono::milliseconds(100));

  REQUIRE(blocks1[1].has<Duration>());
  REQUIRE(blocks1[1].get<Duration>().get() ==
          std::chrono::seconds(20) - std::chrono::milliseconds(100));

  auto blocks2 = channel2->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks2[0].has<Duration>());
  REQUIRE(blocks2[0].get<Duration>().get() == std::chrono::milliseconds(410));

  REQUIRE(blocks2[1].has<Duration>());
  REQUIRE(blocks2[1].get<Duration>().get() ==
          std::chrono::seconds(10) - std::chrono::milliseconds(410));
}

TEST_CASE_METHOD(BaseSceneFixture, "duration_from_programmes_shared") {
  using namespace adm;
  programme->set(Start{std::chrono::seconds(0)});
  programme->set(End{std::chrono::seconds(20)});

  // multiple programmes sharing the same content
  // is fine as long as they have the same duration
  auto secondProgramme = AudioProgramme::create(AudioProgrammeName("second"));
  secondProgramme->set(Start{std::chrono::seconds(10)});
  secondProgramme->set(End{std::chrono::seconds(30)});
  secondProgramme->addReference(content1);

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  updateBlockFormatDurations(document);

  auto blocks1 = channel1->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks1[0].has<Duration>());
  REQUIRE(blocks1[0].get<Duration>().get() == std::chrono::milliseconds(100));

  REQUIRE(blocks1[1].has<Duration>());
  REQUIRE(blocks1[1].get<Duration>().get() ==
          std::chrono::seconds(20) - std::chrono::milliseconds(100));
}

TEST_CASE_METHOD(BaseSceneFixture, "duration_from_programmes_shared_mismatch") {
  using namespace adm;
  programme->set(Start{std::chrono::seconds(0)});
  programme->set(End{std::chrono::seconds(20)});

  // multiple programmes sharing the same content
  // is fine as long as they have the same duration
  auto secondProgramme = AudioProgramme::create(AudioProgrammeName("second"));
  secondProgramme->set(Start{std::chrono::seconds(0)});
  secondProgramme->set(End{std::chrono::seconds(30)});
  secondProgramme->addReference(content1);

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  REQUIRE_THROWS_AS(updateBlockFormatDurations(document), error::AdmException);
}

TEST_CASE_METHOD(BaseSceneFixture, "duration_program_mismatch") {
  using namespace adm;
  programme->set(Start{std::chrono::seconds(0)});
  programme->set(End{std::chrono::seconds(20)});

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  REQUIRE_THROWS_AS(
      updateBlockFormatDurations(document, std::chrono::nanoseconds{10}),
      error::AdmException);
}

TEST_CASE_METHOD(BaseSceneFixture, "duration_from_objects_independent") {
  using namespace adm;
  object1->set(Start{std::chrono::seconds(0)});
  object1->set(Duration{std::chrono::seconds(20)});

  object2->set(Start{std::chrono::seconds(10)});
  object2->set(Duration{std::chrono::seconds(10)});

  programme->addReference(content2);

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  channel2->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel2->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(410)}));

  // file length will not be used as we have object durations
  updateBlockFormatDurations(document, std::chrono::nanoseconds{50});

  auto blocks1 = channel1->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks1[0].has<Duration>());
  REQUIRE(blocks1[0].get<Duration>().get() == std::chrono::milliseconds(100));

  REQUIRE(blocks1[1].has<Duration>());
  REQUIRE(blocks1[1].get<Duration>().get() ==
          std::chrono::seconds(20) - std::chrono::milliseconds(100));

  auto blocks2 = channel2->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks2[0].has<Duration>());
  REQUIRE(blocks2[0].get<Duration>().get() == std::chrono::milliseconds(410));

  REQUIRE(blocks2[1].has<Duration>());
  REQUIRE(blocks2[1].get<Duration>().get() ==
          std::chrono::seconds(10) - std::chrono::milliseconds(410));
}

TEST_CASE_METHOD(BaseSceneFixture, "duration_from_objects_shared") {
  using namespace adm;
  object1->set(Start{std::chrono::seconds(0)});
  object1->set(Duration{std::chrono::seconds(20)});

  // multiple objects sharing the same channelformats
  // are fine as long as they have the same duration
  auto anotherObject = AudioObject::create(AudioObjectName("anotherObject"));
  anotherObject->set(Start{std::chrono::seconds(10)});
  anotherObject->set(Duration{std::chrono::seconds(20)});
  anotherObject->addReference(pack1);
  content1->addReference(anotherObject);

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  // file length will not be used as we have object durations
  updateBlockFormatDurations(document, std::chrono::nanoseconds{50});

  auto blocks1 = channel1->getElements<AudioBlockFormatObjects>();
  REQUIRE(blocks1[0].has<Duration>());
  REQUIRE(blocks1[0].get<Duration>().get() == std::chrono::milliseconds(100));

  REQUIRE(blocks1[1].has<Duration>());
  REQUIRE(blocks1[1].get<Duration>().get() ==
          std::chrono::seconds(20) - std::chrono::milliseconds(100));
}

TEST_CASE_METHOD(BaseSceneFixture, "multiple_object_references_mismatch") {
  using namespace adm;
  object1->set(Start{std::chrono::seconds(0)});
  object1->set(Duration{std::chrono::seconds(20)});

  auto anotherObject = AudioObject::create(AudioObjectName("anotherObject"));
  anotherObject->set(Start{std::chrono::seconds(10)});
  // this object's duration is shorter than object1's duration, but both
  // reference the same pack and therefore blockformats
  anotherObject->set(Duration{std::chrono::seconds(18)});
  anotherObject->addReference(pack1);
  content1->addReference(anotherObject);

  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(0)}));
  channel1->add(AudioBlockFormatObjects(SphericalPosition{},
                                        Rtime{std::chrono::milliseconds(100)}));

  REQUIRE_THROWS_AS(
      updateBlockFormatDurations(document, std::chrono::nanoseconds{50}),
      error::AdmException);
}