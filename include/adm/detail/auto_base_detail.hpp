#include "boost/variant.hpp"

namespace adm {
  namespace detail {
    /// get the first type parameter of a variant
    template <typename V>
    struct VariantFirstTypeT;

    template <typename T, typename... Ts>
    struct VariantFirstTypeT<boost::variant<T, Ts...>> {
      using type = T;
    };

    template <typename V>
    using VariantFirstType = typename VariantFirstTypeT<V>::type;

    // HasParameters implementation

    /// subclass of A and B
    template <typename A, typename B>
    struct CombineRaw : public A, public B {};

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

    /// a subclass of A and B, with using declarations for set, get and has
    template <typename A, typename B>
    struct CombineGetSetHasB : public A, public B {
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

    /// a subclass of A and B, with using declarations for isDefault and unset
    template <typename A, typename B>
    struct CombineIsDefaultUnsetB : public A, public B {
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

    /// a subclass of A and B, with using declarations for add and remove
    template <typename A, typename B>
    struct CombineAddRemoveB : public A, public B {
      using A::add;
      using B::add;

      using A::remove;
      using B::remove;
    };

    /// helper to combine any number of Parameter classes
    ///
    /// this is seperate from Combine so that for one Base (the base case) the
    /// class can be returned directly without subclassesg
    template <typename... Bases>
    struct HasParametersT;

    /// make a base class for Combine which chains the Combine classes
    template <typename A, typename... Tail>
    struct MakeCombineBase {
      using Base = typename HasParametersT<Tail...>::type;

      // need to combine if both bases have the same methods
      using combine_add_remove =
          std::integral_constant<bool, typename A::has_add_remove() &&
                                           typename Base::has_add_remove()>;
      using combine_get_set_has =
          std::integral_constant<bool, typename A::has_get_set_has() &&
                                           typename Base::has_get_set_has()>;
      using combine_isDefault_unset =
          std::integral_constant<bool,
                                 typename A::has_isDefault_unset() &&
                                     typename Base::has_isDefault_unset()>;

      // chain up to three Combine* classes for the above parameters
      //
      // the rules for these are:
      // - if paramers need combining, then
      //   - if it's the last in the chain, use Combine*B to inherit from A and Base
      //   - otherwise, use Combine* with A and Base, inheriting from the next class in the chain
      // - otherwise use the next class in the chain (ot CombineRaw)

      using IsDefaultUnsetCombined =
          std::conditional_t<combine_isDefault_unset::value,
                             CombineIsDefaultUnsetB<A, Base>,
                             CombineRaw<A, Base>>;

      using GetSetHasCombined = std::conditional_t<
          combine_get_set_has::value,
          std::conditional_t<!combine_isDefault_unset::value,
                             CombineGetSetHasB<A, Base>,
                             CombineGetSetHas<A, Base, IsDefaultUnsetCombined>>,
          IsDefaultUnsetCombined>;

      using AddRemoveCombined = std::conditional_t<
          combine_add_remove::value,
          std::conditional_t<!(combine_isDefault_unset::value ||
                               combine_get_set_has::value),
                             CombineAddRemoveB<A, Base>,
                             CombineAddRemove<A, Base, GetSetHasCombined>>,
          GetSetHasCombined>;

      using type = AddRemoveCombined;
    };

    /// a subclass of at least two Parameter classes, with methods according to
    /// their Flags
    template <typename A, typename... Tail>
    struct Combine : public MakeCombineBase<A, Tail...>::type {
     private:
      using Base = typename HasParametersT<Tail...>::type;

     public:
      using has_get_set_has =
          std::integral_constant<bool, typename A::has_get_set_has() ||
                                           typename Base::has_get_set_has()>;
      using has_isDefault_unset =
          std::integral_constant<bool,
                                 typename A::has_isDefault_unset() ||
                                     typename Base::has_isDefault_unset()>;
      using has_add_remove =
          std::integral_constant<bool, typename A::has_add_remove() ||
                                           typename Base::has_add_remove()>;
    };

    template <typename Ba, typename Bb, typename... BTail>
    struct HasParametersT<Ba, Bb, BTail...> {
      using type = Combine<Ba, Bb, BTail...>;
    };

    template <typename B>
    struct HasParametersT<B> {
      using type = B;
    };

    // VariantTypeParameter implementation

    template <typename VariantParam, typename T>
    class VariantTypeParameter;

    /// implementation of VariantTypeParameterBase, called with the variant
    /// parameter type, the current type of the variant to find the base for,
    /// and the list of types in the variant
    ///
    /// This finds T in Ts, and returns a VariantTypeParameter with the next
    /// type in the list, or P
    template <typename VariantParam, typename T, typename...>
    struct VariantTypeParameterBaseT;

    /// base case: for the last type, inherit from VariantParam
    template <typename VariantParam, typename T, typename Last>
    struct VariantTypeParameterBaseT<VariantParam, T, Last> {
      using type = VariantParam;
    };

    /// recursive case
    template <typename VariantParam, typename T, typename Current,
              typename Next, typename... Rest>
    struct VariantTypeParameterBaseT<VariantParam, T, Current, Next, Rest...> {
      // if the first type in the list is T, then return a VariantTypeParameter
      // for the next type, otherwise remove one and recurse
      using type =
          std::conditional_t<std::is_same<T, Current>::value,
                             VariantTypeParameter<VariantParam, Next>,
                             typename VariantTypeParameterBaseT<
                                 VariantParam, T, Next, Rest...>::type>;
    };

    /// wrapper around VariantTypeParameterBaseT which extracts the types from
    /// the variant
    template <typename VariantParam, typename T, typename Variant>
    struct VariantTypeParameterBaseMatch;

    template <typename VariantParam, typename T, typename... VariantTypes>
    struct VariantTypeParameterBaseMatch<VariantParam, T,
                                         boost::variant<VariantTypes...>> {
      using type = typename VariantTypeParameterBaseT<VariantParam, T,
                                                      VariantTypes...>::type;
    };

    /// the base type for VariantTypeParameter<P<variant<Ts...>>, T>
    ///
    /// This is a  VariantTypeParameter for the next type in Ts, or
    /// P<variant<Ts...>>
    template <typename VariantParam, typename T>
    using VariantTypeParameterBase = typename VariantTypeParameterBaseMatch<
        VariantParam, T, typename VariantParam::ParameterType>::type;
  }  // namespace detail
}  // namespace adm
