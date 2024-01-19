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

#ifndef IN_DOXYGEN
    /// make a class derived from the given base classes, combining the
    /// get, set, has, isDefault and unset overloads
    template <typename... Bases>
    using HasParameters = Combine<TypeList<Bases...>>;

    /// Get the default value of T parameters. Specialise this to add custom
    /// defaults.
    template <typename T>
    T getDefault() {
      return T{};
    }

    /// base class with set/get/has methods for a required parameter of type T
    /// combine these together using HasParameters
    template <typename T>
    class RequiredParameter {
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      static constexpr Flags flags = Flags::HAS_GET_SET_HAS;

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
    class OptionalParameter {
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      static constexpr Flags flags =
          Flags::HAS_GET_SET_HAS | Flags::HAS_ISDEFAULT_UNSET;

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
    class DefaultParameter {
      using Tag = typename detail::ParameterTraits<T>::tag;

     public:
      using ParameterType = T;
      static constexpr Flags flags =
          Flags::HAS_GET_SET_HAS | Flags::HAS_ISDEFAULT_UNSET;

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
    class VectorParameter {
      using Tag = typename detail::ParameterTraits<T>::tag;
      using Value = typename T::value_type;

     public:
      using ParameterType = T;
      static constexpr Flags flags = Flags::HAS_GET_SET_HAS |
                                     Flags::HAS_ISDEFAULT_UNSET |
                                     Flags::HAS_ADD_REMOVE;

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
        return std::find_if(
            value_.begin(), value_.end(), [&item](Value const& val) {
              return ParameterCompare<Value>::compare(item, val);
            });
      }
    };

    /// Base class for one type within a variant.
    ///
    /// VariantParam should be a parameter type like
    /// OptionalParameter<boost::variant<...>>, and T should be one of the
    /// types of the variant.
    template <typename VariantParam, typename T>
    class VariantTypeParameter : public virtual VariantParam {
      using Variant = typename VariantParam::ParameterType;
      using Tag = typename detail::ParameterTraits<T>::tag;
      using VariantTag = typename detail::ParameterTraits<Variant>::tag;

     public:
      using VariantParam::get;
      ADM_BASE_EXPORT T get(Tag) const {
        return boost::get<T>(get(VariantTag{}));
      }

      using VariantParam::set;
      ADM_BASE_EXPORT void set(T value) {
        return VariantParam::set(std::move(value));
      }

      using VariantParam::has;
      ADM_BASE_EXPORT bool has(Tag) const {
        return has(VariantTag{}) && get(VariantTag()).type() == typeid(T);
      }

      using VariantParam::isDefault;
      ADM_BASE_EXPORT bool isDefault(Tag) const {
        return isDefault(VariantTag()) && get(VariantTag()).type() == typeid(T);
      }

      using VariantParam::unset;
      ADM_BASE_EXPORT void unset(Tag) {
        if (has(Tag{})) unset(VariantTag{});
      }
    };

    template <typename VariantParam, typename VariantParameter>
    struct VariantParameterHelper;

    template <typename VariantParam, typename... Ts>
    struct VariantParameterHelper<VariantParam, boost::variant<Ts...>> {
      using type = HasParameters<VariantTypeParameter<VariantParam, Ts>...>;
    };

    /// Wrapper which has methods for each type in a variant parameter.
    ///
    /// This should be used in HasParameters, with VariantParam being a
    /// parameter like OptionalParameter<V>, where V is a boost::variant.
    ///
    /// When using this with OptionalParameter<V>, the following classes should
    /// be explicitly instantiated:
    /// - OptionalParameter<V> (not VariantParameter<...>)
    /// - One VariantTypeParameter<OptionalParameter<V>, T> for each T in V.
    template <typename VariantParam>
    using VariantParameter = typename VariantParameterHelper<
        VariantParam, typename VariantParam::ParameterType>::type;

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
