#include "adm/elements/type_descriptor.hpp"
#include <sstream>
#include <regex>
#include <boost/format.hpp>

namespace adm {

  namespace TypeDefinition {
    extern const TypeDescriptor UNDEFINED = TypeDescriptor(0);
    extern const TypeDescriptor DIRECT_SPEAKERS = TypeDescriptor(1);
    extern const TypeDescriptor MATRIX = TypeDescriptor(2);
    extern const TypeDescriptor OBJECTS = TypeDescriptor(3);
    extern const TypeDescriptor HOA = TypeDescriptor(4);
    extern const TypeDescriptor BINAURAL = TypeDescriptor(5);
  }  // namespace TypeDefinition

  TypeDescriptor parseTypeLabel(const std::string& label) {
    if (label == "0000") {
      return TypeDefinition::UNDEFINED;
    } else if (label == "0001") {
      return TypeDefinition::DIRECT_SPEAKERS;
    } else if (label == "0002") {
      return TypeDefinition::MATRIX;
    } else if (label == "0003") {
      return TypeDefinition::OBJECTS;
    } else if (label == "0004") {
      return TypeDefinition::HOA;
    } else if (label == "0005") {
      return TypeDefinition::BINAURAL;
    } else {
      std::stringstream errorString;
      errorString << "invalid typeDefinition: " << label;
      throw std::runtime_error(errorString.str());
    }
  }

  TypeDescriptor parseTypeDefinition(const std::string& definition) {
    if (definition == "Undefined") {
      return TypeDefinition::UNDEFINED;
    } else if (definition == "DirectSpeakers") {
      return TypeDefinition::DIRECT_SPEAKERS;
    } else if (definition == "Matrix") {
      return TypeDefinition::MATRIX;
    } else if (definition == "Objects") {
      return TypeDefinition::OBJECTS;
    } else if (definition == "HOA") {
      return TypeDefinition::HOA;
    } else if (definition == "Binaural") {
      return TypeDefinition::BINAURAL;
    } else {
      std::stringstream errorString;
      errorString << "invalid typeDefinition: " << definition;
      throw std::runtime_error(errorString.str());
    }
  }

  std::string formatTypeLabel(TypeDescriptor desc) {
    if (desc == TypeDefinition::UNDEFINED) {
      return "0000";
    } else if (desc == TypeDefinition::DIRECT_SPEAKERS) {
      return "0001";
    } else if (desc == TypeDefinition::MATRIX) {
      return "0002";
    } else if (desc == TypeDefinition::OBJECTS) {
      return "0003";
    } else if (desc == TypeDefinition::HOA) {
      return "0004";
    } else if (desc == TypeDefinition::BINAURAL) {
      return "0005";
    } else {
      assert(0);
      return "unknown type descriptor";
    }
  }

  std::string formatTypeDefinition(TypeDescriptor desc) {
    if (desc == TypeDefinition::UNDEFINED) {
      return "Undefined";
    } else if (desc == TypeDefinition::DIRECT_SPEAKERS) {
      return "DirectSpeakers";
    } else if (desc == TypeDefinition::MATRIX) {
      return "Matrix";
    } else if (desc == TypeDefinition::OBJECTS) {
      return "Objects";
    } else if (desc == TypeDefinition::HOA) {
      return "HOA";
    } else if (desc == TypeDefinition::BINAURAL) {
      return "Binaural";
    } else {
      assert(0);
      return "unknown type descriptor";
    }
  }
}  // namespace adm
