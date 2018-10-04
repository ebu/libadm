#include <catch2/catch.hpp>
#include <sstream>
#include "adm/xml_reader.hpp"

TEST_CASE("find_audio_format_extended_itu") {
  adm::parseXml("find_audio_format_extended_itu.xml");
}

TEST_CASE("find_audio_format_extended_ebu") {
  adm::parseXml("find_audio_format_extended_ebu.xml");
}
