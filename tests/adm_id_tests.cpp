#include <catch2/catch.hpp>
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/audio_channel_format_id.hpp"
#include "adm/elements/audio_content_id.hpp"
#include "adm/elements/audio_object_id.hpp"
#include "adm/elements/audio_pack_format_id.hpp"
#include "adm/elements/audio_programme_id.hpp"
#include "adm/elements/audio_stream_format_id.hpp"
#include "adm/elements/audio_track_format_id.hpp"
#include "adm/elements/audio_track_uid_id.hpp"

TEST_CASE("audio_programme_id") {
  using namespace adm;
  AudioProgrammeId audioProgrammeId;

  REQUIRE(audioProgrammeId.has<AudioProgrammeIdValue>() == true);
  REQUIRE(audioProgrammeId.isDefault<AudioProgrammeIdValue>() == true);
  audioProgrammeId.set(AudioProgrammeIdValue(1u));
  REQUIRE(audioProgrammeId.isDefault<AudioProgrammeIdValue>() == false);
  audioProgrammeId.unset<AudioProgrammeIdValue>();
  REQUIRE(audioProgrammeId.isDefault<AudioProgrammeIdValue>() == true);

  audioProgrammeId = parseAudioProgrammeId("APR_0001");
  REQUIRE(audioProgrammeId.get<AudioProgrammeIdValue>() == 1u);
  REQUIRE(formatId(audioProgrammeId) == "APR_0001");

  REQUIRE(parseAudioProgrammeId("APR_0001") ==
          parseAudioProgrammeId("APR_0001"));
  REQUIRE(parseAudioProgrammeId("APR_0001") !=
          parseAudioProgrammeId("APR_0002"));
  REQUIRE(parseAudioProgrammeId("APR_0001") <
          parseAudioProgrammeId("APR_0002"));

  REQUIRE(parseAudioProgrammeId("APR_000a").get<AudioProgrammeIdValue>() ==
          10u);
  REQUIRE(parseAudioProgrammeId("APR_000A").get<AudioProgrammeIdValue>() ==
          10u);

  REQUIRE_THROWS(parseAudioProgrammeId("APR_001"));
}

TEST_CASE("audio_content_id") {
  using namespace adm;
  AudioContentId audioContentId;

  REQUIRE(audioContentId.has<AudioContentIdValue>() == true);
  REQUIRE(audioContentId.isDefault<AudioContentIdValue>() == true);
  audioContentId.set(AudioContentIdValue(1u));
  REQUIRE(audioContentId.isDefault<AudioContentIdValue>() == false);
  audioContentId.unset<AudioContentIdValue>();
  REQUIRE(audioContentId.isDefault<AudioContentIdValue>() == true);

  audioContentId = parseAudioContentId("ACO_0001");
  REQUIRE(audioContentId.get<AudioContentIdValue>() == 1u);
  REQUIRE(formatId(audioContentId) == "ACO_0001");

  REQUIRE(parseAudioContentId("ACO_0001") == parseAudioContentId("ACO_0001"));
  REQUIRE(parseAudioContentId("ACO_0001") != parseAudioContentId("ACO_0002"));
  REQUIRE(parseAudioContentId("ACO_0001") < parseAudioContentId("ACO_0002"));

  REQUIRE(parseAudioContentId("ACO_000a").get<AudioContentIdValue>() == 10u);
  REQUIRE(parseAudioContentId("ACO_000A").get<AudioContentIdValue>() == 10u);

  REQUIRE_THROWS(parseAudioContentId("ACO_001"));
}

TEST_CASE("audio_object_id") {
  using namespace adm;
  AudioObjectId audioObjectId;

  REQUIRE(audioObjectId.has<AudioObjectIdValue>() == true);
  REQUIRE(audioObjectId.isDefault<AudioObjectIdValue>() == true);
  audioObjectId.set(AudioObjectIdValue(1u));
  REQUIRE(audioObjectId.isDefault<AudioObjectIdValue>() == false);
  audioObjectId.unset<AudioObjectIdValue>();
  REQUIRE(audioObjectId.isDefault<AudioObjectIdValue>() == true);

  audioObjectId = parseAudioObjectId("AO_0001");
  REQUIRE(audioObjectId.get<AudioObjectIdValue>() == 1u);
  REQUIRE(formatId(audioObjectId) == "AO_0001");

  REQUIRE(parseAudioObjectId("AO_0001") == parseAudioObjectId("AO_0001"));
  REQUIRE(parseAudioObjectId("AO_0001") != parseAudioObjectId("AO_0002"));
  REQUIRE(parseAudioObjectId("AO_0001") < parseAudioObjectId("AO_0002"));

  REQUIRE(parseAudioObjectId("AO_000a").get<AudioObjectIdValue>() == 10u);
  REQUIRE(parseAudioObjectId("AO_000A").get<AudioObjectIdValue>() == 10u);

  REQUIRE_THROWS(parseAudioObjectId("AO_001"));
}

