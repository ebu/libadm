/// @file authoring_information.hpp
#pragma once

#include <memory>
#include <ostream>
#include <vector>

#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/optional_comparison.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/elements/renderer_common_types.hpp"
#include "adm/export.h"
#include "adm/helper/element_range.hpp"

namespace adm {

  class Document;
  class LoudnessRenderer;

  /// @brief Tag for ::ReferenceLayout named-type
  struct ReferenceLayoutTag {};
  /**
   * @brief Reference to audioPackFormat used by a referenceLayout
   * sub-element of authoringInformation (BS.2076-3 Table A1-51).
   */
  class ReferenceLayout {
   public:
    using tag = ReferenceLayoutTag;

    ReferenceLayout() = default;
    explicit ReferenceLayout(std::shared_ptr<AudioPackFormat> packFormat)
        : packFormat_(std::move(packFormat)) {}

    std::shared_ptr<AudioPackFormat> const &get() const { return packFormat_; }

   private:
    std::shared_ptr<AudioPackFormat> packFormat_;
  };

  ADD_TRAIT(ReferenceLayout, ReferenceLayoutTag);

  inline bool operator==(ReferenceLayout const &lhs,
                         ReferenceLayout const &rhs) {
    return lhs.get() == rhs.get();
  }
  inline bool operator!=(ReferenceLayout const &lhs,
                         ReferenceLayout const &rhs) {
    return !(lhs == rhs);
  }

  /// @brief Vector of ReferenceLayout
  using ReferenceLayouts = std::vector<ReferenceLayout>;
  ADD_TRAIT(ReferenceLayouts, ReferenceLayoutsTag);

  /// @brief Tag for Renderer class
  struct RendererTag {};

  namespace detail {
    using RendererBase = HasParameters<
        RequiredParameter<RendererUri>, OptionalParameter<RendererName>,
        OptionalParameter<RendererVersion>, OptionalParameter<CoordinateMode>>;
  }  // namespace detail

  /**
   * @brief Class representation of the renderer sub-element of an
   * authoringInformation element (BS.2076-3 Tables A1-52 / A1-53).
   */
  class AuthoringRenderer
      : private detail::RendererBase,
        private detail::AddWrapperMethods<AuthoringRenderer> {
   public:
    using tag = RendererTag;

    template <typename... Parameters>
    explicit AuthoringRenderer(RendererUri uri, Parameters... namedArgs) {
      this->set(std::move(uri));
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::RendererBase::set;
    using detail::AddWrapperMethods<AuthoringRenderer>::get;
    using detail::AddWrapperMethods<AuthoringRenderer>::has;
    using detail::AddWrapperMethods<AuthoringRenderer>::isDefault;
    using detail::AddWrapperMethods<AuthoringRenderer>::unset;

    /// @brief Add reference to an AudioPackFormat
    ADM_EXPORT bool addReference(std::shared_ptr<AudioPackFormat> packFormat);

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

    /// @brief Remove reference to an AudioPackFormat
    ADM_EXPORT void removeReference(
        std::shared_ptr<AudioPackFormat> packFormat);

    /**
     * @brief Clear references to elements template
     *
     * Removes all references to ADM elements with the specified type.
     */
    template <typename Element>
    void clearReferences();

    /// @brief Convert to LoudnessRenderer preserving all shared parameters.
    ADM_EXPORT LoudnessRenderer toLoudnessRenderer() const;

    ADM_EXPORT void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT const std::weak_ptr<Document> &getParent() const;

   private:
    using detail::RendererBase::get;
    using detail::RendererBase::has;
    using detail::RendererBase::isDefault;
    using detail::RendererBase::unset;

    ADM_EXPORT ElementRange<AudioPackFormat> getReferences(
        detail::ParameterTraits<AudioPackFormat>::tag);
    ADM_EXPORT ElementRange<const AudioPackFormat> getReferences(
        detail::ParameterTraits<AudioPackFormat>::tag) const;
    ADM_EXPORT void clearReferences(
        detail::ParameterTraits<AudioPackFormat>::tag);

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    friend class AuthoringInformation;
    friend class detail::AddWrapperMethods<AuthoringRenderer>;

    std::weak_ptr<Document> parent_;
    std::vector<std::shared_ptr<AudioPackFormat>> packFormats_;
  };

  ADD_TRAIT(AuthoringRenderer, RendererTag);

  inline bool operator==(const AuthoringRenderer &a,
                         const AuthoringRenderer &b) {
    if (!detail::optionalsEqual<RendererUri, RendererName, RendererVersion,
                                CoordinateMode>(a, b)) {
      return false;
    }

    return detail::elementRangeEqual<AudioPackFormat const>(
        a.getReferences<AudioPackFormat>(), b.getReferences<AudioPackFormat>());
  }
  inline bool operator!=(const AuthoringRenderer &a,
                         const AuthoringRenderer &b) {
    return !(a == b);
  }

  /// @brief Vector of Renderer
  using Renderers = std::vector<AuthoringRenderer>;
  ADD_TRAIT(Renderers, RenderersTag);

  /// @brief Tag for AuthoringInformation class
  struct AuthoringInformationTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        VectorParameter<ReferenceLayouts>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        VectorParameter<Renderers>;

    using AuthoringInformationBase =
        HasParameters<VectorParameter<ReferenceLayouts>,
                      VectorParameter<Renderers>>;
  }  // namespace detail

