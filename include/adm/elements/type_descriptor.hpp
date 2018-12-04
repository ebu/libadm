/// @file type_descriptor.hpp
#pragma once

#include <string>
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::TypeDescriptor
  struct TypeDescriptorTag {};

  /**
   * @brief Combined ADM typeLabel and typeDefinition.
   *
   * Valid values are in the range [0, 5]
   *
   * @sa TypeDefinition A list of pre-defined type definitions
   */
  using TypeDescriptor =
      detail::NamedType<int, TypeDescriptorTag, detail::RangeValidator<0, 5>>;

  /// BS.2076 type definitions
  namespace TypeDefinition {
    ADM_EXPORT extern const TypeDescriptor UNDEFINED;
    /// TypeDefinition for Direct Speakers
    ADM_EXPORT extern const TypeDescriptor DIRECT_SPEAKERS;
    /// TypeDefinition for Matrix
    ADM_EXPORT extern const TypeDescriptor MATRIX;
    /// TypeDefinition for Objects
    ADM_EXPORT extern const TypeDescriptor OBJECTS;
    /// TypeDefinition for Higher Order Ambisonics
    ADM_EXPORT extern const TypeDescriptor HOA;
    /// TypeDefinition for Binaural
    ADM_EXPORT extern const TypeDescriptor BINAURAL;
  }  // namespace TypeDefinition

  /// @brief Parse a typeLabel and convert it to a TypeDescriptor
  ADM_EXPORT TypeDescriptor parseTypeLabel(const std::string& label);
  /// @brief Parse a typeDefinition and convert it to a TypeDescriptor
  ADM_EXPORT TypeDescriptor parseTypeDefinition(const std::string& definition);
  /// @brief Format a TypeDescriptor object as a typeLabel string
  ADM_EXPORT std::string formatTypeLabel(TypeDescriptor desc);
  /// @brief Format a TypeDescriptor object as a typeDefinition string
  ADM_EXPORT std::string formatTypeDefinition(TypeDescriptor desc);

}  // namespace adm
