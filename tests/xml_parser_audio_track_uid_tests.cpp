#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_track_uid.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser_audio_track_uid") {
  using namespace adm;
  // Minimal
  {
    std::istringstream admStream(
        "<audioFormatExtended>"
        "<audioTrackUID "
        "UID=\"ATU_00000001\" "
        "sampleRate=\"48000\" "
        "bitDepth=\"24\" "
        "/>"
        "</audioFormatExtended>");
    auto document = adm::parseXml(admStream);
    auto audioTrackUids = document->getElements<AudioTrackUid>();
    auto audioTrackUid = *audioTrackUids.begin();

    REQUIRE(audioTrackUid->has<AudioTrackUidId>() == true);
    REQUIRE(audioTrackUid->has<SampleRate>() == true);
    REQUIRE(audioTrackUid->has<BitDepth>() == true);

    REQUIRE(audioTrackUid->get<AudioTrackUidId>().get<AudioTrackUidIdValue>() ==
            0x00000001u);
    REQUIRE(audioTrackUid->get<SampleRate>() == 48000u);
    REQUIRE(audioTrackUid->get<BitDepth>() == 24u);
  }
}

TEST_CASE("duplicate_id") {
  std::istringstream admStream(
      "<audioFormatExtended>"
      "<audioTrackUID "
      "UID=\"ATU_00000001\" "
      "sampleRate=\"48000\" "
      "bitDepth=\"24\" "
      "/>"
      "<audioTrackUID "
      "UID=\"ATU_00000001\" "
      "sampleRate=\"48000\" "
      "bitDepth=\"24\" "
      "/>"
      "</audioFormatExtended>");

  REQUIRE_THROWS_AS(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
