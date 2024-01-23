#pragma once

#include <boost/variant/variant_fwd.hpp>

namespace adm {
  namespace detail {

    /// implementation of variant_type_index; finds the index of type T within
    /// variant V, or -1 if it's not found
    template <int i, typename T, typename... Vt>
    struct VariantTypeIndexImpl;

    // base case when T is found at index i
    template <int i, typename T, typename... Vt>
    struct VariantTypeIndexImpl<i, T, T, Vt...> {
      static constexpr int index = i;
    };

    // recursive case where T is not at index i
    template <int i, typename T, typename Th, typename... Vt>
    struct VariantTypeIndexImpl<i, T, Th, Vt...> {
      static constexpr int index = VariantTypeIndexImpl<i + 1, T, Vt...>::index;
    };

    // base case at end of list, to get a nice error
    template <int i, typename T>
    struct VariantTypeIndexImpl<i, T> {
      static constexpr int index = -1;
    };

    template <typename T, typename V>
    struct VariantTypeIndexWrapper;

    template <typename T, typename... Vt>
    struct VariantTypeIndexWrapper<T, boost::variant<Vt...>> {
      static constexpr int index = VariantTypeIndexImpl<0, T, Vt...>::index;
    };

    /// get the index of a type T within Variant
    ///
    /// for example, variant_type_index<int, boost::variant<int, float>>() == 0
    ///
    /// static_asserts if the type is not found
    template <typename T, typename Variant>
    constexpr int variant_type_index() {
      constexpr int index = VariantTypeIndexWrapper<T, Variant>::index;
      static_assert(index != -1, "variant cannot hold the given type");

      return index;
    }

    /// check if a variant holds a given type
    ///
    /// for example, holds_alternative<int>(boost::variant<int, float>(1)) == true
    template <typename T, typename Variant>
    constexpr bool holds_alternative(const Variant &v) {
      return v.which() == variant_type_index<T, Variant>();
    }

  }  // namespace detail
}  // namespace adm
