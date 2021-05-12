#pragma once
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"
#include "boost/optional.hpp"

namespace adm {
  namespace detail {
    // libadm elements typically have some overloaded methods corresponding to
    // each parameter. The set of methods defined depends on the behaviour of
    // the parameter, so for example only optional parameters have isDefault
    // and unset methods.
    //
    // We define some classes below like RequiredBase and DefaultedBase which
    // define these methods automatically for given parameter types. To use
    // these, they need to be combined together in such a way that all the
    // methods for all the parameters we want to define are available in one
    // overload set.
    //
    // This is done using multiple inheritance: given a set of *Base classes,
    // we can make a derived class using CombineBase which inherits from all of
    // them.
    //
    // The problem this presents is that the derived class needs a using
    // declaration pointing to each method in the base classes, in order to
    // bring them into one overload set, but we can't have a using declaration
    // which points to a method which doesn't exist.
    //
    // To make this work, each *Base class defines some flags (defined in
    // Flags) which says which sets of methods it implements. When Combine is
    // used to combine two *Base classes together, these flags are or-ed in the
    // resulting class, and the correct specialisations of the Combine* classes
    // are selected by and-ing the flags (we only need the using declarations
    // if both bases have the methods defined).
    //
    // To enable all combinations without making the dreaded diamond, these are
    // chained, with the actual inheritance happening in CombineRaw. When
    // combining A and B we have an inheritance hierarchy like:
    //
    // Combine<A, B>
    //  -> CombineGetSetHas<A, B, X>
    //      -> CombineIsDefaultUnset<A, B, Y>
    //          -> CombineRaw<A, B>
    //              -> A
    //              -> B
    //
    // where X and Y are the flags controlling the specialisations of Combine*
    //
    // Combine is used by CombineBase, which recursively combines many *Base
    // classes.

    struct Flags {
      static constexpr bool has_get_set_has = false;
      static constexpr bool has_isDefault_unset = false;
    };

    /// a subclass of Base, with using declarations for set, get and has in A
    /// and B if defined_in_both
    template <typename A, typename B, bool defined_in_both, typename Base>
    struct CombineGetSetHas;

    template <typename A, typename B, typename Base>
    struct CombineGetSetHas<A, B, false, Base> : public Base {};

    template <typename A, typename B, typename Base>
    struct CombineGetSetHas<A, B, true, Base> : public Base {
      using A::get;
      using B::get;

      using A::set;
      using B::set;

      using A::has;
      using B::has;
    };

    /// a subclass of Base, with using declarations for isDefault and unset in A
    /// and B if defined_in_both
    template <typename A, typename B, bool defined_in_both, typename Base>
    struct CombineIsDefaultUnset;

    template <typename A, typename B, typename Base>
    struct CombineIsDefaultUnset<A, B, false, Base> : public Base {};

    template <typename A, typename B, typename Base>
    struct CombineIsDefaultUnset<A, B, true, Base> : public Base {
      using A::isDefault;
      using B::isDefault;

      using A::unset;
      using B::unset;
    };

    /// subclass of A and B
    template <typename A, typename B>
    struct CombineRaw : public A, public B {};

    /// a subclass of A and B, with methods according to their Flags
    template <typename A, typename B>
    struct Combine
        : public CombineGetSetHas<
              A, B, A::has_get_set_has && B::has_get_set_has,
              CombineIsDefaultUnset<
                  A, B, A::has_isDefault_unset && B::has_isDefault_unset,
                  CombineRaw<A, B>>> {
      static constexpr bool has_get_set_has =
          A::has_get_set_has || B::has_get_set_has;
      static constexpr bool has_isDefault_unset =
          A::has_isDefault_unset || B::has_isDefault_unset;
    };

    /// make a class derived from the given base classes, combining the
    /// get,set,has,isDefault and unset overloads
    template <typename B, typename... BTail>
    struct CombineBase : public Combine<B, CombineBase<BTail...>> {};

    template <typename B>
    struct CombineBase<B> : public B {};

    /// get the default value of T parameters
    template <typename T>
    T getDefault() {
      return T{};
    }

    /// base class with set/get/has methods for a required parameter of type T
    /// combine these together using CombineBase
    template <typename T>
    class RequiredBase : public Flags {
     public:
      static constexpr bool has_get_set_has = true;

      T get(typename detail::ParameterTraits<T>::tag) const { return value_; }
      void set(T value) { value_ = value; }
      bool has(typename detail::ParameterTraits<T>::tag) const { return true; }

     private:
      T value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T with no default. combine these together using
    /// CombineBase
    template <typename T>
    class OptionalBase : public Flags {
     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;

      T get(typename detail::ParameterTraits<T>::tag) const {
        return value_.get();
      }
      void set(T value) { value_ = value; }
      bool has(typename detail::ParameterTraits<T>::tag) const {
        return value_ != boost::none;
      }
      bool isDefault(typename detail::ParameterTraits<T>::tag) const {
        return false;
      }
      void unset(typename detail::ParameterTraits<T>::tag) {
        value_ = boost::none;
      }

     private:
      boost::optional<T> value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T, which has a default provided by DefaultParameter.
    /// combine these together using CombineBase
    template <typename T>
    class DefaultedBase : public Flags {
     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;

      T get(typename detail::ParameterTraits<T>::tag) const {
        return boost::get_optional_value_or(value_, getDefault<T>());
      }
      void set(T value) { value_ = value; }
      bool has(typename detail::ParameterTraits<T>::tag) const { return true; }
      bool isDefault(typename detail::ParameterTraits<T>::tag) const {
        return value_ == boost::none;
      }
      void unset(typename detail::ParameterTraits<T>::tag) {
        value_ = boost::none;
      }

     private:
      boost::optional<T> value_;
    };

  }  // namespace detail
}  // namespace adm
