#pragma once
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"
#include "boost/optional.hpp"

namespace adm {
  namespace detail {

    /// make a class derived from the given base classes, combining the
    /// get,set,has,isDefault and unset overloads
    template <typename B, typename... BTail>
    class CombineBase : public B, public CombineBase<BTail...> {
      using base = CombineBase<BTail...>;

     public:
      using B::set;
      using base::set;

     protected:
      using B::get;
      using B::has;
      using B::isDefault;
      using B::unset;

      using base::get;
      using base::has;
      using base::isDefault;
      using base::unset;
    };

    template <typename B>
    class CombineBase<B> : public B {};

    // could use this instead in c++17:
    // template <typename... Bases>
    // class CombineBases : public Bases... {
    //   using Bases::get...;
    //   using Bases::has...;
    //   using Bases::isDefault...;
    //   using Bases::set...;
    //   using Bases::unset...;
    // };

    /// holder for the default value of T parameters
    template <typename T>
    struct DefaultParameter {
      ADM_EXPORT static const T value;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T with no default. combine these together using
    /// CombineBase
    template <typename T>
    class OptionalBase {
     public:
      void set(T value) { value_ = value; }

     protected:
      T get(typename detail::ParameterTraits<T>::tag) const {
        return value_.get();
      }
      bool has(typename detail::ParameterTraits<T>::tag) const {
        return value_ != boost::none;
      }
      bool isDefault(typename detail::ParameterTraits<T>::tag) const {
        return false;
      }
      void unset(typename detail::ParameterTraits<T>::tag) {
        value_ = boost::none;
      }

      boost::optional<T> value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T, which has a default provided by DefaultParameter.
    /// combine these together using CombineBase
    template <typename T>
    class DefaultedBase : public OptionalBase<T> {
     protected:
      T get(typename detail::ParameterTraits<T>::tag) const {
        return boost::get_optional_value_or(OptionalBase<T>::value_,
                                            DefaultParameter<T>::value);
      }
      bool has(typename detail::ParameterTraits<T>::tag) const { return true; }
      bool isDefault(typename detail::ParameterTraits<T>::tag) const {
        return OptionalBase<T>::value_ == boost::none;
      }
    };

  }  // namespace detail
}  // namespace adm
