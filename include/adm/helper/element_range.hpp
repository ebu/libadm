#pragma once

#include <algorithm>
#include <boost/range/iterator_range_core.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

namespace adm {

  namespace detail {

    /**
     * Simple wrapper around an iterator to convert
     * `std::shared_ptr<Element>` to `std::shared_ptr<const Element>`.
     *
     * While this conversion is directly supported by `std::shared_ptr`, we
     * cannot use this automatically with iterators and containers. From a
     * container's point of view, `std::shared_ptr<Element>` and
     * `std::shared_ptr<const Element>` are two completly different types.
     *
     * Thus, we have this adaptor (which simply uses `boost::iterator_adaptor`
     * with the correct template arguments) to do the conversion for us.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Iterator, typename Element>
    struct ConstSharedPtrIteratorAdaptor
        : boost::iterator_adaptor<
              ConstSharedPtrIteratorAdaptor<Iterator, Element>, Iterator,
              std::shared_ptr<const Element>, boost::use_default,
              const std::shared_ptr<const Element>> {
      typedef boost::iterator_adaptor<
          ConstSharedPtrIteratorAdaptor<Iterator, Element>, Iterator,
          std::shared_ptr<const Element>, boost::use_default,
          const std::shared_ptr<const Element>>
          Base;

      ConstSharedPtrIteratorAdaptor() : Base() {}
      explicit ConstSharedPtrIteratorAdaptor(Iterator it) : Base(it) {}
    };

    template <typename Iterator, typename Element>
    struct ConstWeakPtrIteratorAdaptor
        : boost::iterator_adaptor<
              ConstWeakPtrIteratorAdaptor<Iterator, Element>, Iterator,
              std::weak_ptr<const Element>, boost::use_default,
              const std::weak_ptr<const Element>> {
      typedef boost::iterator_adaptor<
          ConstWeakPtrIteratorAdaptor<Iterator, Element>, Iterator,
          std::weak_ptr<const Element>, boost::use_default,
          const std::weak_ptr<const Element>>
          Base;

      ConstWeakPtrIteratorAdaptor() : Base() {}
      explicit ConstWeakPtrIteratorAdaptor(Iterator it) : Base(it) {}
    };

    /**
     * Helper template to get the iterator_range type for
     * `std::shared_ptr<Element>` in an `std::vector`.
     * @sa struct ElementRangeImpl<const Element>
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeImpl {
      using type = typename boost::iterator_range<
          typename std::vector<std::shared_ptr<Element>>::const_iterator>;
    };

    /**
     * Specialization of the helper template to get the iterator_range type
     * for `std::shared_ptr<const Element>` in a `std::vector`.
     *
     * The main trick here is that we use `ConstSharedPtrIteratorAdaptor` to
     * get iterators to `std::shared_ptr<const Element>`  from a
     * `std::vector` containing `std::shared_ptr<Element>`.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeImpl<const Element> {
      using type = typename boost::iterator_range<ConstSharedPtrIteratorAdaptor<
          typename std::vector<std::shared_ptr<Element>>::const_iterator,
          Element>>;
    };

    /**
     * Helper template to get the iterator_range type for
     * `std::weak_ptr<Element>` in an `std::vector`.
     * @sa struct ElementRangeImpl<const Element>
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeWeakImpl {
      using type = typename boost::iterator_range<
          typename std::vector<std::weak_ptr<Element>>::const_iterator>;
    };

    /**
     * Specialization of the helper template to get the iterator_range type
     * for `std::weak_ptr<const Element>` in a `std::vector`.
     *
     * The main trick here is that we use `ConstSharedPtrIteratorAdaptor` to
     * get iterators to `std::weak_ptr<const Element>`  from a
     * `std::vector` containing `std::weak_ptr<Element>`.
     * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
     */
    template <typename Element>
    struct ElementRangeWeakImpl<const Element> {
      using type = typename boost::iterator_range<ConstWeakPtrIteratorAdaptor<
          typename std::vector<std::weak_ptr<Element>>::const_iterator,
          Element>>;
    };

  }  // namespace detail

  /**
   * @brief Proxy access to a STL iterator range of Adm elements
   * @headerfile element_range.hpp <adm/helpers/element_range.hpp>
   *
   * This templated alias gives access to a range of `std::shared_ptr<>`
   * to Adm elements.
   *
   * This value type of the underlying iterator/range will be either
   * `std::shared_ptr<Element>` or `const std::shared_ptr<const      *
   * Element`> in the case of `const Element` template parameter.
   *
   * The latter case is the main reason for the existence of this alias and
   * its supporting classes, as this allows as to return a  range of "real"
   * `const` elements.
   *
   * The range itself is simply implemented by using `boost::iterator_range`.
   *
   * One make use the convenience `detail::makeElementRange()` function to
   * quickly create a range from an container.
   *
   * @code
    ElementRange<AudioObject> getMutableAudioObjects();
    ElementRange<const AudioObject> getConstAudioObjects();

    // ...

    for (const auto element& : getMutableAudioObjects()) {
      // element is of type std::shared_ptr<AudioObject>
      // ...
     }

    for (const auto element& : getMutableAudioObjects()) {
       // element is of type std::shared_ptr<const AudioObject>
       // ...
    }
   @endcode
   *
   * @note
   * Please note that the range will *always* be represented by
   * `const_iterator`s. The difference is between `Element` and `const
   * Element` is the *value type* of the iterators.
   *
   * The rationale for this is that it's not allowed to change the range
   * itself via its iterators, because there are member functions of the
   * parent elements to achieve this and ensure internal invariants to hold.
   */
  template <typename Element>
  using ElementRange = typename detail::ElementRangeImpl<Element>::type;

  template <typename Element>
  using ElementWeakRange = typename detail::ElementRangeWeakImpl<Element>::type;

  namespace detail {

    /**
     * Convenience function overload to create an range to **const**
     * `Element`s when given a **const** Container.
     * @sa makeElementRange(Container&);
     */
    template <typename Element, typename Container>
    ElementRange<const Element> makeElementRange(const Container &container) {
      typedef typename Container::const_iterator const_iterator;
      typedef ConstSharedPtrIteratorAdaptor<const_iterator, Element> Adaptor;
      return boost::make_iterator_range(Adaptor(container.cbegin()),
                                        Adaptor(container.cend()));
    }

    /**
     * Convenience function overload to create an range to
     * `Element`s when given a **non-const** Container.
     * @sa makeElementRange(const Container&);
     */
    template <typename Element, typename Container>
    ElementRange<Element> makeElementRange(Container &container) {
      return boost::make_iterator_range(container.cbegin(), container.cend());
    }

    template <typename Element, typename Container>
    std::vector<std::shared_ptr<Element>> lockWeakRange(const Container &c) {
      std::vector<std::shared_ptr<Element>> result;
      std::transform(c.cbegin(), c.cend(), std::back_inserter(result),
                     [](std::weak_ptr<Element> w) { return w.lock(); });
      return result;
    }

  }  // namespace detail

}  // namespace adm
