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

TEST_CASE("adm_id") {
  using namespace adm;
  // parse ids
  {
    auto audioProgrammeId = parseAudioProgrammeId("APR_0001");
    REQUIRE(audioProgrammeId.get<AudioProgrammeIdValue>() == 1u);
    auto audioContentId = parseAudioContentId("ACO_0001");
    REQUIRE(audioContentId.get<AudioContentIdValue>() == 1u);
    auto audioObjectId = parseAudioObjectId("AO_0001");
    REQUIRE(audioObjectId.get<AudioObjectIdValue>() == 1u);
    auto audioPackFormatId = parseAudioPackFormatId("AP_00010001");
    REQUIRE(audioPackFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioPackFormatId.get<AudioPackFormatIdValue>() == 1u);
    auto audioChannelFormatId = parseAudioChannelFormatId("AC_00010001");
    REQUIRE(audioChannelFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioChannelFormatId.get<AudioChannelFormatIdValue>() == 1u);
    auto audioBlockFormatId = parseAudioBlockFormatId("AB_00010001_00000001");
    REQUIRE(audioBlockFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioBlockFormatId.get<AudioBlockFormatIdValue>() == 1u);
    REQUIRE(audioBlockFormatId.get<AudioBlockFormatIdCounter>() == 1u);
    auto audioTrackFormatId = parseAudioTrackFormatId("AT_00010001_01");
    REQUIRE(audioTrackFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioTrackFormatId.get<AudioTrackFormatIdValue>() == 1u);
    REQUIRE(audioTrackFormatId.get<AudioTrackFormatIdCounter>() == 1u);
    auto audioStreamFormatId = parseAudioStreamFormatId("AS_00010001");
    REQUIRE(audioStreamFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioStreamFormatId.get<AudioStreamFormatIdValue>() == 1u);
    auto audioTrackUidId = parseAudioTrackUidId("ATU_00000001");
    REQUIRE(audioTrackUidId.get<AudioTrackUidIdValue>() == 1u);
  }
  // format ids
  {
    auto audioProgrammeId =
        formatId(AudioProgrammeId(AudioProgrammeIdValue(1)));
    REQUIRE(audioProgrammeId == "APR_0001");
    auto audioContentId = formatId(AudioContentId(AudioContentIdValue(1)));
    REQUIRE(audioContentId == "ACO_0001");
    auto audioObjectId = formatId(AudioObjectId(AudioObjectIdValue(1)));
    REQUIRE(audioObjectId == "AO_0001");
    auto audioPackFormatId = formatId(
        AudioPackFormatId(AudioPackFormatIdValue(1), TypeDescriptor(1)));
    REQUIRE(audioPackFormatId == "AP_00010001");
    auto audioChannelFormatId = formatId(
        AudioChannelFormatId(AudioChannelFormatIdValue(1), TypeDescriptor(1)));
    REQUIRE(audioChannelFormatId == "AC_00010001");
    auto audioBlockFormatId = formatId(
        AudioBlockFormatId(AudioBlockFormatIdValue(1), TypeDescriptor(1),
                           AudioBlockFormatIdCounter(1)));
    REQUIRE(audioBlockFormatId == "AB_00010001_00000001");
    auto audioTrackFormatId = formatId(
        AudioTrackFormatId(AudioTrackFormatIdValue(1), TypeDescriptor(2),
                           AudioTrackFormatIdCounter(1)));
    REQUIRE(audioTrackFormatId == "AT_00020001_01");
    auto audioStreamFormatId = formatId(
        AudioStreamFormatId(AudioStreamFormatIdValue(1), TypeDescriptor(2)));
    REQUIRE(audioStreamFormatId == "AS_00020001");
    auto audioTrackUidId = formatId(AudioTrackUidId(AudioTrackUidIdValue(1)));
    REQUIRE(audioTrackUidId == "ATU_00000001");
  }
  // parse hex values
  {
    auto audioProgrammeId = parseAudioProgrammeId("APR_000a");
    REQUIRE(audioProgrammeId.get<AudioProgrammeIdValue>() == 10u);
    auto audioContentId = parseAudioContentId("ACO_000a");
    REQUIRE(audioContentId.get<AudioContentIdValue>() == 10u);
    auto audioObjectId = parseAudioObjectId("AO_000a");
    REQUIRE(audioObjectId.get<AudioObjectIdValue>() == 10u);
    auto audioPackFormatId = parseAudioPackFormatId("AP_0001000a");
    REQUIRE(audioPackFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioPackFormatId.get<AudioPackFormatIdValue>() == 10u);
    auto audioChannelFormatId = parseAudioChannelFormatId("AC_0001000a");
    REQUIRE(audioChannelFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioChannelFormatId.get<AudioChannelFormatIdValue>() == 10u);
    auto audioBlockFormatId = parseAudioBlockFormatId("AB_0001000a_0000000a");
    REQUIRE(audioBlockFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioBlockFormatId.get<AudioBlockFormatIdValue>() == 10u);
    REQUIRE(audioBlockFormatId.get<AudioBlockFormatIdCounter>() == 10u);
    auto audioTrackFormatId = parseAudioTrackFormatId("AT_0001000a_0a");
    REQUIRE(audioTrackFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioTrackFormatId.get<AudioTrackFormatIdValue>() == 10u);
    REQUIRE(audioTrackFormatId.get<AudioTrackFormatIdCounter>() == 10u);
    auto audioStreamFormatId = parseAudioStreamFormatId("AS_0001000a");
    REQUIRE(audioStreamFormatId.get<TypeDescriptor>() ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(audioStreamFormatId.get<AudioStreamFormatIdValue>() == 10u);
    auto audioTrackUidId = parseAudioTrackUidId("ATU_0000000a");
    REQUIRE(audioTrackUidId.get<AudioTrackUidIdValue>() == 10u);
  }
}
