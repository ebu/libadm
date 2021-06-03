#pragma once
#include "adm/detail/auto_base_detail.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"
#include "boost/optional.hpp"
#include <algorithm>

// want to be able to override this from the tests
#ifndef ADM_BASE_EXPORT
#define ADM_BASE_EXPORT ADM_EXPORT
#endif

namespace adm {
  namespace detail {
    // libadm elements typically have some overloaded methods corresponding to
    // each parameter. The set of methods defined depends on the behaviour of
    // the parameter, so for example only optional parameters have isDefault
    // and unset methods.
    //
    // We define some classes below like RequiredParameter and DefaultParameter
    // which define these methods automatically for given parameter types. To
    // use these, they need to be combined together in such a way that all the
    // methods for all the parameters we want to define are available in one
    // overload set.
    //
    // This is done using multiple inheritance: given a set of *Base classes,
    // we can make a derived class using HasParameters which inherits from all
    // of them.
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
    //  -> ApplyIf<CombineGetSetHas<A, B>, X>
    //      -> ApplyIf<CombineIsDefaultUnset<A, B>, Y>
    //          -> CombineRaw<A, B>
    //              -> A
    //              -> B
    //
    // where X and Y are the flags controlling whether Combine* is applied or
    // not.
    //
    // Combine is used by HasParameters, which recursively combines many *Base
    // classes.

    struct Flags {
      static constexpr bool has_get_set_has = false;
      static constexpr bool has_isDefault_unset = false;
      static constexpr bool has_add_remove = false;
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

    /// a subclass of Base, with using declarations for add and remove in A and
    /// B
    template <typename A, typename B, typename Base>
    struct CombineAddRemove : public Base {
      using A::add;
      using B::add;

      using A::remove;
      using B::remove;
    };

    /// a subclass of A and B, with methods according to their Flags
    template <typename A, typename B>
    struct Combine
        : public ApplyIf<
              A::has_add_remove && B::has_add_remove, CombineAddRemove, A, B,
              ApplyIf<A::has_get_set_has && B::has_get_set_has,
                      CombineGetSetHas, A, B,
                      ApplyIf<A::has_isDefault_unset && B::has_isDefault_unset,
                              CombineIsDefaultUnset, A, B, CombineRaw<A, B>>>> {
      static constexpr bool has_get_set_has =
          A::has_get_set_has || B::has_get_set_has;
      static constexpr bool has_isDefault_unset =
          A::has_isDefault_unset || B::has_isDefault_unset;
      static constexpr bool has_add_remove =
          A::has_add_remove || B::has_add_remove;
    };

    /// make a class derived from the given base classes, combining the
    /// get, set, has, isDefault and unset overloads
    template <typename B, typename... BTail>
    struct HasParameters : public Combine<B, HasParameters<BTail...>> {};

    template <typename B>
    struct HasParameters<B> : public B {};

    /// Get the default value of T parameters. Specialise this to add custom
    /// defaults.
    template <typename T>
    T getDefault() {
      return T{};
    }