TEST_CASE("audio_track_uid_id") {
  using namespace adm;
  AudioTrackUidId audioTrackUidId;

  REQUIRE(audioTrackUidId.has<AudioTrackUidIdValue>() == true);
  REQUIRE(audioTrackUidId.isDefault<AudioTrackUidIdValue>() == true);
  audioTrackUidId.set(AudioTrackUidIdValue(1u));
  REQUIRE(audioTrackUidId.isDefault<AudioTrackUidIdValue>() == false);
  audioTrackUidId.unset<AudioTrackUidIdValue>();
  REQUIRE(audioTrackUidId.isDefault<AudioTrackUidIdValue>() == true);

  audioTrackUidId = parseAudioTrackUidId("ATU_00000001");
  REQUIRE(audioTrackUidId.get<AudioTrackUidIdValue>() == 1u);
  REQUIRE(formatId(audioTrackUidId) == "ATU_00000001");

  REQUIRE(parseAudioTrackUidId("ATU_00000001") ==
          parseAudioTrackUidId("ATU_00000001"));
  REQUIRE(parseAudioTrackUidId("ATU_00000001") !=
          parseAudioTrackUidId("ATU_00000002"));
  REQUIRE(parseAudioTrackUidId("ATU_00000001") <
          parseAudioTrackUidId("ATU_00000002"));

  REQUIRE(parseAudioTrackUidId("ATU_0000000a").get<AudioTrackUidIdValue>() ==
          10u);
  REQUIRE(parseAudioTrackUidId("ATU_0000000A").get<AudioTrackUidIdValue>() ==
          10u);

  REQUIRE_THROWS(parseAudioTrackUidId("ATU_0000001"));
}

TEST_CASE("audio_pack_format_id") {
  using namespace adm;
  AudioPackFormatId audioPackFormatId;

  REQUIRE(audioPackFormatId.has<TypeDescriptor>() == true);
  REQUIRE(audioPackFormatId.has<AudioPackFormatIdValue>() == true);
  REQUIRE(audioPackFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioPackFormatId.isDefault<AudioPackFormatIdValue>() == true);
  audioPackFormatId.set(TypeDefinition::DIRECT_SPEAKERS);
  audioPackFormatId.set(AudioPackFormatIdValue(1u));
  REQUIRE(audioPackFormatId.isDefault<TypeDescriptor>() == false);
  REQUIRE(audioPackFormatId.isDefault<AudioPackFormatIdValue>() == false);
  audioPackFormatId.unset<TypeDescriptor>();
  audioPackFormatId.unset<AudioPackFormatIdValue>();
  REQUIRE(audioPackFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioPackFormatId.isDefault<AudioPackFormatIdValue>() == true);

  audioPackFormatId = parseAudioPackFormatId("AP_00030001");
  REQUIRE(audioPackFormatId.get<TypeDescriptor>() == TypeDefinition::OBJECTS);
  REQUIRE(audioPackFormatId.get<AudioPackFormatIdValue>() == 1u);
  REQUIRE(formatId(audioPackFormatId) == "AP_00030001");

  REQUIRE(parseAudioPackFormatId("AP_00030001") ==
          parseAudioPackFormatId("AP_00030001"));
  REQUIRE(parseAudioPackFormatId("AP_00030001") !=
          parseAudioPackFormatId("AP_00030002"));
  REQUIRE(parseAudioPackFormatId("AP_00030001") <
          parseAudioPackFormatId("AP_00030002"));

  REQUIRE(parseAudioPackFormatId("AP_0001000a").get<AudioPackFormatIdValue>() ==
          10u);
  REQUIRE(parseAudioPackFormatId("AP_0001000A").get<AudioPackFormatIdValue>() ==
          10u);

  REQUIRE_THROWS(parseAudioPackFormatId("AP_0003001"));
}

