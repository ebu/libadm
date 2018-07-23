#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_block_format_id.hpp"
#include "adm/elements/audio_channel_format_id.hpp"
#include "adm/elements/audio_content_id.hpp"
#include "adm/elements/audio_object_id.hpp"
#include "adm/elements/audio_pack_format_id.hpp"
#include "adm/elements/audio_programme_id.hpp"
#include "adm/elements/audio_stream_format_id.hpp"
#include "adm/elements/audio_track_format_id.hpp"
#include "adm/elements/audio_track_uid_id.hpp"

#define BOOST_TEST_MODULE AdmId
#include <boost/test/included/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(adm_id) {
  using namespace adm;
  // parse ids
  {
    auto audioProgrammeId = parseAudioProgrammeId("APR_0001");
    BOOST_TEST(audioProgrammeId.get<AudioProgrammeIdValue>() == 1u);
    auto audioContentId = parseAudioContentId("ACO_0001");
    BOOST_TEST(audioContentId.get<AudioContentIdValue>() == 1u);
    auto audioObjectId = parseAudioObjectId("AO_0001");
    BOOST_TEST(audioObjectId.get<AudioObjectIdValue>() == 1u);
    auto audioPackFormatId = parseAudioPackFormatId("AP_00010001");
    BOOST_TEST(audioPackFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioPackFormatId.get<AudioPackFormatIdValue>() == 1u);
    auto audioChannelFormatId = parseAudioChannelFormatId("AC_00010001");
    BOOST_TEST(audioChannelFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioChannelFormatId.get<AudioChannelFormatIdValue>() == 1u);
    auto audioBlockFormatId = parseAudioBlockFormatId("AB_00010001_00000001");
    BOOST_TEST(audioBlockFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioBlockFormatId.get<AudioBlockFormatIdValue>() == 1u);
    BOOST_TEST(audioBlockFormatId.get<AudioBlockFormatIdCounter>() == 1u);
    auto audioTrackFormatId = parseAudioTrackFormatId("AT_00010001_01");
    BOOST_TEST(audioTrackFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioTrackFormatId.get<AudioTrackFormatIdValue>() == 1u);
    BOOST_TEST(audioTrackFormatId.get<AudioTrackFormatIdCounter>() == 1u);
    auto audioStreamFormatId = parseAudioStreamFormatId("AS_00010001");
    BOOST_TEST(audioStreamFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioStreamFormatId.get<AudioStreamFormatIdValue>() == 1u);
    auto audioTrackUidId = parseAudioTrackUidId("ATU_00000001");
    BOOST_TEST(audioTrackUidId.get<AudioTrackUidIdValue>() == 1u);
  }
  // format ids
  {
    auto audioProgrammeId =
        formatId(AudioProgrammeId(AudioProgrammeIdValue(1)));
    BOOST_TEST(audioProgrammeId == "APR_0001");
    auto audioContentId = formatId(AudioContentId(AudioContentIdValue(1)));
    BOOST_TEST(audioContentId == "ACO_0001");
    auto audioObjectId = formatId(AudioObjectId(AudioObjectIdValue(1)));
    BOOST_TEST(audioObjectId == "AO_0001");
    auto audioPackFormatId = formatId(
        AudioPackFormatId(AudioPackFormatIdValue(1), TypeDescriptor(1)));
    BOOST_TEST(audioPackFormatId == "AP_00010001");
    auto audioChannelFormatId = formatId(
        AudioChannelFormatId(AudioChannelFormatIdValue(1), TypeDescriptor(1)));
    BOOST_TEST(audioChannelFormatId == "AC_00010001");
    auto audioBlockFormatId = formatId(
        AudioBlockFormatId(AudioBlockFormatIdValue(1), TypeDescriptor(1),
                           AudioBlockFormatIdCounter(1)));
    BOOST_TEST(audioBlockFormatId == "AB_00010001_00000001");
    auto audioTrackFormatId = formatId(
        AudioTrackFormatId(AudioTrackFormatIdValue(1), TypeDescriptor(2),
                           AudioTrackFormatIdCounter(1)));
    BOOST_TEST(audioTrackFormatId == "AT_00020001_01");
    auto audioStreamFormatId = formatId(
        AudioStreamFormatId(AudioStreamFormatIdValue(1), TypeDescriptor(2)));
    BOOST_TEST(audioStreamFormatId == "AS_00020001");
    auto audioTrackUidId = formatId(AudioTrackUidId(AudioTrackUidIdValue(1)));
    BOOST_TEST(audioTrackUidId == "ATU_00000001");
  }
  // parse hex values
  {
    auto audioProgrammeId = parseAudioProgrammeId("APR_000a");
    BOOST_TEST(audioProgrammeId.get<AudioProgrammeIdValue>() == 10u);
    auto audioContentId = parseAudioContentId("ACO_000a");
    BOOST_TEST(audioContentId.get<AudioContentIdValue>() == 10u);
    auto audioObjectId = parseAudioObjectId("AO_000a");
    BOOST_TEST(audioObjectId.get<AudioObjectIdValue>() == 10u);
    auto audioPackFormatId = parseAudioPackFormatId("AP_0001000a");
    BOOST_TEST(audioPackFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioPackFormatId.get<AudioPackFormatIdValue>() == 10u);
    auto audioChannelFormatId = parseAudioChannelFormatId("AC_0001000a");
    BOOST_TEST(audioChannelFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioChannelFormatId.get<AudioChannelFormatIdValue>() == 10u);
    auto audioBlockFormatId = parseAudioBlockFormatId("AB_0001000a_0000000a");
    BOOST_TEST(audioBlockFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioBlockFormatId.get<AudioBlockFormatIdValue>() == 10u);
    BOOST_TEST(audioBlockFormatId.get<AudioBlockFormatIdCounter>() == 10u);
    auto audioTrackFormatId = parseAudioTrackFormatId("AT_0001000a_0a");
    BOOST_TEST(audioTrackFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioTrackFormatId.get<AudioTrackFormatIdValue>() == 10u);
    BOOST_TEST(audioTrackFormatId.get<AudioTrackFormatIdCounter>() == 10u);
    auto audioStreamFormatId = parseAudioStreamFormatId("AS_0001000a");
    BOOST_TEST(audioStreamFormatId.get<TypeDescriptor>() ==
               TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(audioStreamFormatId.get<AudioStreamFormatIdValue>() == 10u);
    auto audioTrackUidId = parseAudioTrackUidId("ATU_0000000a");
    BOOST_TEST(audioTrackUidId.get<AudioTrackUidIdValue>() == 10u);
  }
}
