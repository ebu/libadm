//
// Created by Richard Bailey on 01/07/2022.
//

#ifndef LIBADM_GET_HPP
#define LIBADM_GET_HPP
#include <optional>
#include <tuple>

namespace adm { namespace detail {
// Returns an optional child of a parent element
template<typename ElementT, typename ParentT>
std::optional<ElementT> get_subelement_if(ParentT const& parent) {
  if(parent.template has<ElementT>()) {
    return parent.template get<ElementT>();
  }
  return {};
}

// Extracts types from Ts..., calls get_if<T> with each T, returns tuple of all results
template<typename... Ts, std::size_t... Indexes, typename ParentT>
auto get_subelements_indexed(std::index_sequence<Indexes...>, ParentT const& doc) {
  return std::make_tuple(get_subelement_if<std::tuple_element_t<Indexes, std::tuple<Ts...>>>(doc)...);
}

template<typename ElementT, typename ParentT>
auto get_element_range(ParentT const& parent) {
  return parent.template getElements<ElementT>();
}

template<typename... Ts, std::size_t... Indexes, typename ParentT>
auto get_element_range_indexed(std::index_sequence<Indexes...>, ParentT const& doc) {
  return std::make_tuple(get_element_range<std::tuple_element_t<Indexes, std::tuple<Ts...>>>(doc)...);
}

}

template<typename... Ts>
struct ElementList{};
} // namespace adm::detail
#endif  //LIBADM_GET_HPP
