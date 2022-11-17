#pragma once
#include "adm/detail/auto_base_detail.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"
#include "boost/optional.hpp"
#include "boost/variant.hpp"
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
    // To enable all combinations without making the dreaded diamond, these are
    // chained, with the actual inheritance happening in one of the Combine*B
    // classes. When combining two or more classes we have an inheritance
    // hierarchy like:
    //
    // Combine<Head, Tail...> { B = HasParameters<Tail...> }
    // -> CombineGetSetHas<A, B>
    //    -> CombineIsDefaultUnsetB<A, B>
    //       -> A
    //       -> B
    //
    // Combine is used co-recursively by HasParameters, which combines many
    // *Base classes.
    //
    // For variant types, there is one base class which implements the methods
    // for the variant type (e.g. OptionalParameter<variant<...>>), and one
    // base class for each of the types T in the variant, e.g.
    // VariantTypeParameter<Param, T>.
    //
    // These are combined together using VariantParameter. This works similarly
    // to HasParameters -- the VariantTypeParameter for the first type in the
    // variant inherits from the VariantTypeParameter from the next, and so on,
    // until the OptionalParameter<variant<...>>. This structure is necessary
    // because the methids in VariantTypeParameter need to cahh the methods on
    // the OptionalParameter<variant<...>>, so must inherit from it.
    //
    // For example, if we have P = OptionalParameter<variant<A, B, C>>, then
    // VariantParameter is:
    //
    // VariantParameter<P> = VariantTypeParameter<P, A>
    //
    // And the inheritance structure looks like:
    //
    // VariantTypeParameter<P, A>
    // -> VariantTypeParameter<P, B>
    //    -> VariantTypeParameter<P, C>
    //       -> P

#ifndef IN_DOXYGEN

    struct Flags {
      using has_get_set_has = std::false_type;
      using has_isDefault_unset = std::false_type;
      using has_add_remove = std::false_type;
    };

    /// make a class derived from the given base classes, combining the
    /// get, set, has, isDefault and unset overloads
    template <typename... Bases>
    using HasParameters = typename HasParametersT<Bases...>::type;

    /// Get the default value of T parameters. Specialise this to add custom
    /// defaults.
    template <typename T>
    T getDefault() {
      return T{};
    }

    /// base class with set/get/has methods for a required parameter of type T
    /// combine these together using HasParameters
    template <typename T>
    class RequiredParameter : public Flags {
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      using has_get_set_has = std::true_type;

      ADM_BASE_EXPORT T get(Tag) const { return value_; }
      ADM_BASE_EXPORT void set(T value) { value_ = std::move(value); }
      ADM_BASE_EXPORT bool has(Tag) const { return true; }

     private:
      T value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T with no default. combine these together using
    /// HasParameters
    template <typename T>
    class OptionalParameter : public Flags {
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      using has_get_set_has = std::true_type;
      using has_isDefault_unset = std::true_type;

      ADM_BASE_EXPORT T get(Tag) const { return value_.get(); }
      ADM_BASE_EXPORT void set(T value) { value_ = std::move(value); }
      ADM_BASE_EXPORT bool has(Tag) const { return value_ != boost::none; }
      ADM_BASE_EXPORT bool isDefault(Tag) const { return false; }
      ADM_BASE_EXPORT void unset(Tag) { value_ = boost::none; }

     private:
      boost::optional<T> value_;
    };

    /// base class with set/get/has/isDefault/unset methods for an optional
    /// parameter of type T, which has a default provided by DefaultParameter.
    /// combine these together using HasParameters
    template <typename T>
    class DefaultParameter : public Flags {
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      using has_get_set_has = std::true_type;
      using has_isDefault_unset = std::true_type;

      ADM_BASE_EXPORT T get(Tag) const {
        return boost::get_optional_value_or(value_, getDefault<T>());
      }
      ADM_BASE_EXPORT void set(T value) { value_ = std::move(value); }
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
    template <typename T>
    class VectorParameter : public Flags {
      using Value = typename T::value_type;
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      using has_get_set_has = std::true_type;
      using has_isDefault_unset = std::true_type;
      using has_add_remove = std::true_type;

      ADM_BASE_EXPORT T get(Tag) const { return value_; }
      ADM_BASE_EXPORT void set(T value) { value_ = std::move(value); }
      ADM_BASE_EXPORT bool has(Tag) const { return value_.size() > 0; }
      ADM_BASE_EXPORT bool isDefault(Tag) const { return false; }
      ADM_BASE_EXPORT void unset(Tag) { value_.clear(); }

      ADM_BASE_EXPORT bool add(Value item) {
        auto it = find_item(item);
        if (it == value_.end()) {
          value_.push_back(std::move(item));
          return true;
        } else {
          return false;
        }
      }

      ADM_BASE_EXPORT void remove(Value const& item) {
        auto it = find_item(item);
        if (it != value_.end()) value_.erase(it);
      }

     private:
      T value_;
      ADM_BASE_EXPORT typename T::iterator find_item(Value const& item) {
        auto it = value_.begin();
        for (; it != value_.end(); it++)
          if (ParameterCompare<Value>::compare(item, *it)) break;
        return it;
      }
    };

    /// Base class for one type within a variant.
    ///
    /// VariantParam should be a parameter type like
    /// OptionalParameter<boost::variant<...>>, and T should be one of the
    /// types of the variant.
    template <typename VariantParam, typename T>
    class VariantTypeParameter
        : public VariantTypeParameterBase<VariantParam, T> {
      using Variant = typename VariantParam::ParameterType;
      using Tag = typename detail::ParameterTraits<T>::tag;
      using VariantTag = typename detail::ParameterTraits<Variant>::tag;

     public:
      using VariantTypeParameterBase<VariantParam, T>::get;
      ADM_BASE_EXPORT T get(Tag) const {
        return boost::get<T>(get(VariantTag{}));
      }

      using VariantTypeParameterBase<VariantParam, T>::set;
      ADM_BASE_EXPORT void set(T value) {
        return VariantParam::set(std::move(value));
      }

      using VariantTypeParameterBase<VariantParam, T>::has;
      ADM_BASE_EXPORT bool has(Tag) const {
        return has(VariantTag{}) && get(VariantTag()).type() == typeid(T);
      }

      using VariantTypeParameterBase<VariantParam, T>::isDefault;
      ADM_BASE_EXPORT bool isDefault(Tag) const {
        return isDefault(VariantTag()) && get(VariantTag()).type() == typeid(T);
      }

      using VariantTypeParameterBase<VariantParam, T>::unset;
      ADM_BASE_EXPORT void unset(Tag) {
        if (has(Tag{})) unset(VariantTag{});
      }
    };

    /// Wrapper which has methods for each type in a variant parameter.
    ///
    /// This should be used in HasParameters, with VariantParam being a
    /// parameter like OptionalParameter<V>, where V is a boost::variant.
    ///
    /// When using this with OptionalParameter<V>, the following classes should
    /// be explicitly instantiated:
    /// - OptionalParameter<V> (not VariantParameter<...>)
    /// - One VariantParameter<OptionalParameter<V>, T> for each T in V.
    template <typename VariantParam>
    using VariantParameter = VariantTypeParameter<
        VariantParam, VariantFirstType<typename VariantParam::ParameterType>>;

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

#endif

  }  // namespace detail
}  // namespace adm
