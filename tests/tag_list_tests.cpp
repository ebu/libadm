#include <catch2/catch.hpp>
#include "helper/parameter_checks.hpp"
#include "adm/document.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"
#include "adm/elements/tag_list.hpp"

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
  TagGroup tagGroup;
  Tag tag{TagClass("class"), TagValue("value")};

  check_vector_param<Tags>(tagGroup, canBeSetTo(Tags{tag}));
}

TEST_CASE("TagList parameters") {
  Tag tag{TagClass("class"), TagValue("value")};
  TagGroup tagGroup;
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