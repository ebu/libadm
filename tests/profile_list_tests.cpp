#include <catch2/catch.hpp>
#include "adm/elements/profile_list.hpp"
#include "helper/parameter_checks.hpp"
#include "adm/document.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include "adm/parse_sadm.hpp"
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

TEST_CASE("sadm xml/profilelist") {
  auto header = parseFrameHeader("profile_list_frame_header.accepted.xml");
  REQUIRE(header.has<ProfileList>());
  auto profileList = header.get<ProfileList>();
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
  using namespace std::chrono_literals;
  auto document = Document::create();
  writeXml(xml, document, header);
  CHECK_THAT(xml.str(), EqualsXmlFile("profile_list_frame_header"));
}
