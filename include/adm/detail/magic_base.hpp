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
    /// and B
    template <typename A, typename B, typename Base>
    struct CombineGetSetHas : public Base {
      using A::get;
      using B::get;

      using A::set;
      using B::set;

      using A::has;
      using B::has;
    };

    /// a subclass of Base, with using declarations for isDefault and unset in A
    /// and B
    template <typename A, typename B, typename Base>
    struct CombineIsDefaultUnset : public Base {
      using A::isDefault;
      using B::isDefault;

      using A::unset;
      using B::unset;
    };

    /// Combine A and B using F is defined_in_both
    template <bool defined_in_both,
              template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    struct ApplyIfT;

    template <template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    struct ApplyIfT<false, F, A, B, Base> {
      using type = Base;
    };

    template <template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    struct ApplyIfT<true, F, A, B, Base> {
      using type = F<A, B, Base>;
    };

    template <bool defined_in_both,
              template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    using ApplyIf = typename ApplyIfT<defined_in_both, F, A, B, Base>::type;

    /// subclass of A and B
    template <typename A, typename B>
    struct CombineRaw : public A, public B {};

    /// a subclass of A and B, with methods according to their Flags
    template <typename A, typename B>
    struct Combine
        : public ApplyIf<
              A::has_get_set_has && B::has_get_set_has, CombineGetSetHas, A, B,
              ApplyIf<A::has_isDefault_unset && B::has_isDefault_unset,
                      CombineIsDefaultUnset, A, B, CombineRaw<A, B>>> {
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
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class RequiredBase : public Flags {
     public:
      static constexpr bool has_get_set_has = true;

      ADM_EXPORT T get(Tag) const { return value_; }
      ADM_EXPORT void set(T value) { value_ = value; }
      ADM_EXPORT bool has(Tag) const { return true; }

     private:
      T value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T with no default. combine these together using
    /// CombineBase
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class OptionalBase : public Flags {
     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;

      ADM_EXPORT T get(Tag) const { return value_.get(); }
      ADM_EXPORT void set(T value) { value_ = value; }
      ADM_EXPORT bool has(Tag) const { return value_ != boost::none; }
      ADM_EXPORT bool isDefault(Tag) const { return false; }
      ADM_EXPORT void unset(Tag) { value_ = boost::none; }

     private:
      boost::optional<T> value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T, which has a default provided by DefaultParameter.
    /// combine these together using CombineBase
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class DefaultedBase : public Flags {
     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;

      ADM_EXPORT T get(Tag) const {
        return boost::get_optional_value_or(value_, getDefault<T>());
      }
      ADM_EXPORT void set(T value) { value_ = value; }
      ADM_EXPORT bool has(Tag) const { return true; }
      ADM_EXPORT bool isDefault(Tag) const { return value_ == boost::none; }
      ADM_EXPORT void unset(Tag) { value_ = boost::none; }

     private:
      boost::optional<T> value_;
    };

    template <typename T>
    struct AddWrapperMethods {
      /**
       * @brief ADM parameter getter template
       *
       * Templated getter with the wanted ADM parameter type as template
       * argument. If currently no value is available trying to get the adm
       * parameter will result in an exception. Check with the has method before
       */
      template <typename Parameter>
      Parameter get() const {
        typedef typename detail::ParameterTraits<Parameter>::tag Tag;
        return static_cast<const T*>(this)->get(Tag());
      }
    };
  }  // namespace detail
}  // namespace adm
