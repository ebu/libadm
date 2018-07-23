#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/object_creation.hpp"

#define BOOST_TEST_MODULE CreateAdm
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(basic_document) {
  using namespace adm;

  // create a basic document
  auto admDocument = Document::create();
  auto speechContent = AudioContent::create(AudioContentName("Speech"));
  auto speechContentFr = AudioContent::create(AudioContentName("Speech"),
                                              AudioContentLanguage("fr"));
  auto speechContentEn = AudioContent::create(AudioContentName("Speech"),
                                              AudioContentLanguage("en"));

  BOOST_TEST(speechContentEn->has<AudioContentLanguage>());
  BOOST_TEST(speechContentEn->get<AudioContentLanguage>() == "en");

  auto musicContent = AudioContent::create(AudioContentName("Music"));
  BOOST_TEST(admDocument->add(speechContent));
  BOOST_TEST(admDocument->add(musicContent));
  auto narratorObject = AudioObject::create(AudioObjectName("Narrator"));
  BOOST_TEST(admDocument->add(narratorObject));
  BOOST_TEST(speechContent->addReference(narratorObject));

  auto stereoPack =
      AudioPackFormat::create(AudioPackFormatName("stereo"), TypeDescriptor(0));
  BOOST_TEST(admDocument->add(stereoPack));

  auto admDocument_2 = Document::create();
  BOOST_CHECK_THROW(admDocument_2->add(speechContent), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(musicContent), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(narratorObject), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(stereoPack), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(single_document) {
  using namespace adm;
  // check if Elements can only belong to one document
  auto admDocument = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  auto content = AudioContent::create(AudioContentName("MyContent"));
  auto object = AudioObject::create(AudioObjectName("MyObject"));
  auto packFormat = AudioPackFormat::create(AudioPackFormatName("MyPackFormat"),
                                            TypeDefinition::DIRECT_SPEAKERS);
  auto channelFormat =
      AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                 TypeDefinition::DIRECT_SPEAKERS);
  auto streamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
  auto trackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
  auto trackUid = AudioTrackUid::create();

  admDocument->add(programme);
  admDocument->add(content);
  admDocument->add(object);
  admDocument->add(packFormat);
  admDocument->add(channelFormat);
  admDocument->add(streamFormat);
  admDocument->add(trackFormat);
  admDocument->add(trackUid);

  auto admDocument_2 = Document::create();
  BOOST_CHECK_THROW(admDocument_2->add(programme), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(content), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(object), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(packFormat), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(channelFormat), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(streamFormat), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(trackFormat), std::runtime_error);
  BOOST_CHECK_THROW(admDocument_2->add(trackUid), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(added_recursively) {
  using namespace adm;
  // check if Elements are added recursively

  auto admDocument = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  auto content = AudioContent::create(AudioContentName("MyContent"));
  auto object = AudioObject::create(AudioObjectName("MyObject"));
  auto packFormat = AudioPackFormat::create(AudioPackFormatName("MyPackFormat"),
                                            TypeDefinition::DIRECT_SPEAKERS);
  auto channelFormat =
      AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                 TypeDefinition::DIRECT_SPEAKERS);
  auto streamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
  auto trackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);
  auto trackUid = AudioTrackUid::create();

  programme->addReference(content);
  content->addReference(object);
  object->addReference(packFormat);
  object->addReference(trackUid);
  packFormat->addReference(channelFormat);
  streamFormat->setReference(channelFormat);
  streamFormat->setReference(packFormat);
  streamFormat->addReference(std::weak_ptr<AudioTrackFormat>(trackFormat));
  trackFormat->setReference(streamFormat);
  trackUid->setReference(trackFormat);
  trackUid->setReference(packFormat);

  admDocument->add(programme);

  auto programmes = admDocument->getElements<AudioProgramme>();
  BOOST_TEST(programmes.size() == 1);
  BOOST_TEST(programmes[0] == programme);

  auto contents = admDocument->getElements<AudioContent>();
  BOOST_TEST(contents.size() == 1);
  BOOST_TEST(contents[0] == content);

  auto objects = admDocument->getElements<AudioObject>();
  BOOST_TEST(objects.size() == 1);
  BOOST_TEST(objects[0] == object);

  auto packFormats = admDocument->getElements<AudioPackFormat>();
  BOOST_TEST(packFormats.size() == 1);
  BOOST_TEST(packFormats[0] == packFormat);

  auto channelFormats = admDocument->getElements<AudioChannelFormat>();
  BOOST_TEST(channelFormats.size() == 1);
  BOOST_TEST(channelFormats[0] == channelFormat);

  auto streamFormats = admDocument->getElements<AudioStreamFormat>();
  BOOST_TEST(streamFormats.size() == 1);
  BOOST_TEST(streamFormats[0] == streamFormat);

  auto trackFormats = admDocument->getElements<AudioTrackFormat>();
  BOOST_TEST(trackFormats.size() == 1);
  BOOST_TEST(trackFormats[0] == trackFormat);

  auto trackUids = admDocument->getElements<AudioTrackUid>();
  BOOST_TEST(trackUids.size() == 1);
  BOOST_TEST(trackUids[0] == trackUid);
}

BOOST_AUTO_TEST_CASE(added_recursively_stream_track) {
  // AudioStream / AudioTrackFormat reference each other,
  // so we need to test recursive addtion in both directions
  using namespace adm;
  // check if Elements are added recursively
  {
    auto admDocument = Document::create();

    auto streamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
    auto trackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

    streamFormat->addReference(std::weak_ptr<AudioTrackFormat>(trackFormat));
    trackFormat->setReference(streamFormat);

    admDocument->add(trackFormat);
    auto streamFormats = admDocument->getElements<AudioStreamFormat>();
    BOOST_TEST(streamFormats.size() == 1);
    BOOST_TEST(streamFormats[0] == streamFormat);

    auto trackFormats = admDocument->getElements<AudioTrackFormat>();
    BOOST_TEST(trackFormats.size() == 1);
    BOOST_TEST(trackFormats[0] == trackFormat);
  }
  {
    auto admDocument = Document::create();

    auto streamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
    auto trackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

    streamFormat->addReference(std::weak_ptr<AudioTrackFormat>(trackFormat));
    trackFormat->setReference(streamFormat);

    admDocument->add(streamFormat);

    auto streamFormats = admDocument->getElements<AudioStreamFormat>();
    BOOST_TEST(streamFormats.size() == 1);
    BOOST_TEST(streamFormats[0] == streamFormat);

    auto trackFormats = admDocument->getElements<AudioTrackFormat>();
    BOOST_TEST(trackFormats.size() == 1);
    BOOST_TEST(trackFormats[0] == trackFormat);
  }
}

BOOST_AUTO_TEST_CASE(add_multiple_times) {
  using namespace adm;
  // add elements multiple times
  auto admDocument = Document::create();

  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("AudioProgramme"));
  auto audioContent = AudioContent::create(AudioContentName("AudioContent"));
  auto audioObject = AudioObject::create(AudioObjectName("AduioObject"));
  auto audioPackFormat = AudioPackFormat::create(
      AudioPackFormatName("AudioPack"), TypeDescriptor(0));
  auto audioTrackUid = AudioTrackUid::create();
  auto audioChannelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("AudioChannelFormat"), TypeDefinition::OBJECTS);
  auto audioStreamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("AudioStreamFormat"), FormatDefinition::PCM);
  auto audioTrackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("AudioTrackFormat"), FormatDefinition::PCM);

  admDocument->add(audioProgramme);
  admDocument->add(audioProgramme);
  BOOST_TEST(admDocument->getElements<AudioProgramme>().size() == 1);
  admDocument->add(audioContent);
  admDocument->add(audioContent);
  BOOST_TEST(admDocument->getElements<AudioContent>().size() == 1);
  admDocument->add(audioObject);
  admDocument->add(audioObject);
  BOOST_TEST(admDocument->getElements<AudioObject>().size() == 1);
  admDocument->add(audioPackFormat);
  admDocument->add(audioPackFormat);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>().size() == 1);
  admDocument->add(audioTrackUid);
  admDocument->add(audioTrackUid);
  BOOST_TEST(admDocument->getElements<AudioTrackUid>().size() == 1);
  admDocument->add(audioChannelFormat);
  admDocument->add(audioChannelFormat);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>().size() == 1);
  admDocument->add(audioStreamFormat);
  admDocument->add(audioStreamFormat);
  BOOST_TEST(admDocument->getElements<AudioStreamFormat>().size() == 1);
  admDocument->add(audioTrackFormat);
  admDocument->add(audioTrackFormat);
  BOOST_TEST(admDocument->getElements<AudioTrackFormat>().size() == 1);
}

