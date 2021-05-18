#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"

namespace adm {
  struct XOffsetTag {};
  using XOffset =
      detail::NamedType<float, XOffsetTag, detail::RangeValidator<-1, 1>>;

  struct YOffsetTag {};
  using YOffset =
      detail::NamedType<float, YOffsetTag, detail::RangeValidator<-1, 1>>;

  struct ZOffsetTag {};
  using ZOffset =
      detail::NamedType<float, ZOffsetTag, detail::RangeValidator<-1, 1>>;
  namespace detail {
    // Explicit defaults are set per NamedType in the detail namespace. This
    // isn't necessary here because the default value for number types is 0.
    template <>
    inline YOffset getDefault<YOffset>() {
      return YOffset{0.0f};
    }
  }  // namespace detail

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<XOffset>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<YOffset>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS DefaultParameter<ZOffset>;

    using CartesianPositionOffsetBase =
        HasParameters<DefaultParameter<XOffset>, DefaultParameter<YOffset>,
                      DefaultParameter<ZOffset>>;
  }  // namespace detail

  struct CartesianPositionOffsetTag {};

  class CartesianPositionOffset : private detail::CartesianPositionOffsetBase {
   public:
    typedef CartesianPositionOffsetTag tag;

    template <typename... Parameters>
    CartesianPositionOffset(Parameters... optionalNamedArgs);

    using detail::CartesianPositionOffsetBase::get;
    using detail::CartesianPositionOffsetBase::has;
    using detail::CartesianPositionOffsetBase::isDefault;
    using detail::CartesianPositionOffsetBase::set;

    template <typename Parameter>
    Parameter get() const;

    template <typename Parameter>
    bool has() const;

    template <typename Parameter>
    bool isDefault() const;
  };

  template <typename Parameter>
  Parameter CartesianPositionOffset::get() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return get(Tag());
  }

  template <typename Parameter>
  bool CartesianPositionOffset::has() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return has(Tag());
  }

  template <typename Parameter>
  bool CartesianPositionOffset::isDefault() const {
    typedef typename detail::ParameterTraits<Parameter>::tag Tag;
    return isDefault(Tag());
  }

  template <typename... Parameters>
  CartesianPositionOffset::CartesianPositionOffset(
      Parameters... optionalNamedArgs) {
    detail::setNamedOptionHelper(
        this, std::forward<Parameters>(optionalNamedArgs)...);
  }
}  // namespace adm