#pragma once
#include <type_traits>
#include <catch2/catch.hpp>
#include <adm/elements/audio_block_format_objects.hpp>
#include <boost/type_index.hpp>
#include "parameter_comparators.hpp"
namespace adm_test {
  namespace detail {

    template <typename ParamT, typename ElementT, typename ValueT>
    void check_required_impl(ElementT& element, ValueT expectedInitial,
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
    void check_default_impl(ElementT& element, ValueT expectedDefault,
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
    void check_optional_impl(ElementT& element, ValueT expectedAfterSet) {
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

    template <typename ParamT, typename ElementT, typename VectorT>
    void check_vector_impl(ElementT& element, VectorT parameter_vector) {
      SECTION("test should be supplied with non-empty vector") {
        using ValueT = typename VectorT::value_type;
        // if it's an optional vector, set to empty vector
        if (!element.template has<VectorT>()) {
          element.set(VectorT{});
        }
      }
      SECTION("Check add & remove") {
        auto items = element.template get<ParamT>();
        REQUIRE(items.empty());
        for (auto const& i : parameter_vector) {
          element.add(i);
        }
        items = element.template get<ParamT>();
        REQUIRE(items.size() == parameter_vector.size());
        for (auto i = 0u; i != items.size(); ++i) {
          REQUIRE(items[i] == parameter_vector[i]);
        }
        for (auto i = 0u; i != items.size(); ++i) {
          element.remove(items[i]);
        }
        items = element.template get<ParamT>();
        REQUIRE(items.empty());
      }
      SECTION("Check double add only adds once") {
        REQUIRE(element.template get<ParamT>().empty());
        REQUIRE(element.add(parameter_vector.front()));
        REQUIRE(element.template get<ParamT>().size() == 1);
        REQUIRE(!element.add(parameter_vector.front()));
        REQUIRE(element.template get<ParamT>().size() == 1);
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

    /// get a reference given a shared_ptr or a reference
    template <typename T>
    T& get_from_shared(std::shared_ptr<T> p) {
      return *p;
    }

    template <typename T>
    T& get_from_shared(T& p) {
      return p;
    }
  }  // namespace detail

  template <typename ParamT, typename ElementT, typename ValueT>
  void check_required_param(ElementT element,
                            detail::HasDefaultOf<ValueT> defaultVal,
                            detail::CanBeSetTo<ValueT> modifiedVal) {
    detail::check_required_impl<ParamT>(detail::get_from_shared(element),
                                        defaultVal.get(), modifiedVal.get());
  }

  template <typename ParamT, typename ElementT, typename ValueT>
  void check_defaulted_param(ElementT element,
                             detail::HasDefaultOf<ValueT> defaultVal,
                             detail::CanBeSetTo<ValueT> modifiedVal) {
    detail::check_default_impl<ParamT>(detail::get_from_shared(element),
                                       defaultVal.get(), modifiedVal.get());
  }

  template <typename ParamT, typename ElementT, typename ValueT>
  void check_optional_param(ElementT element,
                            detail::CanBeSetTo<ValueT> modifiedVal) {
    detail::check_optional_impl<ParamT>(detail::get_from_shared(element),
                                        modifiedVal.get());
  }

  template <typename ParamT, typename ElementT, typename VectorT>
  void check_vector_param(std::shared_ptr<ElementT> element,
                          detail::CanBeSetTo<VectorT> modifiedVal) {
    detail::check_vector_impl<ParamT>(detail::get_from_shared(element),
                                      modifiedVal.get());
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
