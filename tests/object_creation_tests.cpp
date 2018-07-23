#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/utilities/object_creation.hpp"

#define BOOST_TEST_MODULE ObjectCreation
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(add_in_loop) {
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
  BOOST_TEST(document->getElements<AudioStreamFormat>().size() == 10);
}

BOOST_AUTO_TEST_CASE(add_object_loop) {
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
  BOOST_TEST(document->getElements<AudioStreamFormat>().size() == 10);
}
