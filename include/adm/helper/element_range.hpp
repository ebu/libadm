#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <boost/range/iterator_range_core.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

namespace adm {

  namespace detail {

    /// iterator adapter from Iterator to an iterator of type T, where the
    /// value type of Iterator can be converted to T by an implicit cast
    ///
    /// this is normally used to add const to the inner type of shared pointers
    template <typename Iterator, typename T>
    struct CastIteratorAdapter
        : boost::iterator_adaptor<CastIteratorAdapter<Iterator, T>, Iterator, T,
                                  boost::use_default, T> {
      using Base = typename CastIteratorAdapter::iterator_adaptor_;

      using Base::Base;
    };

    /// a boost::iterator_range that iterates over a std::vector<VecType>, with
    /// the values casted to const ValueType
    template <typename VecType, typename ValueType>
    using CastedVectorRange = boost::iterator_range<detail::CastIteratorAdapter<
        typename std::vector<VecType>::const_iterator, const ValueType>>;

  }  // namespace detail

  /**
   * @brief Proxy access to a STL iterator range of Adm elements
   *
   * This templated alias gives access to a range of `std::shared_ptr<>`
   * to Adm elements.
   *
   * This value type of the underlying iterator/range will be either
   * `const std::shared_ptr<Element>` or `const std::shared_ptr<const *
   * Element>` in the case of `const Element` template parameter.
   *
   * The latter case is the main reason for the existence of this alias, as
   * this allows as to return a  range of "real" `const` elements.
   *
   * The range itself is implemented using `boost::iterator_range`.
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
  using ElementRange =
      detail::CastedVectorRange<std::shared_ptr<std::remove_const_t<Element>>,
                                std::shared_ptr<Element>>;

  /// same as ElementRange, but with weak_ptr
  template <typename Element>
  using ElementWeakRange =
      detail::CastedVectorRange<std::weak_ptr<std::remove_const_t<Element>>,
                                std::weak_ptr<Element>>;

}  // namespace adm
