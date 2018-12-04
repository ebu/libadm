/// @file format_descriptor.hpp
#pragma once

#include <string>
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::FormatDescriptor
  struct FormatDescriptorTag {};

  ///
  /**
   * @brief Combined ADM formatLabel and formatDefinition.
   *
   * Valid values are in the range [0, 1]

   * @sa FormatDefinition A list of pre-defined format definitions
   */
  using FormatDescriptor =
      detail::NamedType<int, FormatDescriptorTag, detail::RangeValidator<0, 1>>;

  /// BS.2076 format definitions
  namespace FormatDefinition {
    ADM_EXPORT extern const FormatDescriptor UNDEFINED;
    /// FormatDefinition for PCM
    ADM_EXPORT extern const FormatDescriptor PCM;
  }  // namespace FormatDefinition

  /// @brief Parse a formatLabel and convert it to a FormatDescriptor
  ADM_EXPORT FormatDescriptor parseFormatLabel(const std::string& label);
  /// @brief Parse a formatDefinition and convert it to a FormatDescriptor
  ADM_EXPORT FormatDescriptor
  parseFormatDefinition(const std::string& definition);
  /// @brief Format a FormatDescriptor object as a formatLabel string
  ADM_EXPORT std::string formatFormatLabel(FormatDescriptor desc);
  /// @brief Format a FormatDescriptor object as a formatDefinition string
  ADM_EXPORT std::string formatFormatDefinition(FormatDescriptor desc);

}  // namespace adm
