#pragma once
#include <string>
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"

namespace adm {
  /// Tag for LabelValue
  struct LabelValueTag {};
  /// NamedType for the LabelValue attribute
  using LabelValue = detail::NamedType<std::string, LabelValueTag>;

  /// Tag for LabelLanguage
  struct LabelLanguageTag {};
  /// NamedType for LabelLanguage attribute
  using LabelLanguage = detail::NamedType<std::string, LabelLanguageTag>;

  /// Tag for Label
  struct LabelTag {};

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<LabelValue>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<LabelLanguage>;

    using LabelBase = HasParameters<RequiredParameter<LabelValue>,
                                    OptionalParameter<LabelLanguage>>;
  }  // namespace detail

  /// Represents an ADM label element, with optional language attribute
  class Label : private detail::LabelBase,
                private detail::AddWrapperMethods<Label> {
   public:
    using tag = LabelTag;

    template <typename... Parameters>
    Label(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this,
                                   std::forward<Parameters>(namedArgs)...);
    }

    using detail::LabelBase::set;
    using detail::AddWrapperMethods<Label>::get;
    using detail::AddWrapperMethods<Label>::has;
    using detail::AddWrapperMethods<Label>::isDefault;
    using detail::AddWrapperMethods<Label>::unset;

   private:
    using detail::LabelBase::get;
    using detail::LabelBase::has;
    using detail::LabelBase::isDefault;
    using detail::LabelBase::unset;

    friend class detail::AddWrapperMethods<Label>;
  };
}  // namespace adm
