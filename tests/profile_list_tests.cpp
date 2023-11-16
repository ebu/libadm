#include <catch2/catch.hpp>
#include "adm/elements/profile_list.hpp"
#include "helper/parameter_checks.hpp"
#include "adm/document.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"
#include "adm/serial.hpp"

using namespace adm;
using namespace adm_test;

TEST_CASE("Profile parameters") {
  Profile profile{
      ProfileValue{"value"},
      ProfileName{"name"},
      ProfileVersion{"version"},
      ProfileLevel{"level"},
  };

  check_required_param<ProfileValue>(profile,
                                     hasDefaultOf(ProfileValue{"value"}),
                                     canBeSetTo(ProfileValue{"value2"}));
  check_required_param<ProfileName>(profile, hasDefaultOf(ProfileName{"name"}),
                                    canBeSetTo(ProfileName{"name2"}));
  check_required_param<ProfileVersion>(profile,
                                       hasDefaultOf(ProfileVersion{"version"}),
                                       canBeSetTo(ProfileVersion{"version2"}));
  check_required_param<ProfileLevel>(profile,
                                     hasDefaultOf(ProfileLevel{"level"}),
                                     canBeSetTo(ProfileLevel{"level2"}));
}

TEST_CASE("ProfileList parameters") {
  ProfileList profileList;

  Profile profile{
      ProfileValue{"value"},
      ProfileName{"name"},
      ProfileVersion{"version"},
      ProfileLevel{"level"},
  };

  check_vector_param<Profiles>(profileList, canBeSetTo(Profiles{profile}));
}

TEST_CASE("xml/profileList") {
  auto document = parseXml("xml_parser/profile_list.xml");

  REQUIRE(document->has<ProfileList>());
  auto profileList = document->get<ProfileList>();
  auto profiles = profileList.get<Profiles>();
  REQUIRE(profiles.size() == 2);

  CHECK(profiles.at(0).get<ProfileValue>() == "value1");
  CHECK(profiles.at(0).get<ProfileName>() == "name1");
  CHECK(profiles.at(0).get<ProfileVersion>() == "version1");
  CHECK(profiles.at(0).get<ProfileLevel>() == "level1");

  CHECK(profiles.at(1).get<ProfileValue>() == "value2");
  CHECK(profiles.at(1).get<ProfileName>() == "name2");
  CHECK(profiles.at(1).get<ProfileVersion>() == "version2");
  CHECK(profiles.at(1).get<ProfileLevel>() == "level2");

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(), EqualsXmlFile("profile_list"));
}

TEST_CASE("sadm xml/profilelist") {
  auto document = parseXml("xml_parser/profile_list.xml");
  std::stringstream xml;
  using namespace std::chrono_literals;
  FrameHeader header{
      FrameFormat{FrameFormatId{FrameIndex{1}}, Start{0s}, Duration{500ms},
                  FrameType{FrameTypeValue::FULL}},
      document->get<ProfileList>()};
  writeXml(xml, document, header);
  CHECK_THAT(xml.str(), EqualsXmlFile("profile_list_frame_header"));
}
