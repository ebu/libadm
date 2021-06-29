#pragma once
namespace adm {
  namespace detail {
    namespace comparison {
      template <typename ParameterT, typename ElementT>
      bool compareOptionalParameter(ElementT const& lhs, ElementT const& rhs) {
        // if one has the parameter and the other doesn't they're not equal
        if (lhs.template has<ParameterT>() != rhs.template has<ParameterT>()) {
          return false;
        }
        // either both have it or neither does. If neither, they're equal.
        if (!lhs.template has<ParameterT>()) {
          return true;
        }
        // otherwise both have the parameter, so compare.
        return lhs.template get<ParameterT>() == rhs.template get<ParameterT>();
      }

      template <typename FirstParam, typename... Params>
      struct OptionalComparator {
        template <typename ElementT>
        static bool compareAll(ElementT const& lhs, ElementT const& rhs) {
          return (compareOptionalParameter<FirstParam>(lhs, rhs) &&
                  OptionalComparator<Params...>::compareAll(lhs, rhs));
        }
      };
      template <typename FirstParam>
      struct OptionalComparator<FirstParam> {
        template <typename ElementT>
        static bool compareAll(ElementT const& lhs, ElementT const& rhs) {
          return compareOptionalParameter<FirstParam>(lhs, rhs);
        }
      };
    }  // namespace comparison
    template <typename... Params, typename ElementT>
    bool compareOptionals(ElementT const& lhs, ElementT const& rhs) {
      return comparison::OptionalComparator<Params...>::compareAll(lhs, rhs);
    }
  }  // namespace detail
}  // namespace adm
