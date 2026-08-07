/// @file renderer_common_types.hpp
#pragma once

#include <memory>
#include <string>

#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/elements/coordinate_mode.hpp"
#include "adm/elements_fwd.hpp"

namespace adm {

  /// @brief Tag for NamedType ::RendererUri
  struct RendererUriTag {};
  /// @brief NamedType for the renderer uri attribute
  using RendererUri = detail::NamedType<std::string, RendererUriTag>;

  /// @brief Tag for NamedType ::RendererName
  struct RendererNameTag {};
  /// @brief NamedType for the renderer name attribute
  using RendererName = detail::NamedType<std::string, RendererNameTag>;

  /// @brief Tag for NamedType ::RendererVersion
  struct RendererVersionTag {};
  /// @brief NamedType for the renderer version attribute
  using RendererVersion = detail::NamedType<std::string, RendererVersionTag>;

  /// @brief Single audioPackFormat reference used by a loudness renderer
  using RendererPackFormatIdRef = std::shared_ptr<AudioPackFormat>;
  ADD_TRAIT(RendererPackFormatIdRef, RendererPackFormatIdRefTag);

}  // namespace adm