BOOST_AUTO_TEST_CASE(copy_document_no_duplicates) {
  using namespace adm;

  // create
  auto admDocument = Document::create();
  auto myProgramme1 =
      AudioProgramme::create(AudioProgrammeName("MyProgramme1"));
  auto myProgramme2 =
      AudioProgramme::create(AudioProgrammeName("MyProgramme2"));
  auto myContent = AudioContent::create(AudioContentName("MyContent"));
  // connect
  myProgramme1->addReference(myContent);
  myProgramme2->addReference(myContent);

  // add to document1 and reassign ids
  admDocument->add(myProgramme1);
  admDocument->add(myProgramme2);
  reassignIds(admDocument);

  // copy
  auto copy = deepCopy(admDocument);
  BOOST_TEST(copy->getElements<AudioProgramme>().size() == 2);
  BOOST_TEST(copy->getElements<AudioContent>().size() == 1);
  BOOST_TEST(copy->getElements<AudioProgramme>()[0]
                 ->getReferences<AudioContent>()[0]
                 ->get<AudioContentName>() == "MyContent");
  BOOST_TEST(copy->getElements<AudioProgramme>()[1]
                 ->getReferences<AudioContent>()[0]
                 ->get<AudioContentName>() == "MyContent");

  BOOST_TEST(copy->getElements<AudioProgramme>()[1]
                 ->getReferences<AudioContent>()[0] != myContent);
}

