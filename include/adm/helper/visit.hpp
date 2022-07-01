#pragma once
#include <optional>
#include <iostream>
#include "adm/detail/get.hpp"

namespace adm {
  // Get a tuple of optional<ElementT> child elements from a parent
  // Optional will be empty when ParentT::has<ElementT>() returns false.
  template<typename... ElementTs, typename ParentT>
  auto get_subelements(ParentT const& parent) {
    return detail::get_subelements_indexed<ElementTs...>(
      std::make_index_sequence<sizeof...(ElementTs)>(),
      parent);
  }

  // Get a tuple of ElementRange<ElementT> child elements from a parent
  template<typename... ElementTs, typename ParentT>
  auto get_elements(ParentT const& parent) {
    return detail::get_element_range_indexed<ElementTs...>(
        std::make_index_sequence<sizeof...(ElementTs)>(),
        parent);
  }

  // For using with type lists, need to wrap in a struct as can't pass
  // function templates into templates
  template <typename T>
  struct GetSub;

  // Specialisation to grab the type list from the tuple and apply it to get_elements
  template <typename... Ts>
  struct GetSub<ElementList<Ts...>> {
    template <typename T>
    auto operator()(T const& parent) {
      return get_subelements<Ts...>(parent);
    }
  };
  template <typename T>
  struct GetElementRangesS;

  template <typename... Ts>
  struct GetElementRangesS<ElementList<Ts...>> {
    auto operator()(Document const& parent) const {
      return get_elements<Ts...>(parent);
    }
  };

  // A bit of a hack to make the callable wrapper look like a normal function
  template<typename T>
  static constexpr GetElementRangesS<T> getElementRanges = GetElementRangesS<T>{};

  // This does the actual visitation
  template <typename VisitorT, typename... Args>
  void do_visit(VisitorT&& v, Args&&... args) {
    (..., v(std::forward<Args>(args)));
  }

  // Wraps a visitor that handles element types.
  // iterates though the range of each element type.
  // calls the wrapped visitor after de-referencing each element if valid.
  template<typename VisitorT>
  struct RangeVisitor : public VisitorT {
    template <typename RangeT>
    void operator()(RangeT range) {
      for (auto const& e : range) {
        if(e) {
          auto& v = static_cast<VisitorT&>(*this);
          v(*e);
        }
      }
    }
  };

  // the api function for users, takes a tuple of parameters and an object with call
  // operators for all the parameter types (at least) then calls the object
  // with each element of the tuple
  template <typename VisitorT, typename... Ts>
  void visit(std::tuple<Ts...>&& t, VisitorT&& visitor) {
    std::apply(
        // The shenanigans here with the tuple is for correctly forward capturing the visitor
        // doesn't work without a wrapper class and tuple works as the wrapper
        // mutable needed to modify v within the lambda
        [v = std::tuple<VisitorT>(std::forward<VisitorT>(visitor))](auto&&... args) mutable {
          do_visit(std::forward<VisitorT>(std::get<0>(v)),
              std::forward<decltype(args)>(args)...);
        },
        std::forward<std::tuple<Ts...>>(t));
  }
}