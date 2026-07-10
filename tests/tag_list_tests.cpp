#include <catch2/catch.hpp>
#include "helper/parameter_checks.hpp"
#include "adm/document.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"
#include "adm/elements/tag_list.hpp"
#include "adm/utilities/object_creation.hpp"

#include <iostream>

using namespace adm;
using namespace adm_test;

TEST_CASE("Tag parameters") {
  Tag tag{TagValue("value")};

  check_optional_param<TagClass>(tag, canBeSetTo(TagClass{"class2"}));
  check_required_param<TagValue>(tag, hasDefaultOf(TagValue{"value"}),
                                 canBeSetTo(TagValue{"value2"}));
}

TEST_CASE("TagGroup parameters") {
  auto programme = AudioProgramme::create(AudioProgrammeName{"Test"});
  TagGroup tagGroup{programme};
  Tag tag{TagClass("class"), TagValue("value")};

  check_vector_param<Tags>(tagGroup, canBeSetTo(Tags{tag}));
}

TEST_CASE("TagList parameters") {
  auto programme = AudioProgramme::create(AudioProgrammeName{"Test"});
  Tag tag{TagClass("class"), TagValue("value")};
  TagGroup tagGroup{programme};
  tagGroup.add(tag);
  TagList tagList;

  check_vector_param<TagGroups>(tagList, canBeSetTo(TagGroups{tagGroup}));
}

TEST_CASE("adm xml/taglist") {
  auto doc = parseXml("tag_list.accepted.xml");

  REQUIRE(doc->has<TagList>());
  auto tag_list = doc->get<TagList>();
  auto tag_groups = tag_list.get<TagGroups>();
  REQUIRE(tag_groups.size() == 1);

  auto tag_group = tag_groups.at(0);
  auto tags = tag_group.get<Tags>();
  auto tag = tags.at(0);
  CHECK(tag.get<TagClass>() == "class1");
  CHECK(tag.get<TagValue>() == "value1");

  std::stringstream xml;
  writeXml(xml, doc);
  CHECK_THAT(xml.str(), EqualsXmlFile("tag_list"));
}

TEST_CASE("document copy updates tagList references") {
  auto doc = Document::create();
  auto holder = addSimpleObjectTo(doc, "Test");
  Tag tag{TagClass("class"), TagValue("value")};
  auto programme = AudioProgramme::create(AudioProgrammeName{"Test"});
  TagGroup tagGroup{programme};
  tagGroup.add(tag);
  tagGroup.addReference(holder.audioObject);
  auto tagList = TagList{};
  tagList.add(tagGroup);
  doc->set(tagList);

  auto doc_copy = doc->deepCopy();
  auto copied_object = doc_copy->getElements<AudioObject>().front();
  REQUIRE(doc_copy->has<TagList>());
  auto copied_tag_groups = doc_copy->get<TagList>().get<TagGroups>();
  REQUIRE(!copied_tag_groups.empty());
  auto copied_tagged_object_refs =
      copied_tag_groups.front().getReferences<AudioObject>();
  REQUIRE(!copied_tagged_object_refs.empty());
  auto tagged_object_ref = copied_tagged_object_refs.front();
  REQUIRE(copied_object == tagged_object_ref);
}

TEST_CASE("document copy remaps tagList programme/content/object references") {
  auto doc = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName{"Programme"});
  auto content = AudioContent::create(AudioContentName{"Content"});
  auto object = AudioObject::create(AudioObjectName{"Object"});
  doc->add(programme);
  doc->add(content);
  doc->add(object);

  TagGroup tagGroup{programme};
  tagGroup.addReference(content);
  tagGroup.addReference(object);
  tagGroup.add(Tag{TagClass("class"), TagValue("value")});
  TagList tagList{};
  tagList.add(tagGroup);
  doc->set(tagList);

  auto docCopy = doc->deepCopy();
  REQUIRE(docCopy->has<TagList>());

  auto copiedProgramme = docCopy->getElements<AudioProgramme>().front();
  auto copiedContent = docCopy->getElements<AudioContent>().front();
  auto copiedObject = docCopy->getElements<AudioObject>().front();

  auto copiedGroups = docCopy->get<TagList>().get<TagGroups>();
  REQUIRE(copiedGroups.size() == 1);

  auto copiedProgrammeRefs =
      copiedGroups.front().getReferences<AudioProgramme>();
  REQUIRE(copiedProgrammeRefs.size() == 1);
  REQUIRE(copiedProgrammeRefs.front() == copiedProgramme);
  REQUIRE(copiedProgrammeRefs.front() != programme);

  auto copiedContentRefs = copiedGroups.front().getReferences<AudioContent>();
  REQUIRE(copiedContentRefs.size() == 1);
  REQUIRE(copiedContentRefs.front() == copiedContent);
  REQUIRE(copiedContentRefs.front() != content);

  auto copiedObjectRefs = copiedGroups.front().getReferences<AudioObject>();
  REQUIRE(copiedObjectRefs.size() == 1);
  REQUIRE(copiedObjectRefs.front() == copiedObject);
  REQUIRE(copiedObjectRefs.front() != object);
}

