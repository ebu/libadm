#pragma once

#include "adm/detail/named_type.hpp"
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"

namespace adm {
  /// @brief Tag for NamedType ::Bypass
  struct BypassTag {};
  /// @brief NamedType for bypass parameter
  using Bypass = detail::NamedType<bool, BypassTag>;
  /// @brief Tag for NamedType ::DirectToReverberantRatio
  struct DirectToReverberantRatioTag {};
  /// @brief NamedType for DRR parameter
  using DirectToReverberantRatio =
      detail::NamedType<float, DirectToReverberantRatioTag,
                        detail::RangeValidator<-130, 130>>;

  namespace detail {
    /// @brief default value for DirectToReverberantRatio is +130dB
    template <>
    inline DirectToReverberantRatio getDefault<DirectToReverberantRatio>() {
      return DirectToReverberantRatio(130);
    }

    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<Bypass>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        DefaultParameter<DirectToReverberantRatio>;

    using HeadphoneVirtualiseBase =
        HasParameters<DefaultParameter<Bypass>,
                      DefaultParameter<DirectToReverberantRatio>>;
  }  // namespace detail

  struct HeadphoneVirtualiseTag {};

  class HeadphoneVirtualise : private detail::HeadphoneVirtualiseBase {
   public:
    typedef HeadphoneVirtualiseTag tag;

    template <typename... Parameters>
    explicit HeadphoneVirtualise(Parameters... optionalNamedArgs);

    using detail::HeadphoneVirtualiseBase::get;
    using detail::HeadphoneVirtualiseBase::has;
    using detail::HeadphoneVirtualiseBase::isDefault;
    using detail::HeadphoneVirtualiseBase::set;
    using detail::HeadphoneVirtualiseBase::unset;

    /**
     * @brief ADM parameter get template
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template
     *
     * Templated has method with the ADM parameter type as template
     * argument. Returns true if the parameter has been set or is optional.
     */
    template <typename Parameter>
    bool has() const;

    /**
     * @brief ADM parameter isDefault template
     *
     * Templated isDefault method with the ADM parameter type as template
     * argument. Returns true if the optional parameter has been changed from
     * its default value.
     */
    template <typename Parameter>
    bool isDefault() const;

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();
  };

  // Implementation
  template <typename Parameter>
  Parameter HeadphoneVirtualise::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool HeadphoneVirtualise::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool HeadphoneVirtualise::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename Parameter>
  void HeadphoneVirtualise::unset() {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return unset(Tag());
  }

  template <typename... Parameters>
  HeadphoneVirtualise::HeadphoneVirtualise(Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }
}  // namespace adm