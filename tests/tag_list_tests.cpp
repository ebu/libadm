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
  TTag tag{TTagValue("value")};

  check_optional_param<TTagClass>(tag, canBeSetTo(TTagClass{"class2"}));
  check_required_param<TTagValue>(tag, hasDefaultOf(TTagValue{"value"}),
                                  canBeSetTo(TTagValue{"value2"}));
}

TEST_CASE("TagGroup parameters") {
  TagGroup tagGroup;
  TTag tag{TTagClass("class"), TTagValue("value")};

  check_vector_param<TTags>(tagGroup, canBeSetTo(TTags{tag}));
}

TEST_CASE("TagList parameters") {
  TTag tag{TTagClass("class"), TTagValue("value")};
  TagGroup tagGroup;
  tagGroup.add(tag);
  TagList tagList;

  check_vector_param<TagGroups>(tagList, canBeSetTo(TagGroups{tagGroup}));
}

TEST_CASE("adm xml/taglist") {
  auto doc = parseXml("tag_list.accepted.xml");
  auto tag_list = doc->getElement<TagList>();
  auto tag_groups = tag_list->get<TagGroups>();
  REQUIRE(tag_groups.size() == 1);

  auto tag_group = tag_groups.at(0);
  auto tags = tag_group.get<TTags>();
  auto tag = tags.at(0);
  CHECK(tag.get<TTagClass>() == "class1");
  CHECK(tag.get<TTagValue>() == "value1");

  std::stringstream xml;
  writeXml(xml, doc);
  CHECK_THAT(xml.str(), EqualsXmlFile("tag_list"));
}

TEST_CASE("document copy updates tagList references") {
  auto doc = Document::create();
  auto holder = addSimpleObjectTo(doc, "Test");
  TTag tag{TTagClass("class"), TTagValue("value")};
  TagGroup tagGroup;
  tagGroup.add(tag);
  tagGroup.addReference(holder.audioObject);
  auto tagList = std::make_shared<TagList>();
  tagList->add(tagGroup);
  doc->add(tagList);

  auto doc_copy = doc->deepCopy();
  auto copied_object = doc_copy->getElements<AudioObject>().front();
  auto copied_tag_group = doc->getElement<TagList>()->get<TagGroups>().front();
  auto tagged_object_ref =
      copied_tag_group.getReferences<AudioObject>().front();
  REQUIRE(copied_object == tagged_object_ref);
}
