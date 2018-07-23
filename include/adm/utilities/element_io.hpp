#pragma once
#include <ostream>

namespace adm {

  template <class Element>
  auto operator<<(std::ostream& os, const Element& element)
      -> decltype(element.print(os), os) {
    element.print(os);
    return os;
  }

}  // namespace adm