BOOST_AUTO_TEST_CASE(copy_document_all_adm_elements) {
  using namespace adm;

  // create
  auto admDocument = Document::create();
  auto myProgramme = AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  auto myContent = AudioContent::create(AudioContentName("MyContent"));
  auto holder = createSimpleObject("MyObject");
  // connect
  myProgramme->addReference(myContent);
  myContent->addReference(holder.audioObject);

  // add to document1 and reassign ids
  admDocument->add(myProgramme);
  // reassignIds(admDocument);

  BOOST_TEST(admDocument->getElements<AudioProgramme>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioContent>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioObject>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioTrackFormat>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioStreamFormat>().size() == 1);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>().size() == 1);

  auto copy = deepCopy(admDocument);

  BOOST_TEST(copy->getElements<AudioProgramme>().size() == 1);
  BOOST_TEST(copy->getElements<AudioContent>().size() == 1);
  BOOST_TEST(copy->getElements<AudioObject>().size() == 1);
  BOOST_TEST(copy->getElements<AudioPackFormat>().size() == 1);
  BOOST_TEST(copy->getElements<AudioChannelFormat>().size() == 1);
  BOOST_TEST(copy->getElements<AudioTrackFormat>().size() == 1);
  BOOST_TEST(copy->getElements<AudioStreamFormat>().size() == 1);
  BOOST_TEST(copy->getElements<AudioChannelFormat>().size() == 1);

  BOOST_TEST(admDocument->getElements<AudioProgramme>()[0] !=
             copy->getElements<AudioProgramme>()[0]);
  BOOST_TEST(admDocument->getElements<AudioContent>()[0] !=
             copy->getElements<AudioContent>()[0]);
  BOOST_TEST(admDocument->getElements<AudioObject>()[0] !=
             copy->getElements<AudioObject>()[0]);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>()[0] !=
             copy->getElements<AudioPackFormat>()[0]);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>()[0] !=
             copy->getElements<AudioChannelFormat>()[0]);
  BOOST_TEST(admDocument->getElements<AudioTrackFormat>()[0] !=
             copy->getElements<AudioTrackFormat>()[0]);
  BOOST_TEST(admDocument->getElements<AudioStreamFormat>()[0] !=
             copy->getElements<AudioStreamFormat>()[0]);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>()[0] !=
             copy->getElements<AudioChannelFormat>()[0]);
  // disabled until https://gitlab.irt.de/pa/libadm/issues/12 is resolved
  /*
    // copy to document2
    admDocument2->add(myProgramme->copy());
    BOOST_TEST(admDocument2->resolveReferences(admDocument1) == true);
    BOOST_TEST(admDocument2->getElements<AudioProgramme>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioContent>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioObject>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioTrackUid>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioStreamFormat>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioTrackFormat>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioPackFormat>().size() == 1);
    BOOST_TEST(admDocument2->getElements<AudioChannelFormat>().size() == 1);

    // copy document in stages to document3
    auto myPackFormat = admDocument1->getElements<AudioPackFormat>()[0];
    admDocument3->add(myPackFormat->copy());
    admDocument3->add(myProgramme->copy());
    admDocument3->resolveReferences(admDocument1);
    BOOST_TEST(admDocument3->getElements<AudioProgramme>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioContent>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioObject>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioTrackUid>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioStreamFormat>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioTrackFormat>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioPackFormat>().size() == 1);
    BOOST_TEST(admDocument3->getElements<AudioChannelFormat>().size() == 1);
*/
}