TEST_CASE("audio_channel_format_id") {
  using namespace adm;
  AudioChannelFormatId audioChannelFormatId;

  REQUIRE(audioChannelFormatId.has<TypeDescriptor>() == true);
  REQUIRE(audioChannelFormatId.has<AudioChannelFormatIdValue>() == true);
  REQUIRE(audioChannelFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioChannelFormatId.isDefault<AudioChannelFormatIdValue>() == true);
  audioChannelFormatId.set(TypeDefinition::DIRECT_SPEAKERS);
  audioChannelFormatId.set(AudioChannelFormatIdValue(1u));
  REQUIRE(audioChannelFormatId.isDefault<TypeDescriptor>() == false);
  REQUIRE(audioChannelFormatId.isDefault<AudioChannelFormatIdValue>() == false);
  audioChannelFormatId.unset<TypeDescriptor>();
  audioChannelFormatId.unset<AudioChannelFormatIdValue>();
  REQUIRE(audioChannelFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioChannelFormatId.isDefault<AudioChannelFormatIdValue>() == true);

  audioChannelFormatId = parseAudioChannelFormatId("AC_00030001");
  REQUIRE(audioChannelFormatId.get<TypeDescriptor>() ==
          TypeDefinition::OBJECTS);
  REQUIRE(audioChannelFormatId.get<AudioChannelFormatIdValue>() == 1u);
  REQUIRE(formatId(audioChannelFormatId) == "AC_00030001");

  REQUIRE(parseAudioChannelFormatId("AC_00030001") ==
          parseAudioChannelFormatId("AC_00030001"));
  REQUIRE(parseAudioChannelFormatId("AC_00030001") !=
          parseAudioChannelFormatId("AC_00030002"));
  REQUIRE(parseAudioChannelFormatId("AC_00030001") <
          parseAudioChannelFormatId("AC_00030002"));

  REQUIRE(parseAudioChannelFormatId("AC_0001000a")
              .get<AudioChannelFormatIdValue>() == 10u);
  REQUIRE(parseAudioChannelFormatId("AC_0001000A")
              .get<AudioChannelFormatIdValue>() == 10u);

  REQUIRE_THROWS(parseAudioChannelFormatId("AC_0003001"));
}

TEST_CASE("audio_stream_format_id") {
  using namespace adm;
  AudioStreamFormatId audioStreamFormatId;

  REQUIRE(audioStreamFormatId.has<TypeDescriptor>() == true);
  REQUIRE(audioStreamFormatId.has<AudioStreamFormatIdValue>() == true);
  REQUIRE(audioStreamFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioStreamFormatId.isDefault<AudioStreamFormatIdValue>() == true);
  audioStreamFormatId.set(TypeDefinition::UNDEFINED);
  audioStreamFormatId.set(AudioStreamFormatIdValue(1u));
  REQUIRE(audioStreamFormatId.isDefault<TypeDescriptor>() == false);
  REQUIRE(audioStreamFormatId.isDefault<AudioStreamFormatIdValue>() == false);
  audioStreamFormatId.unset<TypeDescriptor>();
  audioStreamFormatId.unset<AudioStreamFormatIdValue>();
  REQUIRE(audioStreamFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioStreamFormatId.isDefault<AudioStreamFormatIdValue>() == true);

  audioStreamFormatId = parseAudioStreamFormatId("AS_00010001");
  REQUIRE(audioStreamFormatId.get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS);
  REQUIRE(audioStreamFormatId.get<AudioStreamFormatIdValue>() == 1u);
  REQUIRE(formatId(audioStreamFormatId) == "AS_00010001");

  REQUIRE(parseAudioStreamFormatId("AS_00010001") ==
          parseAudioStreamFormatId("AS_00010001"));
  REQUIRE(parseAudioStreamFormatId("AS_00010001") !=
          parseAudioStreamFormatId("AS_00010002"));
  REQUIRE(parseAudioStreamFormatId("AS_00010001") <
          parseAudioStreamFormatId("AS_00010002"));

  REQUIRE(
      parseAudioStreamFormatId("AS_0001000a").get<AudioStreamFormatIdValue>() ==
      10u);
  REQUIRE(
      parseAudioStreamFormatId("AS_0001000A").get<AudioStreamFormatIdValue>() ==
      10u);

  REQUIRE_THROWS(parseAudioStreamFormatId("AS_0001001"));
}

