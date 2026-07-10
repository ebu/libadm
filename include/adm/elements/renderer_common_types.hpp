/// @file renderer_common_types.hpp
#pragma once

#include <string>
#include <vector>

#include "adm/detail/named_type.hpp"
#include "adm/elements/audio_object_id.hpp"
#include "adm/elements/audio_pack_format_id.hpp"
#include "adm/elements/coordinate_mode.hpp"

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

  /// @brief Vector of audioPackFormatIDRef values used by a renderer
  using RendererPackFormatIdRefs = std::vector<AudioPackFormatId>;
  ADD_TRAIT(RendererPackFormatIdRefs, RendererPackFormatIdRefsTag);

  /// @brief Vector of audioObjectIDRef values used by a renderer
  using RendererObjectIdRefs = std::vector<AudioObjectId>;
  ADD_TRAIT(RendererObjectIdRefs, RendererObjectIdRefsTag);

}  // namespace adm
