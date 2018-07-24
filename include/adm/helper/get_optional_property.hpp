#pragma once

namespace adm {

  /**
   * Return value of a property or a given defaultValue if the property is not
   available.
   *
   * This is just a more expressive shorthand equivalent to
   @code
     element->has<Property>() ? element->get<Property>() : defaultValue;
   @endcode
   */
  template <typename Property, typename ElementPtr>
  Property getPropertyOr(ElementPtr element, Property defaultValue) {
    if (!element->template has<Property>()) {
      return defaultValue;
    }
    return element->template get<Property>();
  }

}  // namespace adm
