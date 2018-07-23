#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_track_uid.hpp"
#include "adm/xml_reader.hpp"
#include "adm/errors.hpp"

#define BOOST_TEST_MODULE XmlParserAudioTrackUid
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(xml_parser_audio_track_uid) {
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

    BOOST_TEST(audioTrackUid->has<AudioTrackUidId>() == true);
    BOOST_TEST(audioTrackUid->has<SampleRate>() == true);
    BOOST_TEST(audioTrackUid->has<BitDepth>() == true);

    BOOST_TEST(
        audioTrackUid->get<AudioTrackUidId>().get<AudioTrackUidIdValue>() ==
        0x00000001u);
    BOOST_TEST(audioTrackUid->get<SampleRate>() == 48000u);
    BOOST_TEST(audioTrackUid->get<BitDepth>() == 24u);
  }
}

BOOST_AUTO_TEST_CASE(duplicate_id) {
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

  BOOST_CHECK_THROW(adm::parseXml(admStream),
                    adm::error::XmlParsingDuplicateId);
}
