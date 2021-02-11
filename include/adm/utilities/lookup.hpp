#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include "adm/document.hpp"

namespace adm {

  template <typename Element, typename Attribute>
  std::vector<std::shared_ptr<Element>> lookup(
      std::shared_ptr<Document> document, const Attribute attribute) {
    std::vector<std::shared_ptr<Element>> elements;
    std::copy_if(document->template getElements<Element>().begin(),
                 document->template getElements<Element>().end(),
                 std::back_inserter(elements),
                 [attribute](const std::shared_ptr<Element> element) {
                   return element->template get<Attribute>() == attribute;
                 });
    return elements;
  }

  template <typename Element, typename ElementId, typename ElementRange>
  std::shared_ptr<Element> lookupById(const ElementRange& range,
                                      const ElementId& id) {
    for (auto& element : range) {
      if (element->template get<ElementId>() == id) {
        return element;
      }
    }
    return nullptr;
  }

}  // namespace adm