  /**
   * @brief Class representation of the authoringInformation sub-element of an
   * audioProgramme element (BS.2076-3 §5.8.6).
   */
  class AuthoringInformation
      : private detail::AuthoringInformationBase,
        private detail::AddWrapperMethods<AuthoringInformation> {
   public:
    using tag = AuthoringInformationTag;

    template <typename... Parameters>
    explicit AuthoringInformation(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::AuthoringInformationBase::add;
    using detail::AuthoringInformationBase::remove;
    using detail::AuthoringInformationBase::set;
    using detail::AddWrapperMethods<AuthoringInformation>::get;
    using detail::AddWrapperMethods<AuthoringInformation>::has;
    using detail::AddWrapperMethods<AuthoringInformation>::isDefault;
    using detail::AddWrapperMethods<AuthoringInformation>::unset;

    /// @brief Add an AuthoringRenderer
    ADM_EXPORT bool add(AuthoringRenderer renderer);
    /// @brief Set AuthoringRenderers
    ADM_EXPORT void set(Renderers renderers);

    /// Get adm::Document this element belongs to
    ADM_EXPORT const std::weak_ptr<Document> &getParent() const;

    ADM_EXPORT void print(std::ostream &os) const;

   private:
    using detail::AuthoringInformationBase::get;
    using detail::AuthoringInformationBase::has;
    using detail::AuthoringInformationBase::isDefault;
    using detail::AuthoringInformationBase::unset;

    ADM_EXPORT void setParent(std::weak_ptr<Document> document);

    friend class AudioProgramme;
    friend class detail::AddWrapperMethods<AuthoringInformation>;

    std::weak_ptr<Document> parent_;
  };

  ADD_TRAIT(AuthoringInformation, AuthoringInformationTag);

  template <typename Element>
  ElementRange<Element> AuthoringRenderer::getReferences() {
    using Tag = typename detail::ParameterTraits<Element>::tag;
    return getReferences(Tag());
  }

  template <typename Element>
  ElementRange<const Element> AuthoringRenderer::getReferences() const {
    using Tag = typename detail::ParameterTraits<Element>::tag;
    return getReferences(Tag());
  }

  template <typename Element>
  void AuthoringRenderer::clearReferences() {
    using Tag = typename detail::ParameterTraits<Element>::tag;
    clearReferences(Tag());
  }

}  // namespace adm
