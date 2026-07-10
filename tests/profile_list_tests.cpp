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
  auto document = Document::create();
  writeXml(xml, document, header);
  CHECK_THAT(xml.str(), EqualsXmlFile("profile_list_frame_header"));
}

TEST_CASE("document deep copy preserves profile list") {
  auto document = Document::create();

  ProfileList profileList;
  profileList.add(Profile{ProfileValue{"value1"}, ProfileName{"name1"},
                          ProfileVersion{"version1"}, ProfileLevel{"level1"}});
  profileList.add(Profile{ProfileValue{"value2"}, ProfileName{"name2"},
                          ProfileVersion{"version2"}, ProfileLevel{"level2"}});
  document->set(profileList);

  auto documentCopy = document->deepCopy();
  REQUIRE(documentCopy->has<ProfileList>());
  auto copiedProfiles = documentCopy->get<ProfileList>().get<Profiles>();
  REQUIRE(copiedProfiles.size() == 2);

  CHECK(copiedProfiles.at(0).get<ProfileValue>() == "value1");
  CHECK(copiedProfiles.at(0).get<ProfileName>() == "name1");
  CHECK(copiedProfiles.at(0).get<ProfileVersion>() == "version1");
  CHECK(copiedProfiles.at(0).get<ProfileLevel>() == "level1");

  CHECK(copiedProfiles.at(1).get<ProfileValue>() == "value2");
  CHECK(copiedProfiles.at(1).get<ProfileName>() == "name2");
  CHECK(copiedProfiles.at(1).get<ProfileVersion>() == "version2");
  CHECK(copiedProfiles.at(1).get<ProfileLevel>() == "level2");

  ProfileList updatedProfileList;
  updatedProfileList.add(
      Profile{ProfileValue{"changed"}, ProfileName{"changed"},
              ProfileVersion{"changed"}, ProfileLevel{"changed"}});
  document->set(updatedProfileList);

  auto copiedProfilesAfterUpdate =
      documentCopy->get<ProfileList>().get<Profiles>();
  REQUIRE(copiedProfilesAfterUpdate.size() == 2);
  CHECK(copiedProfilesAfterUpdate.at(0).get<ProfileValue>() == "value1");
}
