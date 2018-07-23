#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include "adm/xml_reader.hpp"

#define BOOST_TEST_MODULE ParseAdm
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(find_audio_format_extended) {
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
