/// @file loudness_renderer.hpp
#pragma once

#include <memory>
#include <ostream>
#include <vector>

#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/elements/renderer_common_types.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/export.h"
#include "adm/helper/element_range.hpp"

namespace adm {

  class AuthoringRenderer;

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
        OptionalParameter<RendererPackFormatIdRef>;

    using LoudnessRendererBase = HasParameters<
        OptionalParameter<RendererUri>, OptionalParameter<RendererName>,
        OptionalParameter<RendererVersion>, OptionalParameter<CoordinateMode>,
        OptionalParameter<RendererPackFormatIdRef>>;
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

    using detail::LoudnessRendererBase::set;
    using detail::AddWrapperMethods<LoudnessRenderer>::get;
    using detail::AddWrapperMethods<LoudnessRenderer>::has;
    using detail::AddWrapperMethods<LoudnessRenderer>::isDefault;
    using detail::AddWrapperMethods<LoudnessRenderer>::unset;

    /// @brief Add reference to an AudioObject
    ADM_EXPORT bool addReference(std::shared_ptr<AudioObject> object);

    /**
     * @brief Get references to ADM elements template
     *
     * Templated getter with the wanted ADM element type as template
     * argument.
     */
    template <typename Element>
    ElementRange<Element> getReferences();

    /**
     * @brief Get references to ADM elements template
     *
     * Templated getter with the wanted ADM element type as template
     * argument.
     */
    template <typename Element>
    ElementRange<const Element> getReferences() const;

    /// @brief Remove reference to an AudioObject
    ADM_EXPORT void removeReference(std::shared_ptr<AudioObject> object);

    /**
     * @brief Clear references to elements template
     *
     * Removes all references to ADM elements with the specified type.
     */
    template <typename Element>
    void clearReferences();

    /// @brief Create a LoudnessRenderer from an authoring Renderer.
    ADM_EXPORT static LoudnessRenderer fromRenderer(
        AuthoringRenderer const& renderer);

    /// @brief Convert to Renderer, using provided uri and explicitly dropping audioObjectIDRef values.
    /// uri is required as it is optional in a Loudness renderer but required in authoring renderer
    ADM_EXPORT AuthoringRenderer
    toRendererDroppingObjectRefs(RendererUri uri) const;

    ADM_EXPORT void print(std::ostream& os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT const std::weak_ptr<Document>& getParent() const;

   private:
    using detail::LoudnessRendererBase::get;
    using detail::LoudnessRendererBase::has;
    using detail::LoudnessRendererBase::isDefault;
    using detail::LoudnessRendererBase::unset;

    ADM_EXPORT ElementRange<AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag);
    ADM_EXPORT ElementRange<const AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag) const;
    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioObject>::tag);

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    friend class LoudnessMetadata;
    friend class detail::AddWrapperMethods<LoudnessRenderer>;

    std::weak_ptr<Document> parent_;
    std::vector<std::shared_ptr<AudioObject>> audioObjects_;
  };

  ADD_TRAIT(LoudnessRenderer, LoudnessRendererTag);

  template <typename Element>
  ElementRange<Element> LoudnessRenderer::getReferences() {
    using Tag = typename detail::ParameterTraits<Element>::tag;
    return getReferences(Tag());
  }

  template <typename Element>
  ElementRange<const Element> LoudnessRenderer::getReferences() const {
    using Tag = typename detail::ParameterTraits<Element>::tag;
    return getReferences(Tag());
  }

  template <typename Element>
  void LoudnessRenderer::clearReferences() {
    using Tag = typename detail::ParameterTraits<Element>::tag;
    clearReferences(Tag());
  }

}  // namespace adm
