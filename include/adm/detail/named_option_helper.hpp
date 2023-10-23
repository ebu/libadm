#pragma once
#include <memory>
#include <functional>
#include <type_traits>

namespace adm {

  namespace detail {

    template <typename Element>
    inline void setNamedOptionHelper(Element&& /*element*/) {}

    template <typename Element, typename T, typename... Parameters>
    void setNamedOptionHelper(Element&& element, T&& v, Parameters&&... args) {
      element->set(std::forward<T>(v));
      setNamedOptionHelper(std::forward<Element>(element),
                           std::forward<Parameters>(args)...);
    }

    /// check that the first template type is present in the remainder
    template <typename T>
    constexpr bool optionInList() {
      return false;
    }

    template <typename T, typename Param, typename... Params>
    constexpr bool optionInList() {
      using ParamT = std::remove_const_t<std::remove_reference_t<Param>>;
      return std::is_same<T, ParamT>::value || optionInList<T, Params...>();
    }

  }  // namespace detail
}  // namespace adm
