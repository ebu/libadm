/// @file type_descriptor.hpp
#pragma once

#include <string>
#include "adm/detail/named_type.hpp"
#include "adm/libadm_export.h"

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
    LIBADM_EXPORT extern const TypeDescriptor UNDEFINED;
    /// TypeDefinition for Direct Speakers
    LIBADM_EXPORT extern const TypeDescriptor DIRECT_SPEAKERS;
    /// TypeDefinition for Matrix
    LIBADM_EXPORT extern const TypeDescriptor MATRIX;
    /// TypeDefinition for Objects
    LIBADM_EXPORT extern const TypeDescriptor OBJECTS;
    /// TypeDefinition for Higher Order Ambisonics
    LIBADM_EXPORT extern const TypeDescriptor HOA;
    /// TypeDefinition for Binaural
    LIBADM_EXPORT extern const TypeDescriptor BINAURAL;
  }  // namespace TypeDefinition

  /// @brief Parse a typeLabel and convert it to a TypeDescriptor
  LIBADM_EXPORT TypeDescriptor parseTypeLabel(const std::string& label);
  /// @brief Parse a typeDefinition and convert it to a TypeDescriptor
  LIBADM_EXPORT TypeDescriptor
  parseTypeDefinition(const std::string& definition);
  /// @brief Format a TypeDescriptor object as a typeLabel string
  LIBADM_EXPORT std::string formatTypeLabel(TypeDescriptor desc);
  /// @brief Format a TypeDescriptor object as a typeDefinition string
  LIBADM_EXPORT std::string formatTypeDefinition(TypeDescriptor desc);

}  // namespace adm
