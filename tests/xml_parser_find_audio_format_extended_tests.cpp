#include <catch2/catch.hpp>
#include <sstream>
#include "adm/parse.hpp"
#include "test_config.hpp"

TEST_CASE("xml_parser/find_audio_format_extended_ebu") {
  adm::parseXml(data_file("xml_parser/find_audio_format_extended_ebu.xml"));
}

TEST_CASE("find_audio_format_extended_itu") {
  adm::parseXml(data_file("xml_parser/find_audio_format_extended_itu.xml"),
                adm::xml::ParserOptions::recursive_node_search);
}

TEST_CASE("xml_parser/find_audio_format_extended_ebu_with_other_metadata") {
  adm::parseXml(data_file(
      "xml_parser/find_audio_format_extended_ebu_with_other_metadata.xml"));
}
