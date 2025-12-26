// ensure that *Parameter methods are not declared as __declspec(dllimport) on
// windows -- we want them to be defined in this unit, as they are not defined
// in libadm
#define ADM_BASE_EXPORT
#include "adm/detail/auto_base.hpp"

#include <catch2/catch.hpp>
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/type_traits.hpp"
#include <vector>

namespace adm {
  struct RequiredTag {};
  using Required = detail::NamedType<int, RequiredTag>;

  struct OptionalTag {};
  using Optional = detail::NamedType<int, OptionalTag>;

  struct DefaultTag {};
  using Default = detail::NamedType<int, DefaultTag>;

  struct VectorTag {};
  using Vector = detail::NamedType<int, VectorTag>;

  struct VectorsTag {};
  using Vectors = std::vector<Vector>;
  ADD_TRAIT(Vectors, VectorsTag);

  struct VariantATag {};
  struct VariantA {
    using tag = VariantATag;
    int x;
  };
  struct VariantBTag {};
  struct VariantB {
    using tag = VariantBTag;
    int x;
  };

  struct VariantTag {};
  using Variant = boost::variant<VariantA, VariantB>;
  ADD_TRAIT(Variant, VariantTag);

  namespace detail {
    template <>
    Default getDefault<Default>() {
      return Default{42};
    }

    template class RequiredParameter<Required>;
    template class OptionalParameter<Optional>;
    template class DefaultParameter<Default>;
    template class VectorParameter<Vectors>;
    template class OptionalParameter<Variant>;
    template class VariantTypeParameter<OptionalParameter<Variant>, VariantA>;
    template class VariantTypeParameter<OptionalParameter<Variant>, VariantB>;

    using Base =
        HasParameters<RequiredParameter<Required>, OptionalParameter<Optional>,
                      DefaultParameter<Default>, VectorParameter<Vectors>,
                      VariantParameter<OptionalParameter<Variant>>>;
  };  // namespace detail

  // standard wrapper around Base providing templated methods. not really
  // needed for testing, but we might want to replace these with a template in
  // the future, and this shows how it will typically be used.
  struct TestElement : private detail::Base {
    template <typename... Parameters>
    explicit TestElement(Parameters... namedArgs) {
      detail::setNamedOptionHelper(this, std::move(namedArgs)...);
    }

    using detail::Base::add;
    using detail::Base::remove;
    using detail::Base::set;

    template <typename Parameter>
    Parameter get() const {
      using Tag = typename detail::ParameterTraits<Parameter>::tag;
      return get(Tag());
    }

    template <typename Parameter>
    bool has() const {
      using Tag = typename detail::ParameterTraits<Parameter>::tag;
      return has(Tag());
    }

    template <typename Parameter>
    bool isDefault() const {
      using Tag = typename detail::ParameterTraits<Parameter>::tag;
      return isDefault(Tag());
    }

    template <typename Parameter>
    void unset() {
      using Tag = typename detail::ParameterTraits<Parameter>::tag;
      return unset(Tag());
    }

   private:
    using detail::Base::get;
    using detail::Base::has;
    using detail::Base::isDefault;
    using detail::Base::unset;
  };

};  // namespace adm

using namespace adm;

TEST_CASE("constructor") {
  TestElement e{Required{1}, Optional{2}, Default{3}, Vectors{{Vector{4}}}};

  REQUIRE(e.get<Required>() == 1);
  REQUIRE(e.get<Optional>() == 2);
  REQUIRE(e.get<Default>() == 3);
  REQUIRE(e.get<Vectors>() == Vectors{{Vector{4}}});
}

TEST_CASE("required") {
  TestElement e;
  REQUIRE(e.get<Required>() == 0);
  REQUIRE(e.has<Required>());

  e.set(Required{5});
  REQUIRE(e.get<Required>() == 5);
}

TEST_CASE("optional") {
  TestElement e;
  REQUIRE(!e.has<Optional>());
  REQUIRE(!e.isDefault<Optional>());

  e.set(Optional{5});
  REQUIRE(e.get<Optional>() == 5);
  REQUIRE(e.has<Optional>());
  REQUIRE(!e.isDefault<Optional>());

  e.unset<Optional>();
  REQUIRE(!e.has<Optional>());
  REQUIRE(!e.isDefault<Optional>());
}

TEST_CASE("default") {
  TestElement e;
  REQUIRE(e.has<Default>());
  REQUIRE(e.get<Default>() == 42);
  REQUIRE(e.isDefault<Default>());

  e.set(Default{5});
  REQUIRE(e.has<Default>());
  REQUIRE(e.get<Default>() == 5);
  REQUIRE(!e.isDefault<Default>());

  e.unset<Default>();
  REQUIRE(e.has<Default>());
  REQUIRE(e.get<Default>() == 42);
  REQUIRE(e.isDefault<Default>());
}

TEST_CASE("vector") {
  TestElement e;
  REQUIRE(!e.has<Vectors>());
  REQUIRE(e.get<Vectors>() == Vectors{});
  REQUIRE(!e.isDefault<Vectors>());

  e.set(Vectors{{Vector{5}}});
  REQUIRE(e.has<Vectors>());
  REQUIRE(e.get<Vectors>() == Vectors{{Vector{5}}});
  REQUIRE(!e.isDefault<Vectors>());

  e.add(Vector{6});
  REQUIRE(e.has<Vectors>());
  REQUIRE(e.get<Vectors>() == Vectors{{Vector{5}, Vector{6}}});
  REQUIRE(!e.isDefault<Vectors>());

  e.remove(Vector{5});
  REQUIRE(e.has<Vectors>());
  REQUIRE(e.get<Vectors>() == Vectors{{Vector{6}}});
  REQUIRE(!e.isDefault<Vectors>());

  e.unset<Vectors>();
  REQUIRE(!e.has<Vectors>());
  REQUIRE(e.get<Vectors>() == Vectors{});
  REQUIRE(!e.isDefault<Vectors>());
}

TEST_CASE("variant") {
  TestElement e;
  REQUIRE(!e.has<Variant>());
  REQUIRE(!e.isDefault<Variant>());

  // set from the variant type
  e.set(Variant{VariantA{1}});
  REQUIRE(boost::get<VariantA>(e.get<Variant>()).x == 1);
  REQUIRE(e.has<VariantA>());
  REQUIRE(!e.has<VariantB>());
  REQUIRE(e.get<VariantA>().x == 1);

  e.set(Variant{VariantB{2}});
  REQUIRE(boost::get<VariantB>(e.get<Variant>()).x == 2);
  REQUIRE(!e.has<VariantA>());
  REQUIRE(e.has<VariantB>());
  REQUIRE(e.get<VariantB>().x == 2);

  // set from the contained types
  e.set(VariantA{3});
  REQUIRE(e.get<VariantA>().x == 3);

  e.set(VariantB{4});
  REQUIRE(e.get<VariantB>().x == 4);
}
