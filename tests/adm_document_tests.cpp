#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/object_creation.hpp"

TEST_CASE("basic_document") {
  using namespace adm;

  // create a basic document
  auto admDocument = Document::create();
  auto speechContent = AudioContent::create(AudioContentName("Speech"));
  auto speechContentFr = AudioContent::create(AudioContentName("Speech"),
                                              AudioContentLanguage("fr"));
  auto speechContentEn = AudioContent::create(AudioContentName("Speech"),
                                              AudioContentLanguage("en"));

  REQUIRE(speechContentEn->has<AudioContentLanguage>());
  REQUIRE(speechContentEn->get<AudioContentLanguage>() == "en");

  auto musicContent = AudioContent::create(AudioContentName("Music"));
  REQUIRE(admDocument->add(speechContent));
  REQUIRE(admDocument->add(musicContent));
  auto narratorObject = AudioObject::create(AudioObjectName("Narrator"));
  REQUIRE(admDocument->add(narratorObject));
  REQUIRE(speechContent->addReference(narratorObject));

  auto stereoPack =
      AudioPackFormat::create(AudioPackFormatName("stereo"), TypeDescriptor(0));
  REQUIRE(admDocument->add(stereoPack));

  auto admDocument_2 = Document::create();
  REQUIRE_THROWS_AS(admDocument_2->add(speechContent), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(musicContent), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(narratorObject), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(stereoPack), std::runtime_error);
}

TEST_CASE("single_document") {
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
  REQUIRE_THROWS_AS(admDocument_2->add(programme), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(content), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(object), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(packFormat), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(channelFormat), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(streamFormat), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(trackFormat), std::runtime_error);
  REQUIRE_THROWS_AS(admDocument_2->add(trackUid), std::runtime_error);
}

TEST_CASE("added_recursively") {
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
  REQUIRE(programmes.size() == 1);
  REQUIRE(programmes[0] == programme);

  auto contents = admDocument->getElements<AudioContent>();
  REQUIRE(contents.size() == 1);
  REQUIRE(contents[0] == content);

  auto objects = admDocument->getElements<AudioObject>();
  REQUIRE(objects.size() == 1);
  REQUIRE(objects[0] == object);

  auto packFormats = admDocument->getElements<AudioPackFormat>();
  REQUIRE(packFormats.size() == 1);
  REQUIRE(packFormats[0] == packFormat);

  auto channelFormats = admDocument->getElements<AudioChannelFormat>();
  REQUIRE(channelFormats.size() == 1);
  REQUIRE(channelFormats[0] == channelFormat);

  auto streamFormats = admDocument->getElements<AudioStreamFormat>();
  REQUIRE(streamFormats.size() == 1);
  REQUIRE(streamFormats[0] == streamFormat);

  auto trackFormats = admDocument->getElements<AudioTrackFormat>();
  REQUIRE(trackFormats.size() == 1);
  REQUIRE(trackFormats[0] == trackFormat);

  auto trackUids = admDocument->getElements<AudioTrackUid>();
  REQUIRE(trackUids.size() == 1);
  REQUIRE(trackUids[0] == trackUid);
}

TEST_CASE("added_recursively_stream_track") {
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
    REQUIRE(streamFormats.size() == 1);
    REQUIRE(streamFormats[0] == streamFormat);

    auto trackFormats = admDocument->getElements<AudioTrackFormat>();
    REQUIRE(trackFormats.size() == 1);
    REQUIRE(trackFormats[0] == trackFormat);
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
    REQUIRE(streamFormats.size() == 1);
    REQUIRE(streamFormats[0] == streamFormat);

    auto trackFormats = admDocument->getElements<AudioTrackFormat>();
    REQUIRE(trackFormats.size() == 1);
    REQUIRE(trackFormats[0] == trackFormat);
  }
}

TEST_CASE("add_multiple_times") {
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
  REQUIRE(admDocument->getElements<AudioProgramme>().size() == 1);
  admDocument->add(audioContent);
  admDocument->add(audioContent);
  REQUIRE(admDocument->getElements<AudioContent>().size() == 1);
  admDocument->add(audioObject);
  admDocument->add(audioObject);
  REQUIRE(admDocument->getElements<AudioObject>().size() == 1);
  admDocument->add(audioPackFormat);
  admDocument->add(audioPackFormat);
  REQUIRE(admDocument->getElements<AudioPackFormat>().size() == 1);
  admDocument->add(audioTrackUid);
  admDocument->add(audioTrackUid);
  REQUIRE(admDocument->getElements<AudioTrackUid>().size() == 1);
  admDocument->add(audioChannelFormat);
  admDocument->add(audioChannelFormat);
  REQUIRE(admDocument->getElements<AudioChannelFormat>().size() == 1);
  admDocument->add(audioStreamFormat);
  admDocument->add(audioStreamFormat);
  REQUIRE(admDocument->getElements<AudioStreamFormat>().size() == 1);
  admDocument->add(audioTrackFormat);
  admDocument->add(audioTrackFormat);
  REQUIRE(admDocument->getElements<AudioTrackFormat>().size() == 1);
}

