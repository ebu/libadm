#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/type_descriptor.hpp"

#define BOOST_TEST_MODULE TypeDescriptor
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(type_descriptor) {
  using namespace adm;

  // parseTypeLabel
  {
    BOOST_TEST(parseTypeLabel("0000") == TypeDefinition::UNDEFINED);
    BOOST_TEST(parseTypeLabel("0001") == TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(parseTypeLabel("0002") == TypeDefinition::MATRIX);
    BOOST_TEST(parseTypeLabel("0003") == TypeDefinition::OBJECTS);
    BOOST_TEST(parseTypeLabel("0004") == TypeDefinition::HOA);
    BOOST_TEST(parseTypeLabel("0005") == TypeDefinition::BINAURAL);
  }
  // parseTypeDefinition
  {
    BOOST_TEST(parseTypeDefinition("Undefined") == TypeDefinition::UNDEFINED);
    BOOST_TEST(parseTypeDefinition("DirectSpeakers") ==
                TypeDefinition::DIRECT_SPEAKERS);
    BOOST_TEST(parseTypeDefinition("Matrix") == TypeDefinition::MATRIX);
    BOOST_TEST(parseTypeDefinition("Objects") == TypeDefinition::OBJECTS);
    BOOST_TEST(parseTypeDefinition("HOA") == TypeDefinition::HOA);
    BOOST_TEST(parseTypeDefinition("Binaural") == TypeDefinition::BINAURAL);
  }
  // formatTypeDefinition
  {
    BOOST_TEST(formatTypeDefinition(TypeDefinition::UNDEFINED) == "Undefined");
    BOOST_TEST(formatTypeDefinition(TypeDefinition::DIRECT_SPEAKERS) ==
                "DirectSpeakers");
    BOOST_TEST(formatTypeDefinition(TypeDefinition::MATRIX) == "Matrix");
    BOOST_TEST(formatTypeDefinition(TypeDefinition::OBJECTS) == "Objects");
    BOOST_TEST(formatTypeDefinition(TypeDefinition::HOA) == "HOA");
    BOOST_TEST(formatTypeDefinition(TypeDefinition::BINAURAL) == "Binaural");
  }
  // formatTypeDefinition
  {
    BOOST_TEST(formatTypeLabel(TypeDefinition::UNDEFINED) == "0000");
    BOOST_TEST(formatTypeLabel(TypeDefinition::DIRECT_SPEAKERS) == "0001");
    BOOST_TEST(formatTypeLabel(TypeDefinition::MATRIX) == "0002");
    BOOST_TEST(formatTypeLabel(TypeDefinition::OBJECTS) == "0003");
    BOOST_TEST(formatTypeLabel(TypeDefinition::HOA) == "0004");
    BOOST_TEST(formatTypeLabel(TypeDefinition::BINAURAL) == "0005");
  }
}
