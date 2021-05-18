
namespace adm {
  namespace detail {
    /// Combine A and B using F is defined_in_both
    template <bool defined_in_both,
              template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    struct ApplyIfT;

    template <template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    struct ApplyIfT<false, F, A, B, Base> {
      using type = Base;
    };

    template <template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    struct ApplyIfT<true, F, A, B, Base> {
      using type = F<A, B, Base>;
    };

    template <bool defined_in_both,
              template <typename A, typename B, typename Base> class F,
              typename A, typename B, typename Base>
    using ApplyIf = typename ApplyIfT<defined_in_both, F, A, B, Base>::type;

    /// subclass of A and B
    template <typename A, typename B>
    struct CombineRaw : public A, public B {};
  }  // namespace detail
}  // namespace adm
