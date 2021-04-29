#include <catch2/catch.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

TEST_CASE("write_audio_track_uid_channel_format_reference") {
  using namespace adm;

  auto document = Document::create();

  auto audioPackFormat = AudioPackFormat::create(
      AudioPackFormatName("MyPackFormat"), TypeDefinition::DIRECT_SPEAKERS);
  auto audioChannelFormat =
      AudioChannelFormat::create(AudioChannelFormatName("MyChannelFormat"),
                                 TypeDefinition::DIRECT_SPEAKERS);
  auto audioTrackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("MyTrackFormat"), FormatDefinition::PCM);

  // TODO how complete does the document have to be in these tests, do we need
  // the audioContent, an object, and a streamFormat linking track to channel
  // format?

  document->add(audioPackFormat);
  document->add(audioChannelFormat);
  document->add(audioTrackFormat);

  // 1. AudioTrackUid with direct link to a channelFormat
  {
    auto audioTrackUid = AudioTrackUid::create(AudioTrackUidIdValue(1));

    audioTrackUid->setReference(audioPackFormat);
    audioTrackUid->setReference(audioChannelFormat);
    document->add(audioTrackUid);
  }

  // 2. AudioTrackUid with link to trackFormat instead
  {
    auto audioTrackUid = AudioTrackUid::create(AudioTrackUidIdValue(2));

    audioTrackUid->setReference(audioPackFormat);
    audioTrackUid->setReference(audioTrackFormat);
    document->add(audioTrackUid);
  }

  std::stringstream xml;
  writeXml(xml, document);

  CHECK_THAT(xml.str(),
             EqualsXmlFile("write_audio_track_uid_channel_format_reference"));
}
