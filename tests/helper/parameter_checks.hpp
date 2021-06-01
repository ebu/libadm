#pragma once
#include <type_traits>
#include <catch2/catch.hpp>
#include <adm/elements/audio_block_format_objects.hpp>
#include <boost/type_index.hpp>
#include "parameter_comparators.hpp"
namespace adm_test {
  namespace detail {

    template <typename ParamT, typename ElementT, typename ValueT>
    void check_required_impl(ElementT element, ValueT expectedInitial,
                             ValueT expectedAfterSet) {
      using boost::typeindex::type_id;
      INFO("element type: " << type_id<ElementT>().pretty_name());
      INFO("parameter type: " << type_id<ParamT>().pretty_name());
      SECTION("check default constructed element") {
        REQUIRE(element.template has<ParamT>() == true);
        REQUIRE(element.template isDefault<ParamT>() == false);
        REQUIRE(element.template get<ParamT>() == expectedInitial);
      }
      SECTION("check parameter after set") {
        element.set(ParamT{expectedAfterSet});
        REQUIRE(element.template has<ParamT>() == true);
        REQUIRE(element.template isDefault<ParamT>() == false);
        REQUIRE(element.template get<ParamT>() == expectedAfterSet);
      }
    }

    template <typename ParamT, typename ElementT, typename ValueT>
    void check_default_impl(ElementT element, ValueT expectedDefault,
                            ValueT expectedAfterSet) {
      using boost::typeindex::type_id;
      INFO("element type: " << type_id<ElementT>().pretty_name());
      INFO("parameter type: " << type_id<ParamT>().pretty_name());
      SECTION("check default constructed element") {
        REQUIRE(element.template has<ParamT>() == true);
        REQUIRE(element.template isDefault<ParamT>() == true);
        REQUIRE(element.template get<ParamT>() == expectedDefault);
      }
      SECTION("check parameter after set") {
        element.set(ParamT{expectedAfterSet});
        REQUIRE(element.template has<ParamT>() == true);
        REQUIRE(element.template isDefault<ParamT>() == false);
        REQUIRE(element.template get<ParamT>() == expectedAfterSet);
        SECTION("and unset") {
          element.template unset<ParamT>();
          REQUIRE(element.template has<ParamT>() == true);
          REQUIRE(element.template isDefault<ParamT>() == true);
          REQUIRE(element.template get<ParamT>() == expectedDefault);
        }
      }
    }

    template <typename ParamT, typename ElementT, typename ValueT>
    void check_optional_impl(ElementT element, ValueT expectedAfterSet) {
      using boost::typeindex::type_id;
      INFO("element type: " << type_id<ElementT>().pretty_name());
      INFO("parameter type: " << type_id<ParamT>().pretty_name());
      SECTION("check default constructed element") {
        REQUIRE(element.template has<ParamT>() == false);
        REQUIRE(element.template isDefault<ParamT>() == false);
      }
      SECTION("check parameter after set") {
        element.set(ParamT{expectedAfterSet});
        REQUIRE(element.template has<ParamT>() == true);
        REQUIRE(element.template isDefault<ParamT>() == false);
        REQUIRE(element.template get<ParamT>() == expectedAfterSet);
        SECTION("and unset") {
          element.template unset<ParamT>();
          REQUIRE(element.template has<ParamT>() == false);
          REQUIRE(element.template isDefault<ParamT>() == false);
        }
      }
    }

    template <typename T>
    class HasDefaultOf {
     public:
      explicit HasDefaultOf(T const& value) : val{value} {}
      T get() const { return val; }

     private:
      T val;
    };
    template <typename T>
    class CanBeSetTo {
     public:
      explicit CanBeSetTo(T const& value) : val{value} {}
      T get() const { return val; }

     private:
      T val;
    };
  }  // namespace detail

  template <typename ParamT, typename ElementT, typename ValueT>
  void check_required_param(ElementT element,
                            detail::HasDefaultOf<ValueT> defaultVal,
                            detail::CanBeSetTo<ValueT> modifiedVal) {
    detail::check_required_impl<ParamT>(element, defaultVal.get(),
                                        modifiedVal.get());
  }

  template <typename ParamT, typename ElementT, typename ValueT>
  void check_defaulted_param(ElementT element,
                             detail::HasDefaultOf<ValueT> defaultVal,
                             detail::CanBeSetTo<ValueT> modifiedVal) {
    detail::check_default_impl<ParamT>(element, defaultVal.get(),
                                       modifiedVal.get());
  }

  template <typename ParamT, typename ElementT, typename ValueT>
  void check_optional_param(ElementT element,
                            detail::CanBeSetTo<ValueT> modifiedVal) {
    detail::check_optional_impl<ParamT>(element, modifiedVal.get());
  }

  template <typename T>
  detail::HasDefaultOf<T> hasDefaultOf(T value) {
    return detail::HasDefaultOf<T>(value);
  }

  template <typename T>
  detail::CanBeSetTo<T> canBeSetTo(T value) {
    return detail::CanBeSetTo<T>(value);
  }
}  // namespace adm_test