    /// base class with set/get/has methods for a required parameter of type T
    /// combine these together using HasParameters
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class RequiredParameter : public Flags {
     public:
      static constexpr bool has_get_set_has = true;

      ADM_BASE_EXPORT T get(Tag) const { return value_; }
      ADM_BASE_EXPORT void set(T value) { value_ = value; }
      ADM_BASE_EXPORT bool has(Tag) const { return true; }

     private:
      T value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T with no default. combine these together using
    /// HasParameters
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class OptionalParameter : public Flags {
     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;

      ADM_BASE_EXPORT T get(Tag) const { return value_.get(); }
      ADM_BASE_EXPORT void set(T value) { value_ = value; }
      ADM_BASE_EXPORT bool has(Tag) const { return value_ != boost::none; }
      ADM_BASE_EXPORT bool isDefault(Tag) const { return false; }
      ADM_BASE_EXPORT void unset(Tag) { value_ = boost::none; }

     private:
      boost::optional<T> value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T, which has a default provided by DefaultParameter.
    /// combine these together using HasParameters
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class DefaultParameter : public Flags {
     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;

      ADM_BASE_EXPORT T get(Tag) const {
        return boost::get_optional_value_or(value_, getDefault<T>());
      }
      ADM_BASE_EXPORT void set(T value) { value_ = value; }
      ADM_BASE_EXPORT bool has(Tag) const { return true; }
      ADM_BASE_EXPORT bool isDefault(Tag) const {
        return value_ == boost::none;
      }
      ADM_BASE_EXPORT void unset(Tag) { value_ = boost::none; }

     private:
      boost::optional<T> value_;
    };

    template <typename T>
    struct ParameterCompare {
      static bool compare(T const& lhs, T const& rhs) { return lhs == rhs; }
    };

    /// base class for storage of multiple elements in a std::vector.
    /// T should be a std::vector<Something>, as this is what the tag is
    /// associated with.
    template <typename T,
              typename Tag = typename detail::ParameterTraits<T>::tag>
    class VectorParameter : public Flags {
      using Value = typename T::value_type;

     public:
      static constexpr bool has_get_set_has = true;
      static constexpr bool has_isDefault_unset = true;
      static constexpr bool has_add_remove = true;

      ADM_BASE_EXPORT T get(Tag) const { return value_; }
      ADM_BASE_EXPORT void set(T value) { value_ = value; }
      ADM_BASE_EXPORT bool has(Tag) const { return value_.size() > 0; }
      ADM_BASE_EXPORT bool isDefault(Tag) const { return false; }
      ADM_BASE_EXPORT void unset(Tag) { value_.clear(); }

      ADM_BASE_EXPORT bool add(Value item) {
        auto it = find_item(item);
        if (it == value_.end()) {
          value_.push_back(item);
          return true;
        } else {
          return false;
        }
      }

      ADM_BASE_EXPORT void remove(Value item) {
        auto it = find_item(item);
        if (it != value_.end()) value_.erase(it);
      }

     private:
      T value_;
      ADM_BASE_EXPORT typename T::iterator find_item(Value const& item) {
        return std::find_if(
            value_.begin(), value_.end(), [&item, this](Value const& val) {
              return ParameterCompare<Value>::compare(item, val);
            });
      }
    };

    /// Helper containing templated wrapper methods like `has<Param>()` around
    /// overloaded `has(ParamTag)` type methods defined in T.
    ///
    /// This should be used from a class `C` CRTP-style, by privately
    /// inheriting from `AddWrapperMethods<C>` in `C`, making
    /// `AddWrapperMethods<C>` a friend class of `C`, and adding public using
    /// declarations for the methods you need.
    template <typename T>
    class AddWrapperMethods {
     public:
      /**
       * @brief ADM parameter getter template
       *
       * Templated getter with the wanted ADM parameter type as template
       * argument. If currently no value is available trying to get the adm
       * parameter will result in an exception. Check with the has method before
       */
      template <typename Parameter>
      Parameter get() const {
        using Tag = typename detail::ParameterTraits<Parameter>::tag;
        return static_cast<const T*>(this)->get(Tag());
      }

      /**
       * @brief ADM parameter has template
       *
       * Templated has method with the ADM parameter type as template argument.
       * Returns true if the ADM parameter is set or has a default value.
       */
      template <typename Parameter>
      bool has() const {
        using Tag = typename detail::ParameterTraits<Parameter>::tag;
        return static_cast<const T*>(this)->has(Tag());
      }

      template <typename Tag>
      bool isDefault(Tag) const {
        return false;
      }

      /**
       * @brief ADM parameter isDefault template
       *
       * Templated isDefault method with the ADM parameter type as template
       * argument. Returns true if the ADM parameter is the default value.
       */
      template <typename Parameter>
      bool isDefault() const {
        using Tag = typename detail::ParameterTraits<Parameter>::tag;
        return static_cast<const T*>(this)->isDefault(Tag());
      }

      /**
       * @brief ADM parameter unset template
       *
       * Templated unset method with the ADM parameter type as template
       * argument. Removes an ADM parameter if it is optional or resets it to
       * the default value if there is one.
       */
      template <typename Parameter>
      void unset() {
        using Tag = typename detail::ParameterTraits<Parameter>::tag;
        static_cast<T*>(this)->unset(Tag());
      }
    };

  }  // namespace detail
}  // namespace adm