TEST_CASE(
    "removing last referenced element from document removes TagGroup and "
    "transitively TagList") {
  auto doc = Document::create();
  auto holder = addSimpleObjectTo(doc, "Test");
  Tag tag{TagClass("class"), TagValue("value")};
  auto programme = AudioProgramme::create(AudioProgrammeName{"Test"});
  TagGroup tagGroup{programme};
  tagGroup.add(tag);
  tagGroup.addReference(holder.audioObject);
  auto tagList = TagList{};
  tagList.add(tagGroup);
  doc->set(tagList);
  REQUIRE(doc->has<TagList>());
  REQUIRE(doc->get<TagList>().get<TagGroups>().size() == 1);
  doc->remove(holder.audioObject);
  REQUIRE(!doc->has<TagList>());
}

TEST_CASE(
    "removing referenced AudioProgramme from document removes TagGroup and "
    "transitively TagList") {
  auto doc = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName{"Programme"});
  auto content = AudioContent::create(AudioContentName{"Content"});
  doc->add(programme);
  doc->add(content);

  TagGroup tagGroup{programme};
  tagGroup.add(Tag{TagClass("class"), TagValue("value")});
  tagGroup.addReference(content);
  TagList tagList{};
  tagList.add(tagGroup);
  doc->set(tagList);

  REQUIRE(doc->has<TagList>());
  REQUIRE(doc->get<TagList>().get<TagGroups>().size() == 1);
  doc->remove(programme);
  REQUIRE(!doc->has<TagList>());
}

TEST_CASE(
    "removing referenced AudioContent from document removes TagGroup and "
    "transitively TagList") {
  auto doc = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName{"Programme"});
  auto content = AudioContent::create(AudioContentName{"Content"});
  doc->add(programme);
  doc->add(content);

  TagGroup tagGroup{programme};
  tagGroup.add(Tag{TagClass("class"), TagValue("value")});
  tagGroup.addReference(content);
  TagList tagList{};
  tagList.add(tagGroup);
  doc->set(tagList);

  REQUIRE(doc->has<TagList>());
  REQUIRE(doc->get<TagList>().get<TagGroups>().size() == 1);
  doc->remove(content);
  REQUIRE(!doc->has<TagList>());
}

TEST_CASE(
    "TagList referencing elements owned by another document is rejected") {
  auto otherDoc = Document::create();
  auto holder = addSimpleObjectTo(otherDoc, "Other");

  TagGroup tagGroup{holder.audioObject};
  tagGroup.add(Tag{TagValue("v")});
  TagList tagList;
  tagList.add(tagGroup);

  auto doc = Document::create();
  REQUIRE(doc->set(tagList) == false);
  REQUIRE(!doc->has<TagList>());
}

TEST_CASE(
    "TagList referencing unparented elements adopts them into the document") {
  auto doc = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName{"P"});
  auto content = AudioContent::create(AudioContentName{"C"});
  auto object = AudioObject::create(AudioObjectName{"O"});

  TagGroup tagGroup{programme};
  tagGroup.addReference(content);
  tagGroup.addReference(object);
  tagGroup.add(Tag{TagValue("v")});
  TagList tagList;
  tagList.add(tagGroup);

  doc->set(tagList);
  REQUIRE(doc->has<TagList>());
  // The previously-unparented elements should now belong to the document.
  REQUIRE(programme->getParent().lock() == doc);
  REQUIRE(content->getParent().lock() == doc);
  REQUIRE(object->getParent().lock() == doc);
  // ... and be reachable through the usual element collections.
  auto programmes = doc->getElements<AudioProgramme>();
  REQUIRE(std::find(programmes.begin(), programmes.end(), programme) !=
          programmes.end());
  auto contents = doc->getElements<AudioContent>();
  REQUIRE(std::find(contents.begin(), contents.end(), content) !=
          contents.end());
  auto objects = doc->getElements<AudioObject>();
  REQUIRE(std::find(objects.begin(), objects.end(), object) != objects.end());
}

TEST_CASE(
    "TagGroups with same tag are only equal if references are also equal") {
  auto object = adm::AudioObject::create(AudioObjectName("First"));
  auto programme = adm::AudioProgramme::create(AudioProgrammeName("Second"));
  Tag tag("Duplicate");
  TagGroup first(object);
  first.add(tag);

  TagGroup second(programme);
  second.add(tag);

  TagGroup third(object);
  third.add(tag);

  REQUIRE(first != second);
  REQUIRE(first == first);
  REQUIRE(second == second);
  REQUIRE(first == third);
}