template <typename T>
std::vector<T> asVector(std::initializer_list<T> l) {
  return std::vector<T>{l};
}

/**
 * Test if adding an ADM element which does not belong to a document yet cna be
 * added as a reference to an ADM element which does belong to an ADM document.
 * The {add;set}Reference method should automatically add the object to the
 * document.
 */
BOOST_AUTO_TEST_CASE(add_reference_without_parent) {
  using namespace adm;

  auto programme = AudioProgramme::create(AudioProgrammeName("MyProgramme"));
  auto content = AudioContent::create(AudioContentName("MyContent"));
  auto object1 = AudioObject::create(AudioObjectName("MyObject1"));
  auto object2 = AudioObject::create(AudioObjectName("MyObject2"));
  auto trackUid = AudioTrackUid::create();
  auto packFormat1 = AudioPackFormat::create(
      AudioPackFormatName("MyPackFormat1"), TypeDefinition::OBJECTS);
  auto packFormat2 = AudioPackFormat::create(
      AudioPackFormatName("MyPackFormat2"), TypeDefinition::OBJECTS);
  auto packFormat3 = AudioPackFormat::create(
      AudioPackFormatName("MyPackFormat3"), TypeDefinition::OBJECTS);
  auto packFormat4 = AudioPackFormat::create(
      AudioPackFormatName("MyPackFormat4"), TypeDefinition::OBJECTS);
  auto channelFormat1 = AudioChannelFormat::create(
      AudioChannelFormatName("MyChannelFormat1"), TypeDefinition::OBJECTS);
  auto channelFormat2 = AudioChannelFormat::create(
      AudioChannelFormatName("MyChannelFormat2"), TypeDefinition::OBJECTS);
  auto streamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("MyStreamFormat"), FormatDefinition::PCM);
  auto trackFormat1 = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat1"), FormatDefinition::PCM);
  auto trackFormat2 = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat2"), FormatDefinition::PCM);

  auto admDocument = Document::create();
  admDocument->add(programme);
  programme->addReference(content);
  BOOST_TEST(admDocument->getElements<AudioContent>() == asVector({content}));
  content->addReference(object1);
  BOOST_TEST(admDocument->getElements<AudioObject>() == asVector({object1}));
  object1->addReference(trackUid);
  BOOST_TEST(admDocument->getElements<AudioTrackUid>() == asVector({trackUid}));
  object1->addReference(packFormat1);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>() ==
             asVector({packFormat1}));
  object1->addReference(object2);
  BOOST_TEST(admDocument->getElements<AudioObject>() ==
             asVector({object1, object2}));
  trackUid->setReference(trackFormat1);
  BOOST_TEST(admDocument->getElements<AudioTrackFormat>() ==
             asVector({trackFormat1}));
  trackUid->setReference(packFormat2);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>() ==
             asVector({packFormat1, packFormat2}));
  packFormat1->addReference(channelFormat1);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>() ==
             asVector({channelFormat1}));
  packFormat1->addReference(packFormat3);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>() ==
             asVector({packFormat1, packFormat2, packFormat3}));
  trackFormat1->setReference(streamFormat);
  BOOST_TEST(admDocument->getElements<AudioStreamFormat>() ==
             asVector({streamFormat}));
  streamFormat->setReference(packFormat4);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>() ==
             asVector({packFormat1, packFormat2, packFormat3, packFormat4}));
  streamFormat->setReference(channelFormat2);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>() ==
             asVector({channelFormat1, channelFormat2}));
  streamFormat->addReference(std::weak_ptr<AudioTrackFormat>(trackFormat2));
  BOOST_TEST(admDocument->getElements<AudioTrackFormat>() ==
             asVector({trackFormat1, trackFormat2}));
}

