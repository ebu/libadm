#pragma once

#include "adm/detail/enum_bitmask.hpp"
#include <functional>
#include <type_traits>
#include <tuple>

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

    template <typename List>
    struct CarImpl;

    template <typename Head, typename... Tail>
    struct CarImpl<TypeList<Head, Tail...>> {
      using type = Head;
    };

    // Get the Head element of a TypeList
    template <typename... Ts>
    using Car = typename CarImpl<TypeList<Ts...>>::type;

    template <typename Tail>
    struct CdrImpl;

    template <typename Head, typename... Tail>
    struct CdrImpl<TypeList<Head, Tail...>> {
      using type = TypeList<Tail...>;
    };

    // Get the Tail... Elements of a TypeList (blame lisp)
    template <typename... Ts>
    using Cdr = typename CdrImpl<TypeList<Ts...>>::type;

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

    // Declaration
    template <template <typename, typename> typename MatchPredicate,
              typename TargetT, typename CandidateTs, typename Enable = void>
    struct FindType;

    // Match found, Success
    template <template <typename, typename> typename MatchPredicate,
              typename TargetT, typename... CandidateTs>
    struct FindType<
        MatchPredicate, TargetT, TypeList<CandidateTs...>,
        std::enable_if_t<MatchPredicate<TargetT, Car<CandidateTs...>>::value>> {
      static constexpr bool success{true};
      using type = Car<CandidateTs...>;
    };

    // No match, but not end of list, so try again with next Candidate
    template <template <typename, typename> typename MatchPredicate,
              typename TargetT, typename... CandidateTs>
    struct FindType<MatchPredicate, TargetT, TypeList<CandidateTs...>,
                    std::enable_if_t<
                        !MatchPredicate<TargetT, Car<CandidateTs...>>::value>> {
      static constexpr bool success{
          FindType<MatchPredicate, TargetT, Cdr<CandidateTs...>>::success};
      using type =
          typename FindType<MatchPredicate, TargetT, Cdr<CandidateTs...>>::type;
    };

    // No match and at end of list, failure case
    template <template <typename, typename> typename MatchPredicate,
              typename TargetT>
    struct FindType<MatchPredicate, TargetT, TypeList<>> {
      static constexpr bool success{false};
      using type = void;
    };

    // Find, from a list of types, an argument type that can be used
    // to initialize the given base type
    template <typename BaseT, typename ArgT>
    struct FindArg {
      static constexpr bool value = std::is_same_v<
          typename BaseT::ParameterType,
          typename std::remove_reference_t<std::remove_cv_t<ArgT>>>;
    };

    // Find, from a list of types, a base type that can be initialised
    // by the given argument type
    template <typename ArgT, typename BaseT>
    struct FindBase {
      static constexpr bool value = std::is_same_v<
          typename std::remove_reference_t<std::remove_cv_t<ArgT>>,
          typename BaseT::ParameterType>;
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
      template <typename... Args>
      explicit Combine(Args... args)
          : Bases(std::invoke(
                // this lambda will be called once per base class due to pack
                // expansion '...' at the end of the 'Bases' initializer expression
                [&args...]() {
                  using FindResult =
                      FindType<FindArg, Bases, TypeList<Args...>>;
                  constexpr bool found = FindResult::success;
                  using Match = typename FindResult::type;

                  if constexpr (!found) {
                    (..., (void)args);  // avoid unused capture warning
                    // Base parameter not passed as argument, so default construct
                    return Bases{};
                  } else {
                    // Initialize base class from matching argument
                    // I'm sure there's better way to do this
                    // std::tie (and add_lvalue_reference<Match>) might be preferable
                    // if any arguments can't be copied?
                    // Maybe the tuple construction gets optimized away.
                    auto arr = std::make_tuple(args...);
                    return std::get<Match>(arr);
                  }
                }))... {}

     protected:
      // This wouldn't work using CRTP from the ctor as the derived type would
      // not be initialised at base type construction.
      template <typename Derived, typename... Args>
      void init(Derived& derived, Args&&... args) {
        // lambda called once per argument due to fold expression
        (..., [&derived](auto arg) {
          using FindResult =
              FindType<FindBase, decltype(arg), TypeList<Bases...>>;
          constexpr bool found = FindResult::success;
          if constexpr (!found) {
            derived.set(std::forward<decltype(arg)>(arg));
          }
        }(args));
      }
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