TEST_CASE("audio_track_format_id") {
  using namespace adm;
  AudioTrackFormatId audioTrackFormatId;

  REQUIRE(audioTrackFormatId.has<TypeDescriptor>() == true);
  REQUIRE(audioTrackFormatId.has<AudioTrackFormatIdValue>() == true);
  REQUIRE(audioTrackFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioTrackFormatId.isDefault<AudioTrackFormatIdValue>() == true);
  audioTrackFormatId.set(TypeDefinition::UNDEFINED);
  audioTrackFormatId.set(AudioTrackFormatIdValue(1u));
  REQUIRE(audioTrackFormatId.isDefault<TypeDescriptor>() == false);
  REQUIRE(audioTrackFormatId.isDefault<AudioTrackFormatIdValue>() == false);
  audioTrackFormatId.unset<TypeDescriptor>();
  audioTrackFormatId.unset<AudioTrackFormatIdValue>();
  REQUIRE(audioTrackFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioTrackFormatId.isDefault<AudioTrackFormatIdValue>() == true);

  audioTrackFormatId = parseAudioTrackFormatId("AT_00010001_01");
  REQUIRE(audioTrackFormatId.get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS);
  REQUIRE(audioTrackFormatId.get<AudioTrackFormatIdValue>() == 1u);
  REQUIRE(audioTrackFormatId.get<AudioTrackFormatIdCounter>() == 1u);
  REQUIRE(formatId(audioTrackFormatId) == "AT_00010001_01");

  REQUIRE(parseAudioTrackFormatId("AT_00010001_01") ==
          parseAudioTrackFormatId("AT_00010001_01"));
  REQUIRE(parseAudioTrackFormatId("AT_00010001_01") !=
          parseAudioTrackFormatId("AT_00010002_01"));
  REQUIRE(parseAudioTrackFormatId("AT_00010001_01") <
          parseAudioTrackFormatId("AT_00010002_01"));

  REQUIRE(parseAudioTrackFormatId("AT_0001000a_0a")
              .get<AudioTrackFormatIdValue>() == 10u);
  REQUIRE(parseAudioTrackFormatId("AT_0001000A_0a")
              .get<AudioTrackFormatIdValue>() == 10u);
  REQUIRE(parseAudioTrackFormatId("AT_0001000a_0a")
              .get<AudioTrackFormatIdCounter>() == 10u);
  REQUIRE(parseAudioTrackFormatId("AT_0001000A_0a")
              .get<AudioTrackFormatIdCounter>() == 10u);

  REQUIRE_THROWS(parseAudioTrackFormatId("AT_0001001"));
}

TEST_CASE("audio_block_format_id") {
  using namespace adm;
  AudioBlockFormatId audioBlockFormatId;

  REQUIRE(audioBlockFormatId.has<TypeDescriptor>() == true);
  REQUIRE(audioBlockFormatId.has<AudioBlockFormatIdValue>() == true);
  REQUIRE(audioBlockFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioBlockFormatId.isDefault<AudioBlockFormatIdValue>() == true);
  audioBlockFormatId.set(TypeDefinition::UNDEFINED);
  audioBlockFormatId.set(AudioBlockFormatIdValue(1u));
  REQUIRE(audioBlockFormatId.isDefault<TypeDescriptor>() == false);
  REQUIRE(audioBlockFormatId.isDefault<AudioBlockFormatIdValue>() == false);
  audioBlockFormatId.unset<TypeDescriptor>();
  audioBlockFormatId.unset<AudioBlockFormatIdValue>();
  REQUIRE(audioBlockFormatId.isDefault<TypeDescriptor>() == true);
  REQUIRE(audioBlockFormatId.isDefault<AudioBlockFormatIdValue>() == true);

  audioBlockFormatId = parseAudioBlockFormatId("AB_00010001_00000001");
  REQUIRE(audioBlockFormatId.get<TypeDescriptor>() ==
          TypeDefinition::DIRECT_SPEAKERS);
  REQUIRE(audioBlockFormatId.get<AudioBlockFormatIdValue>() == 1u);
  REQUIRE(audioBlockFormatId.get<AudioBlockFormatIdCounter>() == 1u);
  REQUIRE(formatId(audioBlockFormatId) == "AB_00010001_00000001");

  REQUIRE(parseAudioBlockFormatId("AB_00010001_00000001") ==
          parseAudioBlockFormatId("AB_00010001_00000001"));
  REQUIRE(parseAudioBlockFormatId("AB_00010001_00000001") !=
          parseAudioBlockFormatId("AB_00010001_00000002"));
  REQUIRE(parseAudioBlockFormatId("AB_00010001_00000001") <
          parseAudioBlockFormatId("AB_00010001_00000002"));

  REQUIRE(parseAudioBlockFormatId("AB_0001000a_0000000a")
              .get<AudioBlockFormatIdCounter>() == 10u);
  REQUIRE(parseAudioBlockFormatId("AB_0001000A_0000000A")
              .get<AudioBlockFormatIdCounter>() == 10u);
  REQUIRE(parseAudioBlockFormatId("AB_0001000a_0000000a")
              .get<AudioBlockFormatIdValue>() == 10u);
  REQUIRE(parseAudioBlockFormatId("AB_0001000A_0000000A")
              .get<AudioBlockFormatIdValue>() == 10u);

  REQUIRE_THROWS(parseAudioBlockFormatId("AT_0001001"));
}