BOOST_AUTO_TEST_CASE(remove_elements) {
  using namespace adm;
  // add elements multiple times
  auto admDocument = Document::create();

  auto audioProgramme =
      AudioProgramme::create(AudioProgrammeName("AudioProgramme"));
  auto audioProgramme2 =
      AudioProgramme::create(AudioProgrammeName("AudioProgramme"));
  auto audioContent = AudioContent::create(AudioContentName("AudioContent"));
  auto audioContent2 = AudioContent::create(AudioContentName("AudioContent"));
  auto audioObject = AudioObject::create(AudioObjectName("AduioObject"));
  auto audioObject2 = AudioObject::create(AudioObjectName("AduioObject"));
  auto audioPackFormat = AudioPackFormat::create(
      AudioPackFormatName("AudioPack"), TypeDescriptor(0));
  auto audioPackFormat2 = AudioPackFormat::create(
      AudioPackFormatName("AudioPack"), TypeDescriptor(0));
  auto audioTrackUid = AudioTrackUid::create();
  auto audioTrackUid2 = AudioTrackUid::create();
  auto audioChannelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("AudioChannelFormat"), TypeDefinition::OBJECTS);
  auto audioChannelFormat2 = AudioChannelFormat::create(
      AudioChannelFormatName("AudioChannelFormat"), TypeDefinition::OBJECTS);
  auto audioStreamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("AudioStreamFormat"), FormatDefinition::PCM);
  auto audioStreamFormat2 = AudioStreamFormat::create(
      AudioStreamFormatName("AudioStreamFormat"), FormatDefinition::PCM);
  auto audioTrackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("AudioTrackFormat"), FormatDefinition::PCM);
  auto audioTrackFormat2 = AudioTrackFormat::create(
      AudioTrackFormatName("AudioTrackFormat"), FormatDefinition::PCM);

  admDocument->add(audioProgramme);
  admDocument->add(audioProgramme2);
  BOOST_TEST(admDocument->remove(audioProgramme) == true);
  BOOST_TEST(admDocument->getElements<AudioProgramme>().size() == 1);
  BOOST_TEST(admDocument->remove(audioProgramme) == false);
  admDocument->add(audioContent);
  admDocument->add(audioContent2);
  BOOST_TEST(admDocument->remove(audioContent) == true);
  BOOST_TEST(admDocument->getElements<AudioContent>().size() == 1);
  BOOST_TEST(admDocument->remove(audioContent) == false);
  admDocument->add(audioObject);
  admDocument->add(audioObject2);
  BOOST_TEST(admDocument->remove(audioObject) == true);
  BOOST_TEST(admDocument->getElements<AudioObject>().size() == 1);
  BOOST_TEST(admDocument->remove(audioObject) == false);
  admDocument->add(audioPackFormat);
  admDocument->add(audioPackFormat2);
  BOOST_TEST(admDocument->remove(audioPackFormat) == true);
  BOOST_TEST(admDocument->getElements<AudioPackFormat>().size() == 1);
  BOOST_TEST(admDocument->remove(audioPackFormat) == false);
  admDocument->add(audioTrackUid);
  admDocument->add(audioTrackUid2);
  BOOST_TEST(admDocument->remove(audioTrackUid) == true);
  BOOST_TEST(admDocument->getElements<AudioTrackUid>().size() == 1);
  BOOST_TEST(admDocument->remove(audioTrackUid) == false);
  admDocument->add(audioChannelFormat);
  admDocument->add(audioChannelFormat2);
  BOOST_TEST(admDocument->remove(audioChannelFormat) == true);
  BOOST_TEST(admDocument->getElements<AudioChannelFormat>().size() == 1);
  BOOST_TEST(admDocument->remove(audioChannelFormat) == false);
  admDocument->add(audioStreamFormat);
  admDocument->add(audioStreamFormat2);
  BOOST_TEST(admDocument->remove(audioStreamFormat) == true);
  BOOST_TEST(admDocument->getElements<AudioStreamFormat>().size() == 1);
  BOOST_TEST(admDocument->remove(audioStreamFormat) == false);
  admDocument->add(audioTrackFormat);
  admDocument->add(audioTrackFormat2);
  BOOST_TEST(admDocument->remove(audioTrackFormat) == true);
  BOOST_TEST(admDocument->getElements<AudioTrackFormat>().size() == 1);
  BOOST_TEST(admDocument->remove(audioTrackFormat) == false);
}
