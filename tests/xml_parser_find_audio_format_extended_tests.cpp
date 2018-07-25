#include <catch2/catch.hpp>
#include <sstream>
#include "adm/xml_reader.hpp"

TEST_CASE("find_audio_format_extended") {
  {
    std::istringstream admStream(
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<ituADM>"
        "<audioFormatExtended>"
        "</audioFormatExtended>"
        "</ituADM>");
    adm::parseXml(admStream);
  }
  {
    std::istringstream admStream(
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<ebuCoreMain>"
        "<coreMetadata>"
        "<format>"
        "<audioFormatExtended>"
        "</audioFormatExtended>"
        "</format>"
        "</coreMetadata>"
        "</ebuCoreMain>");
    adm::parseXml(admStream);
  }
}
