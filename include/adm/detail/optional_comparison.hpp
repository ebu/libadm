#pragma once
namespace adm {
  namespace detail {
    namespace comparison {
      template <typename ParameterT, typename ElementT>
      bool optionalParameterEqual(ElementT const& lhs, ElementT const& rhs) {
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

      template <typename ParameterT, typename ElementT>
      bool optionalParameterLess(ElementT const& lhs, ElementT const& rhs) {
        // if one has the parameter and the other doesn't, sort the one that doesn't first
        if (lhs.template has<ParameterT>() != rhs.template has<ParameterT>()) {
          return lhs.template has<ParameterT>() <
                 rhs.template has<ParameterT>();
        }
        // either both have it or neither does. If neither, they're equal.
        if (!lhs.template has<ParameterT>()) {
          return false;
        }
        // otherwise both have the parameter, so compare.
        return lhs.template get<ParameterT>() < rhs.template get<ParameterT>();
      }

      template <typename FirstParam, typename... Params>
      struct OptionalComparator {
        template <typename ElementT>
        static bool allEqual(ElementT const& lhs, ElementT const& rhs) {
          return (optionalParameterEqual<FirstParam>(lhs, rhs) &&
                  OptionalComparator<Params...>::allEqual(lhs, rhs));
        }
        template <typename ElementT>
        static bool allLess(ElementT const& lhs, ElementT const& rhs) {
          return optionalParameterEqual<FirstParam>(lhs, rhs)
                     ? OptionalComparator<Params...>::allLess(lhs, rhs)
                     : optionalParameterLess<FirstParam>(lhs, rhs);
        }
      };
      template <typename FirstParam>
      struct OptionalComparator<FirstParam> {
        template <typename ElementT>
        static bool allEqual(ElementT const& lhs, ElementT const& rhs) {
          return optionalParameterEqual<FirstParam>(lhs, rhs);
        }
        template <typename ElementT>
        static bool allLess(ElementT const& lhs, ElementT const& rhs) {
          return optionalParameterLess<FirstParam>(lhs, rhs);
        }
      };
    }  // namespace comparison
    template <typename... Params, typename ElementT>
    bool optionalsEqual(ElementT const& lhs, ElementT const& rhs) {
      return comparison::OptionalComparator<Params...>::allEqual(lhs, rhs);
    }

    template <typename... Params, typename ElementT>
    bool optionalsLess(ElementT const& lhs, ElementT const& rhs) {
      return comparison::OptionalComparator<Params...>::allLess(lhs, rhs);
    }
  }  // namespace detail
}  // namespace adm
