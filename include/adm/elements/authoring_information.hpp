/// @file authoring_information.hpp
#pragma once

#include <ostream>
#include <vector>

#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/optional_comparison.hpp"
#include "adm/elements/renderer_common_types.hpp"
#include "adm/export.h"

namespace adm {

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
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        VectorParameter<RendererPackFormatIdRefs>;

    using RendererBase = HasParameters<
        RequiredParameter<RendererUri>, OptionalParameter<RendererName>,
        OptionalParameter<RendererVersion>, OptionalParameter<CoordinateMode>,
        VectorParameter<RendererPackFormatIdRefs>>;
  }  // namespace detail

  /**
   * @brief Class representation of the renderer sub-element of an
   * authoringInformation element (BS.2076-3 Tables A1-52 / A1-53).
   */
  class Renderer : private detail::RendererBase,
                   private detail::AddWrapperMethods<Renderer> {
   public:
    using tag = RendererTag;

    template <typename... Parameters>
    explicit Renderer(RendererUri uri, Parameters... namedArgs) {
      this->set(std::move(uri));
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::RendererBase::add;
    using detail::RendererBase::remove;
    using detail::RendererBase::set;
    using detail::AddWrapperMethods<Renderer>::get;
    using detail::AddWrapperMethods<Renderer>::has;
    using detail::AddWrapperMethods<Renderer>::isDefault;
    using detail::AddWrapperMethods<Renderer>::unset;

    /// @brief Convert to LoudnessRenderer preserving all shared parameters.
    ADM_EXPORT LoudnessRenderer toLoudnessRenderer() const;

    ADM_EXPORT void print(std::ostream &os) const;

   private:
    using detail::RendererBase::get;
    using detail::RendererBase::has;
    using detail::RendererBase::isDefault;
    using detail::RendererBase::unset;

    friend class detail::AddWrapperMethods<Renderer>;
  };

  ADD_TRAIT(Renderer, RendererTag);

  inline bool operator==(const Renderer &a, const Renderer &b) {
    if (!detail::optionalsEqual<RendererUri, RendererName, RendererVersion,
                                CoordinateMode>(a, b)) {
      return false;
    }

    if (a.has<RendererPackFormatIdRefs>() !=
        b.has<RendererPackFormatIdRefs>()) {
      return false;
    }

    if (!a.has<RendererPackFormatIdRefs>()) {
      return true;
    }

    return a.get<RendererPackFormatIdRefs>() ==
           b.get<RendererPackFormatIdRefs>();
  }
  inline bool operator!=(const Renderer &a, const Renderer &b) {
    return !(a == b);
  }

  /// @brief Vector of Renderer
  using Renderers = std::vector<Renderer>;
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

    ADM_EXPORT void print(std::ostream &os) const;

   private:
    using detail::AuthoringInformationBase::get;
    using detail::AuthoringInformationBase::has;
    using detail::AuthoringInformationBase::isDefault;
    using detail::AuthoringInformationBase::unset;

    friend class detail::AddWrapperMethods<AuthoringInformation>;
  };

  ADD_TRAIT(AuthoringInformation, AuthoringInformationTag);

}  // namespace adm
