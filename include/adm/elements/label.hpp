#pragma once
#include <string>
#include <vector>
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/optional_comparison.hpp"
#include "adm/errors.hpp"

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
    explicit Label(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this,
                                   std::forward<Parameters>(namedArgs)...);
    }

    ADM_EXPORT explicit Label(std::string str)
        : Label(LabelValue(std::move(str))) {}
    ADM_EXPORT explicit Label(const char* s);

    ADM_EXPORT void print(std::ostream& os) const;

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

  /// Tag for Label
  struct LabelsTag {};
  /// Represents multiple labels with different languages
  using Labels = std::vector<Label>;
  ADD_TRAIT(Labels, LabelsTag);

  inline bool operator==(Label const& lhs, Label const& rhs) {
    return detail::compareOptionals<LabelValue, LabelLanguage>(lhs, rhs);
  }

  inline bool operator!=(Label const& lhs, Label const& rhs) {
    return !(lhs == rhs);
  }

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS VectorParameter<Labels>;
  }  // namespace detail
}  // namespace adm
