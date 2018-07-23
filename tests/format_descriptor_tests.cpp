#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/format_descriptor.hpp"

#define BOOST_TEST_MODULE FormatDescriptor
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(format_descriptor) {
  using namespace adm;

  // parseFormatLabel
  {
    BOOST_TEST(parseFormatLabel("0000") == FormatDefinition::UNDEFINED);
    BOOST_TEST(parseFormatLabel("0001") == FormatDefinition::PCM);
  }
  // parseFormatDefinition
  {
    BOOST_TEST(parseFormatDefinition("Undefined") ==
               FormatDefinition::UNDEFINED);
    BOOST_TEST(parseFormatDefinition("PCM") == FormatDefinition::PCM);
  }
  // formatFormatDefinition
  {
    BOOST_TEST(formatFormatDefinition(FormatDefinition::UNDEFINED) ==
               "Undefined");
    BOOST_TEST(formatFormatDefinition(FormatDefinition::PCM) == "PCM");
  }
  // formatFormatDefinition
  {
    BOOST_TEST(formatFormatLabel(FormatDefinition::UNDEFINED) == "0000");
    BOOST_TEST(formatFormatLabel(FormatDefinition::PCM) == "0001");
  }
}
