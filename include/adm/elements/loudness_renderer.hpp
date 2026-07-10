/// @file loudness_renderer.hpp
#pragma once

#include <ostream>

#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/elements/renderer_common_types.hpp"
#include "adm/export.h"

namespace adm {

  class Renderer;

  /// @brief Tag for LoudnessRenderer class
  struct LoudnessRendererTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<RendererUri>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<RendererName>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<RendererVersion>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<CoordinateMode>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        VectorParameter<RendererPackFormatIdRefs>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        VectorParameter<RendererObjectIdRefs>;

    using LoudnessRendererBase = HasParameters<
        OptionalParameter<RendererUri>, OptionalParameter<RendererName>,
        OptionalParameter<RendererVersion>, OptionalParameter<CoordinateMode>,
        VectorParameter<RendererPackFormatIdRefs>,
        VectorParameter<RendererObjectIdRefs>>;
  }  // namespace detail

  /**
   * @brief Class representation of the renderer sub-element of a
   * loudnessMetadata element (BS.2076-3 Tables A1-39 / A1-40).
   */
  class LoudnessRenderer : private detail::LoudnessRendererBase,
                           private detail::AddWrapperMethods<LoudnessRenderer> {
   public:
    using tag = LoudnessRendererTag;

    template <typename... Parameters>
    explicit LoudnessRenderer(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::LoudnessRendererBase::add;
    using detail::LoudnessRendererBase::remove;
    using detail::LoudnessRendererBase::set;
    using detail::AddWrapperMethods<LoudnessRenderer>::get;
    using detail::AddWrapperMethods<LoudnessRenderer>::has;
    using detail::AddWrapperMethods<LoudnessRenderer>::isDefault;
    using detail::AddWrapperMethods<LoudnessRenderer>::unset;

    /// @brief Create a LoudnessRenderer from an authoring Renderer.
    ADM_EXPORT static LoudnessRenderer fromRenderer(Renderer const& renderer);

    /// @brief Convert to Renderer, using provided uri and explicitly dropping audioObjectIDRef values.
    /// uri is required as it is optional in a Loudness renderer but required in authoring renderer
    ADM_EXPORT Renderer toRendererDroppingObjectRefs(RendererUri uri) const;

    ADM_EXPORT void print(std::ostream& os) const;

   private:
    using detail::LoudnessRendererBase::get;
    using detail::LoudnessRendererBase::has;
    using detail::LoudnessRendererBase::isDefault;
    using detail::LoudnessRendererBase::unset;

    friend class detail::AddWrapperMethods<LoudnessRenderer>;
  };

  ADD_TRAIT(LoudnessRenderer, LoudnessRendererTag);

}  // namespace adm
