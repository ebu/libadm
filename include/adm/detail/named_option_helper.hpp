#pragma once
#include <memory>
#include <functional>
#include <type_traits>

namespace adm {

  namespace detail {

    template <typename Element>
    inline void setNamedOptionHelper(Element&& /*element*/) {}

    template <typename Element, typename T, typename... Parameters>
    void setNamedOptionHelper(Element&& element, T v, Parameters... args) {
      element->set(std::move(v));

      setNamedOptionHelper(std::forward<Element>(element), std::move(args)...);
    }

  }  // namespace detail
}  // namespace adm
