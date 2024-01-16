#pragma once

#include "adm/detail/enum_bitmask.hpp"
#include <type_traits>

namespace adm {
  namespace detail {
    /// represent a list of types as a single type
    template <typename... T>
    class TypeList {};

    template <typename Head, typename Tail>
    struct ConsImpl;

    template <typename Head, typename... Tail>
    struct ConsImpl<Head, TypeList<Tail...>> {
      using type = TypeList<Head, Tail...>;
    };

    /// prepend Head to TypeList Tail
    template <typename Head, typename Tail>
    using Cons = typename ConsImpl<Head, Tail>::type;

    /// specify which method a parameter has
    enum class Flags : unsigned {
      HAS_GET_SET_HAS = 1,
      HAS_ISDEFAULT_UNSET = 2,
      HAS_ADD_REMOVE = 4,
    };

    template <>
    struct EnableBitMaskOperators<Flags> : public std::true_type {};

    /// From a list of bases as a TypeList, select the ones which have exactly
    /// the specified flags
    template <typename Bases, Flags flags, typename Enable = void>
    struct BasesWithFlags;

    // recursive case when the head has the requested flags
    template <typename Head, typename... Tail, Flags flags>
    struct BasesWithFlags<TypeList<Head, Tail...>, flags,
                          std::enable_if_t<Head::flags == flags>> {
      using type =
          Cons<Head, typename BasesWithFlags<TypeList<Tail...>, flags>::type>;
    };

    // recursive case when the head does not have the requested flags
    template <typename Head, typename... Tail, Flags flags>
    struct BasesWithFlags<TypeList<Head, Tail...>, flags,
                          std::enable_if_t<Head::flags != flags>> {
      using type = typename BasesWithFlags<TypeList<Tail...>, flags>::type;
    };

    // recursive case
    template <Flags flags>
    struct BasesWithFlags<TypeList<>, flags> {
      using type = TypeList<>;
    };

    /// a class inheriting publicly from classes in Bases (a TypeList), with
    /// using declarations for classes specified in the other TypeList
    /// parameters; e.g. classes in AddRemoveBases get a using declaration for
    /// add and remove
    ///
    /// note that the bases are selected based on the combinations of flags
    /// that are actually used, so each base is only in one list, to make the
    /// type names smaller. more lists may need to be added to accommodate new
    /// types of parameters if they use different subsets of methods
    template <
        typename Bases,
        typename GetSetHasBases =
            typename BasesWithFlags<Bases, Flags::HAS_GET_SET_HAS>::type,
        typename IsDefaultUnsetBases = typename BasesWithFlags<
            Bases, Flags::HAS_GET_SET_HAS | Flags::HAS_ISDEFAULT_UNSET>::type,
        typename AddRemoveBases = typename BasesWithFlags<
            Bases, Flags::HAS_GET_SET_HAS | Flags::HAS_ISDEFAULT_UNSET |
                       Flags::HAS_ADD_REMOVE>::type>
    class Combine;

    template <typename... Bases, typename... GetSetHasBases,
              typename... IsDefaultUnsetBases, typename... AddRemoveBases>
    class Combine<TypeList<Bases...>, TypeList<GetSetHasBases...>,
                  TypeList<IsDefaultUnsetBases...>, TypeList<AddRemoveBases...>>
        : public Bases... {
     public:
      using GetSetHasBases::get...;
      using GetSetHasBases::set...;
      using GetSetHasBases::has...;

      using IsDefaultUnsetBases::get...;
      using IsDefaultUnsetBases::set...;
      using IsDefaultUnsetBases::has...;
      using IsDefaultUnsetBases::isDefault...;
      using IsDefaultUnsetBases::unset...;

      using AddRemoveBases::get...;
      using AddRemoveBases::set...;
      using AddRemoveBases::has...;
      using AddRemoveBases::isDefault...;
      using AddRemoveBases::unset...;
      using AddRemoveBases::add...;
      using AddRemoveBases::remove...;
    };
  }  // namespace detail
}  // namespace adm
