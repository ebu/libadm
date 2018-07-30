/// @file format_descriptor.hpp
#pragma once

#include <string>
#include "adm/detail/named_type.hpp"
#include "adm/libadm_export.h"

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
    LIBADM_EXPORT extern const FormatDescriptor UNDEFINED;
    /// FormatDefinition for PCM
    LIBADM_EXPORT extern const FormatDescriptor PCM;
  }  // namespace FormatDefinition

  /// @brief Parse a formatLabel and convert it to a FormatDescriptor
  LIBADM_EXPORT FormatDescriptor parseFormatLabel(const std::string& label);
  /// @brief Parse a formatDefinition and convert it to a FormatDescriptor
  LIBADM_EXPORT FormatDescriptor
  parseFormatDefinition(const std::string& definition);
  /// @brief Format a FormatDescriptor object as a formatLabel string
  LIBADM_EXPORT std::string formatFormatLabel(FormatDescriptor desc);
  /// @brief Format a FormatDescriptor object as a formatDefinition string
  LIBADM_EXPORT std::string formatFormatDefinition(FormatDescriptor desc);

}  // namespace adm
