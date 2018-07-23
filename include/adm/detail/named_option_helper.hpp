#pragma once
#include <memory>
#include <functional>
#include <type_traits>

namespace adm {

  namespace detail {

    template <typename Element>
    inline void setNamedOptionHelper(Element /*element*/) {}

    template <typename Element, typename T, typename... Parameters>
    void setNamedOptionHelper(Element element, const T& v, Parameters... args) {
      element->set(v);

      setNamedOptionHelper(element, std::forward<Parameters>(args)...);
    }

    template <typename Element, typename T>
    void setNamedOptionHelper(Element element, const T& v) {
      element->set(v);
    }

  }  // namespace detail
}  // namespace adm
