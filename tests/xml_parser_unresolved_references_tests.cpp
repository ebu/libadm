#include <catch2/catch.hpp>
#include <sstream>
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "test_config.hpp"

std::string formatFilepath(const std::string& filename) {
  std::stringstream ss;
  ss << "xml_parser/unresolved_references/" << filename << ".xml";
  return ss.str();
}

TEST_CASE("xml_parser/unresolved_references") {
  for (auto filename :
       {"audio_programme", "audio_content", "audio_object_1", "audio_object_2",
        "audio_object_3", "audio_pack_format_1", "audio_pack_format_2",
        "audio_track_uid_1", "audio_track_uid_2", "audio_track_format",
        "audio_stream_format_1", "audio_stream_format_2"}) {
    SECTION(filename) {
      REQUIRE_THROWS_AS(adm::parseXml(data_file(formatFilepath(filename))),
                        adm::error::XmlParsingUnresolvedReference);
    }
  }
}
