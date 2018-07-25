#include <catch2/catch.hpp>
#include "adm/elements/type_descriptor.hpp"

TEST_CASE("type_descriptor") {
  using namespace adm;

  // parseTypeLabel
  {
    REQUIRE(parseTypeLabel("0000") == TypeDefinition::UNDEFINED);
    REQUIRE(parseTypeLabel("0001") == TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(parseTypeLabel("0002") == TypeDefinition::MATRIX);
    REQUIRE(parseTypeLabel("0003") == TypeDefinition::OBJECTS);
    REQUIRE(parseTypeLabel("0004") == TypeDefinition::HOA);
    REQUIRE(parseTypeLabel("0005") == TypeDefinition::BINAURAL);
  }
  // parseTypeDefinition
  {
    REQUIRE(parseTypeDefinition("Undefined") == TypeDefinition::UNDEFINED);
    REQUIRE(parseTypeDefinition("DirectSpeakers") ==
            TypeDefinition::DIRECT_SPEAKERS);
    REQUIRE(parseTypeDefinition("Matrix") == TypeDefinition::MATRIX);
    REQUIRE(parseTypeDefinition("Objects") == TypeDefinition::OBJECTS);
    REQUIRE(parseTypeDefinition("HOA") == TypeDefinition::HOA);
    REQUIRE(parseTypeDefinition("Binaural") == TypeDefinition::BINAURAL);
  }
  // formatTypeDefinition
  {
    REQUIRE(formatTypeDefinition(TypeDefinition::UNDEFINED) == "Undefined");
    REQUIRE(formatTypeDefinition(TypeDefinition::DIRECT_SPEAKERS) ==
            "DirectSpeakers");
    REQUIRE(formatTypeDefinition(TypeDefinition::MATRIX) == "Matrix");
    REQUIRE(formatTypeDefinition(TypeDefinition::OBJECTS) == "Objects");
    REQUIRE(formatTypeDefinition(TypeDefinition::HOA) == "HOA");
    REQUIRE(formatTypeDefinition(TypeDefinition::BINAURAL) == "Binaural");
  }
  // formatTypeDefinition
  {
    REQUIRE(formatTypeLabel(TypeDefinition::UNDEFINED) == "0000");
    REQUIRE(formatTypeLabel(TypeDefinition::DIRECT_SPEAKERS) == "0001");
    REQUIRE(formatTypeLabel(TypeDefinition::MATRIX) == "0002");
    REQUIRE(formatTypeLabel(TypeDefinition::OBJECTS) == "0003");
    REQUIRE(formatTypeLabel(TypeDefinition::HOA) == "0004");
    REQUIRE(formatTypeLabel(TypeDefinition::BINAURAL) == "0005");
  }
}
