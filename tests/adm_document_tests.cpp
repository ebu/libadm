#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"

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

template <typename El>
bool equalIds(std::shared_ptr<El> const& lhs, std::shared_ptr<El> const& rhs) {
  using id_t = typename El::id_type;
  auto lhsId = lhs->template get<id_t>();
  auto rhsId = rhs->template get<id_t>();
  return lhsId == rhsId;
};

using namespace adm;
template <typename El>
std::shared_ptr<El const> getFirst(Document const& doc) {
  auto elements = doc.getElements<El>();
  return elements.front();
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
  reassignIds(admDocument);

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

  REQUIRE(equalIds(getFirst<AudioProgramme>(*admDocument),
                   getFirst<AudioProgramme>(*copy)));
  REQUIRE(equalIds(getFirst<AudioContent>(*admDocument),
                   getFirst<AudioContent>(*copy)));
  REQUIRE(equalIds(getFirst<AudioObject>(*admDocument),
                   getFirst<AudioObject>(*copy)));
  REQUIRE(equalIds(getFirst<AudioPackFormat>(*admDocument),
                   getFirst<AudioPackFormat>(*copy)));
  REQUIRE(equalIds(getFirst<AudioChannelFormat>(*admDocument),
                   getFirst<AudioChannelFormat>(*copy)));
  REQUIRE(equalIds(getFirst<AudioTrackFormat>(*admDocument),
                   getFirst<AudioTrackFormat>(*copy)));
  REQUIRE(equalIds(getFirst<AudioStreamFormat>(*admDocument),
                   getFirst<AudioStreamFormat>(*copy)));
  REQUIRE(equalIds(getFirst<AudioTrackUid>(*admDocument),
                   getFirst<AudioTrackUid>(*copy)));

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

TEST_CASE("copy_document_remaps_2076_3_renderer_references") {
  using namespace adm;

  auto admDocument = Document::create();

  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  auto content = AudioContent::create(AudioContentName("Content"));
  auto object = AudioObject::create(AudioObjectName("Object"));
  auto packA = AudioPackFormat::create(AudioPackFormatName("PackA"),
                                       TypeDefinition::OBJECTS);
  auto packB = AudioPackFormat::create(AudioPackFormatName("PackB"),
                                       TypeDefinition::OBJECTS);

  object->addReference(packA);
  object->addReference(packB);
  content->addReference(object);
  programme->addReference(content);
  admDocument->add(programme);

  auto packAId = packA->get<AudioPackFormatId>();
  auto packBId = packB->get<AudioPackFormatId>();
  auto objectId = object->get<AudioObjectId>();

  AuthoringInformation info;
  AuthoringRenderer authoringRenderer{
      RendererUri{"urn:itu:bs:2127:0:itu_adm_renderer"}};
  authoringRenderer.addReference(packA);
  authoringRenderer.addReference(packB);
  info.add(authoringRenderer);
  info.add(ReferenceLayout{packA});
  info.add(ReferenceLayout{packB});
  programme->set(info);

  LoudnessRenderer programmeRenderer;
  programmeRenderer.set(RendererPackFormatIdRef{packA});
  programmeRenderer.addReference(object);
  LoudnessMetadata programmeLm;
  programmeLm.set(programmeRenderer);
  programme->set(LoudnessMetadatas{programmeLm});

  LoudnessRenderer contentRenderer;
  contentRenderer.set(RendererPackFormatIdRef{packA});
  contentRenderer.addReference(object);
  LoudnessMetadata contentLm;
  contentLm.set(contentRenderer);
  content->set(LoudnessMetadatas{contentLm});

  auto copy = admDocument->deepCopy();

  auto copiedProgramme = copy->getElements<AudioProgramme>().front();
  auto copiedContent = copy->getElements<AudioContent>().front();

  auto copiedInfo = copiedProgramme->get<AuthoringInformation>();
  REQUIRE(copiedInfo.has<Renderers>());
  auto copiedRenderers = copiedInfo.get<Renderers>();
  REQUIRE(copiedRenderers.size() == 1);
  auto copiedAuthoringRefs =
      copiedRenderers.at(0).getReferences<AudioPackFormat>();
  REQUIRE(copiedAuthoringRefs.size() == 2);
  bool sawPackAInAuthoring = false;
  bool sawPackBInAuthoring = false;
  for (auto const& ref : copiedAuthoringRefs) {
    REQUIRE(ref->getParent().lock() == copy);
    REQUIRE(ref != packA);
    REQUIRE(ref != packB);
    if (ref->get<AudioPackFormatId>() == packAId) sawPackAInAuthoring = true;
    if (ref->get<AudioPackFormatId>() == packBId) sawPackBInAuthoring = true;
  }
  REQUIRE(sawPackAInAuthoring);
  REQUIRE(sawPackBInAuthoring);

  REQUIRE(copiedInfo.has<ReferenceLayouts>());
  auto copiedLayouts = copiedInfo.get<ReferenceLayouts>();
  REQUIRE(copiedLayouts.size() == 2);
  bool sawPackAInLayouts = false;
  bool sawPackBInLayouts = false;
  for (auto const& layout : copiedLayouts) {
    auto const& pack = layout.get();
    REQUIRE(pack->getParent().lock() == copy);
    REQUIRE(pack != packA);
    REQUIRE(pack != packB);
    if (pack->get<AudioPackFormatId>() == packAId) sawPackAInLayouts = true;
    if (pack->get<AudioPackFormatId>() == packBId) sawPackBInLayouts = true;
  }
  REQUIRE(sawPackAInLayouts);
  REQUIRE(sawPackBInLayouts);

  auto copiedProgrammeLms = copiedProgramme->get<LoudnessMetadatas>();
  REQUIRE(copiedProgrammeLms.size() == 1);
  auto copiedProgrammeRenderer =
      copiedProgrammeLms.at(0).get<LoudnessRenderer>();
  REQUIRE(copiedProgrammeRenderer.has<RendererPackFormatIdRef>());
  REQUIRE(copiedProgrammeRenderer.getReferences<AudioObject>().size() == 1);

  auto copiedProgrammePackRef =
      copiedProgrammeRenderer.get<RendererPackFormatIdRef>();
  REQUIRE(copiedProgrammePackRef->getParent().lock() == copy);
  REQUIRE(copiedProgrammePackRef != packA);
  REQUIRE(copiedProgrammePackRef != packB);
  REQUIRE(copiedProgrammePackRef->get<AudioPackFormatId>() == packAId);

  auto copiedProgrammeObjectRefs =
      copiedProgrammeRenderer.getReferences<AudioObject>();
  auto copiedProgrammeObject = copiedProgrammeObjectRefs.front();
  REQUIRE(copiedProgrammeObject->getParent().lock() == copy);
  REQUIRE(copiedProgrammeObject != object);
  REQUIRE(copiedProgrammeObject->get<AudioObjectId>() == objectId);

  auto copiedContentLms = copiedContent->get<LoudnessMetadatas>();
  REQUIRE(copiedContentLms.size() == 1);
  auto copiedContentRenderer = copiedContentLms.at(0).get<LoudnessRenderer>();
  REQUIRE(copiedContentRenderer.has<RendererPackFormatIdRef>());
  REQUIRE(copiedContentRenderer.getReferences<AudioObject>().size() == 1);

  auto copiedContentPackRef =
      copiedContentRenderer.get<RendererPackFormatIdRef>();
  REQUIRE(copiedContentPackRef->getParent().lock() == copy);
  REQUIRE(copiedContentPackRef != packA);
  REQUIRE(copiedContentPackRef != packB);
  REQUIRE(copiedContentPackRef->get<AudioPackFormatId>() == packAId);

  auto copiedContentObjectRefs =
      copiedContentRenderer.getReferences<AudioObject>();
  auto copiedContentObject = copiedContentObjectRefs.front();
  REQUIRE(copiedContentObject->getParent().lock() == copy);
  REQUIRE(copiedContentObject != object);
  REQUIRE(copiedContentObject->get<AudioObjectId>() == objectId);
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

  SECTION("AudioPackFormat – prunes authoring and loudness renderer IDs") {
    auto admDocument = Document::create();

    auto packFormat = AudioPackFormat::create(
        AudioPackFormatName("My PackFormat"), TypeDefinition::OBJECTS);
    admDocument->add(packFormat);

    auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
    auto content = AudioContent::create(AudioContentName("Content"));

    // authoringInformation.renderer + referenceLayout
    AuthoringInformation info;
    AuthoringRenderer authoringRenderer{
        RendererUri{"urn:itu:bs:2127:0:itu_adm_renderer"}};
    authoringRenderer.addReference(packFormat);
    info.add(authoringRenderer);
    info.add(ReferenceLayout{packFormat});
    programme->set(info);

    // loudnessMetadata.renderer on programme
    LoudnessRenderer programmeRenderer;
    programmeRenderer.set(RendererPackFormatIdRef{packFormat});
    LoudnessMetadata programmeLm;
    programmeLm.set(programmeRenderer);
    programme->set(LoudnessMetadatas{programmeLm});

    // loudnessMetadata.renderer on content
    LoudnessRenderer contentRenderer;
    contentRenderer.set(RendererPackFormatIdRef{packFormat});
    LoudnessMetadata contentLm;
    contentLm.set(contentRenderer);
    content->set(LoudnessMetadatas{contentLm});

    admDocument->add(programme);
    admDocument->add(content);

    REQUIRE(admDocument->remove(packFormat));

    auto updatedInfo = programme->get<AuthoringInformation>();
    REQUIRE(updatedInfo.has<Renderers>());
    auto renderers = updatedInfo.get<Renderers>();
    REQUIRE(renderers.size() == 1);
    REQUIRE(renderers.at(0).getReferences<AudioPackFormat>().empty());
    REQUIRE(updatedInfo.has<ReferenceLayouts>() == false);

    auto programmeLms = programme->get<LoudnessMetadatas>();
    REQUIRE(programmeLms.size() == 1);
    REQUIRE(programmeLms.at(0).has<LoudnessRenderer>());
    auto updatedProgrammeRenderer = programmeLms.at(0).get<LoudnessRenderer>();
    REQUIRE(updatedProgrammeRenderer.has<RendererPackFormatIdRef>() == false);

    auto contentLms = content->get<LoudnessMetadatas>();
    REQUIRE(contentLms.size() == 1);
    REQUIRE(contentLms.at(0).has<LoudnessRenderer>());
    auto updatedContentRenderer = contentLms.at(0).get<LoudnessRenderer>();
    REQUIRE(updatedContentRenderer.has<RendererPackFormatIdRef>() == false);
  }

  SECTION(
      "AudioPackFormat – prunes matching renderer IDs and preserves others") {
    auto admDocument = Document::create();

    auto removedPackFormat = AudioPackFormat::create(
        AudioPackFormatName("Removed PackFormat"), TypeDefinition::OBJECTS);
    auto keptPackFormat = AudioPackFormat::create(
        AudioPackFormatName("Kept PackFormat"), TypeDefinition::OBJECTS);
    admDocument->add(removedPackFormat);
    admDocument->add(keptPackFormat);

    auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
    auto content = AudioContent::create(AudioContentName("Content"));

    AuthoringInformation info;
    AuthoringRenderer authoringRenderer{
        RendererUri{"urn:itu:bs:2127:0:itu_adm_renderer"}};
    authoringRenderer.addReference(removedPackFormat);
    authoringRenderer.addReference(keptPackFormat);
    info.add(authoringRenderer);
    info.add(ReferenceLayout{removedPackFormat});
    info.add(ReferenceLayout{keptPackFormat});
    programme->set(info);

    LoudnessRenderer programmeRenderer;
    programmeRenderer.set(RendererPackFormatIdRef{removedPackFormat});
    LoudnessMetadata programmeLm;
    programmeLm.set(programmeRenderer);
    programme->set(LoudnessMetadatas{programmeLm});

    LoudnessRenderer contentRenderer;
    contentRenderer.set(RendererPackFormatIdRef{removedPackFormat});
    LoudnessMetadata contentLm;
    contentLm.set(contentRenderer);
    content->set(LoudnessMetadatas{contentLm});

    admDocument->add(programme);
    admDocument->add(content);

    REQUIRE(admDocument->remove(removedPackFormat));

    auto updatedInfo = programme->get<AuthoringInformation>();
    REQUIRE(updatedInfo.has<Renderers>());
    auto renderers = updatedInfo.get<Renderers>();
    REQUIRE(renderers.size() == 1);
    auto authoringRefs = renderers.at(0).getReferences<AudioPackFormat>();
    REQUIRE(authoringRefs.size() == 1);
    REQUIRE(authoringRefs[0] == keptPackFormat);

    REQUIRE(updatedInfo.has<ReferenceLayouts>());
    auto layouts = updatedInfo.get<ReferenceLayouts>();
    REQUIRE(layouts.size() == 1);
    auto remainingLayout = layouts.at(0).get();
    REQUIRE(remainingLayout == keptPackFormat);

    auto programmeLms = programme->get<LoudnessMetadatas>();
    REQUIRE(programmeLms.size() == 1);
    auto updatedProgrammeRenderer = programmeLms.at(0).get<LoudnessRenderer>();
    REQUIRE(updatedProgrammeRenderer.has<RendererPackFormatIdRef>() == false);

    auto contentLms = content->get<LoudnessMetadatas>();
    REQUIRE(contentLms.size() == 1);
    auto updatedContentRenderer = contentLms.at(0).get<LoudnessRenderer>();
    REQUIRE(updatedContentRenderer.has<RendererPackFormatIdRef>() == false);
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

  SECTION("AudioObject – prunes loudness renderer object IDs") {
    auto admDocument = Document::create();

    auto object = AudioObject::create(AudioObjectName("Object"));
    admDocument->add(object);

    auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
    auto content = AudioContent::create(AudioContentName("Content"));

    LoudnessRenderer programmeRenderer;
    programmeRenderer.addReference(object);
    LoudnessMetadata programmeLm;
    programmeLm.set(programmeRenderer);
    programme->set(LoudnessMetadatas{programmeLm});

    LoudnessRenderer contentRenderer;
    contentRenderer.addReference(object);
    LoudnessMetadata contentLm;
    contentLm.set(contentRenderer);
    content->set(LoudnessMetadatas{contentLm});

    admDocument->add(programme);
    admDocument->add(content);

    REQUIRE(admDocument->remove(object));

    auto programmeLms = programme->get<LoudnessMetadatas>();
    REQUIRE(programmeLms.size() == 1);
    auto updatedProgrammeRenderer = programmeLms.at(0).get<LoudnessRenderer>();
    REQUIRE(updatedProgrammeRenderer.getReferences<AudioObject>().empty());

    auto contentLms = content->get<LoudnessMetadatas>();
    REQUIRE(contentLms.size() == 1);
    auto updatedContentRenderer = contentLms.at(0).get<LoudnessRenderer>();
    REQUIRE(updatedContentRenderer.getReferences<AudioObject>().empty());
  }

  SECTION(
      "AudioObject – prunes matching renderer object IDs and preserves "
      "others") {
    auto admDocument = Document::create();

    auto removedObject = AudioObject::create(AudioObjectName("Removed Object"));
    auto keptObject = AudioObject::create(AudioObjectName("Kept Object"));
    admDocument->add(removedObject);
    admDocument->add(keptObject);

    auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
    auto content = AudioContent::create(AudioContentName("Content"));

    LoudnessRenderer programmeRenderer;
    programmeRenderer.addReference(removedObject);
    programmeRenderer.addReference(keptObject);
    LoudnessMetadata programmeLm;
    programmeLm.set(programmeRenderer);
    programme->set(LoudnessMetadatas{programmeLm});

    LoudnessRenderer contentRenderer;
    contentRenderer.addReference(removedObject);
    contentRenderer.addReference(keptObject);
    LoudnessMetadata contentLm;
    contentLm.set(contentRenderer);
    content->set(LoudnessMetadatas{contentLm});

    admDocument->add(programme);
    admDocument->add(content);

    REQUIRE(admDocument->remove(removedObject));

    auto programmeLms = programme->get<LoudnessMetadatas>();
    REQUIRE(programmeLms.size() == 1);
    auto updatedProgrammeRenderer = programmeLms.at(0).get<LoudnessRenderer>();
    auto programmeRefs = updatedProgrammeRenderer.getReferences<AudioObject>();
    REQUIRE(programmeRefs.size() == 1);
    REQUIRE(programmeRefs[0] == keptObject);

    auto contentLms = content->get<LoudnessMetadatas>();
    REQUIRE(contentLms.size() == 1);
    auto updatedContentRenderer = contentLms.at(0).get<LoudnessRenderer>();
    auto contentRefs = updatedContentRenderer.getReferences<AudioObject>();
    REQUIRE(contentRefs.size() == 1);
    REQUIRE(contentRefs[0] == keptObject);
  }
}

// Tests deepcopy using a modified version of the kitchen sink test material from https://qc.ebu.io/testmaterial
TEST_CASE("Copy the kitchen sink") {
  auto document = parseXml("sink.xml");
  std::stringstream xml;
  writeXml(xml, document);
  auto documentCopy = document->deepCopy();
  std::stringstream xmlCopy;
  writeXml(xmlCopy, documentCopy);
  REQUIRE(xml.str() == xmlCopy.str());
}

TEST_CASE("recursion_depth_guard") {
  // Adversarial input: a chain of 5000 audioPackFormats, each referencing
  // the next. Document::add() recurses through getReferences(), which
  // would overflow the stack without an explicit depth limit.
  using namespace adm;
  auto document = Document::create();
  std::vector<std::shared_ptr<AudioPackFormat>> packs;
  packs.reserve(5000);
  for (int i = 0; i < 5000; ++i) {
    packs.push_back(AudioPackFormat::create(AudioPackFormatName("p"),
                                            TypeDefinition::OBJECTS));
  }
  for (size_t i = 0; i + 1 < packs.size(); ++i) {
    packs[i]->addReference(packs[i + 1]);
  }
  // Adding the head element should now throw (depth-limit) rather than
  // crash; the limit is well below 5000.
  REQUIRE_THROWS_WITH(document->add(packs.front()),
                      Catch::Contains("recursion depth exceeded"));
}