TEST_CASE("copy_document_no_duplicates") {
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
  REQUIRE(copy->getElements<AudioProgramme>().size() == 2);
  REQUIRE(copy->getElements<AudioContent>().size() == 1);
  REQUIRE(copy->getElements<AudioProgramme>()[0]
              ->getReferences<AudioContent>()[0]
              ->get<AudioContentName>() == "MyContent");
  REQUIRE(copy->getElements<AudioProgramme>()[1]
              ->getReferences<AudioContent>()[0]
              ->get<AudioContentName>() == "MyContent");

  REQUIRE(copy->getElements<AudioProgramme>()[1]
              ->getReferences<AudioContent>()[0] != myContent);
}

TEST_CASE("copy_document_all_adm_elements") {
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

  REQUIRE(admDocument->getElements<AudioProgramme>().size() == 1);
  REQUIRE(admDocument->getElements<AudioContent>().size() == 1);
  REQUIRE(admDocument->getElements<AudioObject>().size() == 1);
  REQUIRE(admDocument->getElements<AudioPackFormat>().size() == 1);
  REQUIRE(admDocument->getElements<AudioChannelFormat>().size() == 1);
  REQUIRE(admDocument->getElements<AudioTrackFormat>().size() == 1);
  REQUIRE(admDocument->getElements<AudioStreamFormat>().size() == 1);
  REQUIRE(admDocument->getElements<AudioChannelFormat>().size() == 1);

  auto copy = deepCopy(admDocument);

  REQUIRE(copy->getElements<AudioProgramme>().size() == 1);
  REQUIRE(copy->getElements<AudioContent>().size() == 1);
  REQUIRE(copy->getElements<AudioObject>().size() == 1);
  REQUIRE(copy->getElements<AudioPackFormat>().size() == 1);
  REQUIRE(copy->getElements<AudioChannelFormat>().size() == 1);
  REQUIRE(copy->getElements<AudioTrackFormat>().size() == 1);
  REQUIRE(copy->getElements<AudioStreamFormat>().size() == 1);
  REQUIRE(copy->getElements<AudioChannelFormat>().size() == 1);

  REQUIRE(admDocument->getElements<AudioProgramme>()[0] !=
          copy->getElements<AudioProgramme>()[0]);
  REQUIRE(admDocument->getElements<AudioContent>()[0] !=
          copy->getElements<AudioContent>()[0]);
  REQUIRE(admDocument->getElements<AudioObject>()[0] !=
          copy->getElements<AudioObject>()[0]);
  REQUIRE(admDocument->getElements<AudioPackFormat>()[0] !=
          copy->getElements<AudioPackFormat>()[0]);
  REQUIRE(admDocument->getElements<AudioChannelFormat>()[0] !=
          copy->getElements<AudioChannelFormat>()[0]);
  REQUIRE(admDocument->getElements<AudioTrackFormat>()[0] !=
          copy->getElements<AudioTrackFormat>()[0]);
  REQUIRE(admDocument->getElements<AudioStreamFormat>()[0] !=
          copy->getElements<AudioStreamFormat>()[0]);
  REQUIRE(admDocument->getElements<AudioChannelFormat>()[0] !=
          copy->getElements<AudioChannelFormat>()[0]);
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
TEST_CASE("add_reference_without_parent") {
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
  REQUIRE(admDocument->getElements<AudioContent>() == asVector({content}));
  content->addReference(object1);
  REQUIRE(admDocument->getElements<AudioObject>() == asVector({object1}));
  object1->addReference(trackUid);
  REQUIRE(admDocument->getElements<AudioTrackUid>() == asVector({trackUid}));
  object1->addReference(packFormat1);
  REQUIRE(admDocument->getElements<AudioPackFormat>() ==
          asVector({packFormat1}));
  object1->addReference(object2);
  REQUIRE(admDocument->getElements<AudioObject>() ==
          asVector({object1, object2}));
  trackUid->setReference(trackFormat1);
  REQUIRE(admDocument->getElements<AudioTrackFormat>() ==
          asVector({trackFormat1}));
  trackUid->setReference(packFormat2);
  REQUIRE(admDocument->getElements<AudioPackFormat>() ==
          asVector({packFormat1, packFormat2}));
  packFormat1->addReference(channelFormat1);
  REQUIRE(admDocument->getElements<AudioChannelFormat>() ==
          asVector({channelFormat1}));
  packFormat1->addReference(packFormat3);
  REQUIRE(admDocument->getElements<AudioPackFormat>() ==
          asVector({packFormat1, packFormat2, packFormat3}));
  trackFormat1->setReference(streamFormat);
  REQUIRE(admDocument->getElements<AudioStreamFormat>() ==
          asVector({streamFormat}));
  streamFormat->setReference(packFormat4);
  REQUIRE(admDocument->getElements<AudioPackFormat>() ==
          asVector({packFormat1, packFormat2, packFormat3, packFormat4}));
  streamFormat->setReference(channelFormat2);
  REQUIRE(admDocument->getElements<AudioChannelFormat>() ==
          asVector({channelFormat1, channelFormat2}));
  streamFormat->addReference(std::weak_ptr<AudioTrackFormat>(trackFormat2));
  REQUIRE(admDocument->getElements<AudioTrackFormat>() ==
          asVector({trackFormat1, trackFormat2}));
}

TEST_CASE("remove_elements") {
  using namespace adm;

  SECTION("basic") {
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
    REQUIRE(admDocument->remove(audioProgramme) == true);
    REQUIRE(admDocument->getElements<AudioProgramme>().size() == 1);
    REQUIRE(admDocument->remove(audioProgramme) == false);
    admDocument->add(audioContent);
    admDocument->add(audioContent2);
    REQUIRE(admDocument->remove(audioContent) == true);
    REQUIRE(admDocument->getElements<AudioContent>().size() == 1);
    REQUIRE(admDocument->remove(audioContent) == false);
    admDocument->add(audioObject);
    admDocument->add(audioObject2);
    REQUIRE(admDocument->remove(audioObject) == true);
    REQUIRE(admDocument->getElements<AudioObject>().size() == 1);
    REQUIRE(admDocument->remove(audioObject) == false);
    admDocument->add(audioPackFormat);
    admDocument->add(audioPackFormat2);
    REQUIRE(admDocument->remove(audioPackFormat) == true);
    REQUIRE(admDocument->getElements<AudioPackFormat>().size() == 1);
    REQUIRE(admDocument->remove(audioPackFormat) == false);
    admDocument->add(audioTrackUid);
    admDocument->add(audioTrackUid2);
    REQUIRE(admDocument->remove(audioTrackUid) == true);
    REQUIRE(admDocument->getElements<AudioTrackUid>().size() == 1);
    REQUIRE(admDocument->remove(audioTrackUid) == false);
    admDocument->add(audioChannelFormat);
    admDocument->add(audioChannelFormat2);
    REQUIRE(admDocument->remove(audioChannelFormat) == true);
    REQUIRE(admDocument->getElements<AudioChannelFormat>().size() == 1);
    REQUIRE(admDocument->remove(audioChannelFormat) == false);
    admDocument->add(audioStreamFormat);
    admDocument->add(audioStreamFormat2);
    REQUIRE(admDocument->remove(audioStreamFormat) == true);
    REQUIRE(admDocument->getElements<AudioStreamFormat>().size() == 1);
    REQUIRE(admDocument->remove(audioStreamFormat) == false);
    admDocument->add(audioTrackFormat);
    admDocument->add(audioTrackFormat2);
    REQUIRE(admDocument->remove(audioTrackFormat) == true);
    REQUIRE(admDocument->getElements<AudioTrackFormat>().size() == 1);
    REQUIRE(admDocument->remove(audioTrackFormat) == false);
  }

  SECTION("AudioContent – reference removal") {
    auto admDocument = Document::create();
    auto programme = AudioProgramme::create(AudioProgrammeName("My Programme"));
    auto content = AudioContent::create(AudioContentName("My Content"));

    programme->addReference(content);
    admDocument->add(programme);

    REQUIRE(programme->getReferences<AudioContent>().size() == 1);
    REQUIRE(admDocument->remove(content));
    REQUIRE(programme->getReferences<AudioContent>().size() == 0);
  }

  SECTION("AudioObject – reference removal") {
    auto admDocument = Document::create();
    auto content = AudioContent::create(AudioContentName("My Content"));
    auto object = AudioObject::create(AudioObjectName("My Object"));

    content->addReference(object);
    admDocument->add(content);

    REQUIRE(content->getReferences<AudioObject>().size() == 1);
    REQUIRE(admDocument->remove(object));
    REQUIRE(content->getReferences<AudioObject>().size() == 0);
  }

  SECTION("AudioTrackUID – reference removal") {
    auto admDocument = Document::create();
    auto object = AudioObject::create(AudioObjectName("My Object"));
    auto trackUid = AudioTrackUid::create();

    object->addReference(trackUid);
    admDocument->add(object);

    REQUIRE(object->getReferences<AudioTrackUid>().size() == 1);
    REQUIRE(admDocument->remove(trackUid));
    REQUIRE(object->getReferences<AudioTrackUid>().size() == 0);
  }

  SECTION("AudioPackFormat – reference removal") {
    auto admDocument = Document::create();
    auto packFormat1 = AudioPackFormat::create(
        AudioPackFormatName("My PackFormat"), TypeDefinition::OBJECTS);
    auto packFormat2 = AudioPackFormat::create(
        AudioPackFormatName("My PackFormat"), TypeDefinition::OBJECTS);

    auto trackUID = AudioTrackUid::create();
    auto object = AudioObject::create(AudioObjectName("My Object"));
    auto streamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("My StreamFormat"), FormatDefinition::PCM);

    packFormat1->addReference(packFormat2);
    trackUID->setReference(packFormat2);
    object->addReference(packFormat2);
    streamFormat->setReference(packFormat2);

    admDocument->add(packFormat1);
    admDocument->add(trackUID);
    admDocument->add(object);
    admDocument->add(streamFormat);

    REQUIRE(packFormat1->getReferences<AudioPackFormat>().size() == 1);
    REQUIRE(object->getReferences<AudioPackFormat>().size() == 1);
    REQUIRE(trackUID->getReference<AudioPackFormat>() != nullptr);
    REQUIRE(streamFormat->getReference<AudioPackFormat>() != nullptr);
    REQUIRE(admDocument->remove(packFormat2));
    REQUIRE(packFormat1->getReferences<AudioPackFormat>().size() == 0);
    REQUIRE(object->getReferences<AudioPackFormat>().size() == 0);
    REQUIRE(trackUID->getReference<AudioPackFormat>() == nullptr);
    REQUIRE(streamFormat->getReference<AudioPackFormat>() == nullptr);
  }

  SECTION("AudioChannelFormat – reference removal") {
    auto admDocument = Document::create();
    auto packFormat = AudioPackFormat::create(
        AudioPackFormatName("My PackFormat"), TypeDefinition::OBJECTS);
    auto channelFormat = AudioChannelFormat::create(
        AudioChannelFormatName("My ChannelFormat"), TypeDefinition::OBJECTS);

    packFormat->addReference(channelFormat);
    admDocument->add(packFormat);

    REQUIRE(packFormat->getReferences<AudioChannelFormat>().size() == 1);
    REQUIRE(admDocument->remove(channelFormat));
    REQUIRE(packFormat->getReferences<AudioChannelFormat>().size() == 0);
  }

  SECTION("AudioStreamFormat – reference removal") {
    auto admDocument = Document::create();
    auto streamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("My StreamFormat"), FormatDefinition::PCM);
    auto trackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("My TrackFormat"), FormatDefinition::PCM);

    trackFormat->setReference(streamFormat);
    admDocument->add(trackFormat);

    REQUIRE(trackFormat->getReference<AudioStreamFormat>() != nullptr);
    REQUIRE(admDocument->remove(streamFormat));
    REQUIRE(trackFormat->getReference<AudioStreamFormat>() == nullptr);
  }

  SECTION("AudioTrackFormat – reference removal") {
    auto admDocument = Document::create();
    auto trackFormat = AudioTrackFormat::create(
        AudioTrackFormatName("My TrackFormat"), FormatDefinition::PCM);
    auto streamFormat = AudioStreamFormat::create(
        AudioStreamFormatName("My StreamFormat"), FormatDefinition::PCM);
    auto trackUid = AudioTrackUid::create();

    streamFormat->addReference(trackFormat);
    trackUid->setReference(trackFormat);
    admDocument->add(streamFormat);
    admDocument->add(trackUid);

    REQUIRE(streamFormat->getAudioTrackFormatReferences().size() == 1);
    REQUIRE(trackUid->getReference<AudioTrackFormat>() != nullptr);
    REQUIRE(admDocument->remove(trackFormat));
    REQUIRE(streamFormat->getAudioTrackFormatReferences().size() == 0);
    REQUIRE(trackUid->getReference<AudioTrackFormat>() == nullptr);
  }
}
