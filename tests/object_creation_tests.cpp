#include <catch2/catch.hpp>
#include "adm/utilities/object_creation.hpp"

TEST_CASE("add_in_loop") {
  using namespace adm;

  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto content = AudioContent::create(AudioContentName("Content"));
  programme->addReference(content);
  for (int i = 0; i < 10; ++i) {
    auto holder = createSimpleObject("Object");
    content->addReference(holder.audioObject);
  }
  auto document = Document::create();
  document->add(programme);
  REQUIRE(document->getElements<AudioStreamFormat>().size() == 10);
}

TEST_CASE("add_object_loop") {
  using namespace adm;

  auto object = AudioObject::create(AudioObjectName("Object"));
  for (int i = 0; i < 10; ++i) {
    auto trackUid = AudioTrackUid::create();
    auto trackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("TrackFormat"), FormatDefinition::PCM);
    auto streamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("StreamFormat"), FormatDefinition::PCM);
    object->addReference(trackUid);
    trackUid->setReference(trackFormat);
    streamFormat->addReference(std::weak_ptr<AudioTrackFormat>(trackFormat));
    trackFormat->setReference(streamFormat);
  }
  auto document = Document::create();
  document->add(object);
  REQUIRE(document->getElements<AudioStreamFormat>().size() == 10);
}
