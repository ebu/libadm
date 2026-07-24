/// @file renderer_common_types.hpp
#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

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

  /// @brief Vector of audioPackFormat references used by a renderer
  using RendererPackFormatIdRefs =
      std::vector<std::shared_ptr<AudioPackFormat>>;
  ADD_TRAIT(RendererPackFormatIdRefs, RendererPackFormatIdRefsTag);

  /// @brief Single audioPackFormat reference used by a loudness renderer
  using RendererPackFormatIdRef = std::shared_ptr<AudioPackFormat>;
  ADD_TRAIT(RendererPackFormatIdRef, RendererPackFormatIdRefTag);

  /// @brief Vector of audioObject references used by a renderer
  using RendererObjectIdRefs = std::vector<std::shared_ptr<AudioObject>>;
  ADD_TRAIT(RendererObjectIdRefs, RendererObjectIdRefsTag);

  namespace detail {
    template <>
    struct ParameterCompare<RendererPackFormatIdRefs> {
      static bool compare(RendererPackFormatIdRefs const& lhs,
                          RendererPackFormatIdRefs const& rhs) {
        return lhs == rhs;
      }
    };

    template <>
    struct ParameterCompare<RendererObjectIdRefs> {
      static bool compare(RendererObjectIdRefs const& lhs,
                          RendererObjectIdRefs const& rhs) {
        return lhs == rhs;
      }
    };
  }  // namespace detail

}  // namespace adm
