/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : functions
 * @created     : Sunday Dec 13, 2020 21:54:35 CET
 * @license     : MIT
 * */

#ifndef BRUN_CALLABLES_FUNCTIONS_HPP
#define BRUN_CALLABLES_FUNCTIONS_HPP

#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
#define HAS_MD_SUBSCRIPT 1
#else
#define HAS_MD_SUBSCRIPT 0
#endif

#include <utility>
#include <functional>
#include "detail/partial.hpp"
#include "detail/functional.hpp"

namespace brun
{
#define FWD(x) std::forward<decltype(x)>(x)

// apply
// compose
// identity
// construct
// get
// at

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................APPLY.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct apply_fn : public binary_fn<apply_fn>
{
    template <typename Fn, typename Tuple>
        requires detail::direct_applicable<Fn, Tuple>
    constexpr CB_STATIC
    auto operator()(Fn && fn, Tuple && args) CB_CONST
        noexcept(noexcept(apply(FWD(fn), FWD(args))))
        -> decltype(auto)
    { return apply(FWD(fn), FWD(args)); }

    template <typename Fn, typename T>
        requires detail::has_member_apply_with<T, Fn>
    constexpr CB_STATIC
    auto operator()(Fn && fn, T && obj) CB_CONST
        noexcept(noexcept(FWD(obj).apply(FWD(fn))))
        -> decltype(auto)
    { return FWD(obj).apply(FWD(fn)); }

    using binary_fn<apply_fn>::operator();
};

constexpr inline apply_fn apply;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................COMPOSE................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct compose_fn
{
    template <typename ...Fns>
    struct compose_fn_capture
    {
    private:
        std::tuple<Fns...> _fns;

        struct uncallable {};

        template <typename ...Args>
        constexpr
        static auto call_with_args(std::tuple<Fns...> & tp, Args &&... args) -> decltype(auto)
        { return _call_impl<0>(tp, std::forward<Args>(args)...); }

        template <typename ...Args>
        constexpr
        static auto call_with_args(std::tuple<Fns...> const & tp, Args &&... args) -> decltype(auto)
        { return _call_impl<0>(tp, std::forward<Args>(args)...); }

        template <std::size_t I, typename Tuple, typename ...Args>
        constexpr
        static auto _call_impl(Tuple && fns, Args &&... args) -> decltype(auto)
        {
#           define GET(idx, tpl) std::get<idx>(std::forward<Tuple>(tpl))
            if constexpr (I == sizeof...(Fns) - 1) {
                if constexpr (std::invocable<std::tuple_element_t<I, std::remove_cvref_t<Tuple>>, Args...>) {
                    return GET(I, fns)(FWD(args)...);
                } else if constexpr (sizeof...(Fns) != 1) {
                    return uncallable{};
                }
            } else {
                using nested_call_result = decltype(_call_impl<I + 1>(FWD(fns), FWD(args)...));
                if constexpr (not std::same_as<nested_call_result, uncallable>) {
                    return GET(I, fns)(_call_impl<I + 1>(FWD(fns), FWD(args)...));
                } else {
                    using fn_t = std::tuple_element_t<I, std::remove_cvref_t<Tuple>>;
                    if constexpr ((std::invocable<fn_t, decltype(_call_impl<I + 1>(FWD(fns), FWD(args)))...>)) {
                        return GET(I, fns)(_call_impl<I + 1>(FWD(fns),FWD(args))...);
                    } else if constexpr (I != 0) {
                        return uncallable{};
                    }
                }
            }
#           undef GET
        }
    public:
        template <typename ...Fns2>
            requires detail::pairwise_constructible<Fns...>::template from<Fns2...>
        constexpr
        compose_fn_capture(Fns2 &&... fns) : _fns{std::forward<Fns2>(fns)...} {}

        template <typename ...Args>
        constexpr auto operator()(Args &&... args) const -> decltype(auto)
        { return call_with_args(_fns, std::forward<Args>(args)...); }

        template <typename ...Args>
        constexpr auto operator()(Args &&... args) -> decltype(auto)
        { return call_with_args(_fns, std::forward<Args>(args)...); }
    };

    template <typename ...Fns>
    constexpr CB_STATIC
    auto operator()(Fns &&... fns) CB_CONST noexcept
    {
        return compose_fn_capture<Fns...>{std::forward<Fns>(fns)...};
    }
};

constexpr inline compose_fn compose;

#if 0
static_assert(compose([](auto x) { return x + 1; }, [](auto a, auto b) { return a + b; })(0, 2) == 3);
static_assert(compose([](auto c) { return c - 'z'; }, [](auto c) { return c + 'z'; })('c') == 'c');
static_assert(compose(+[](int a, int b) { return a + b; }, +[](int x) { return x; })(0, 0) == 0);
#endif

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................IDENTITY.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct identity_fn
{
    using is_transparent = void;

    template <typename T>
    constexpr CB_STATIC auto operator()(T && t) CB_CONST noexcept
        -> decltype(auto)
    { return std::forward<T &&>(t); }
};

constexpr inline identity_fn identity;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................CONSTRUCT.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
template <typename T>
struct construct_fn
{
private:
    struct from_tuple_fn
    {
        template <typename ...Args>
            requires std::constructible_from<T, Args...>
        constexpr CB_STATIC
        auto operator()(std::tuple<Args...> && args) CB_CONST
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            -> decltype(auto)
        {
            return [args=std::move(args)]<std::size_t ...I>(std::index_sequence<I...>) {
                return T(std::get<I>(args)...);
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        template <typename ...Args>
            requires std::constructible_from<T, Args...>
        constexpr CB_STATIC
        auto operator()(std::tuple<Args...> const & args) CB_CONST
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            -> decltype(auto)
        {
            return [args=std::move(args)]<std::size_t ...I>(std::index_sequence<I...>) {
                return T(std::get<I>(args)...);
            }(std::make_index_sequence<sizeof...(Args)>{});
        }
    };

public:
    [[no_unique_address]] from_tuple_fn from_tuple;

    template <typename ...Args>
        requires std::constructible_from<T, Args...>
    constexpr CB_STATIC
    auto operator()(Args &&... args) CB_CONST
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
        -> decltype(auto)
    { return T(std::forward<Args>(args)...); }
};

template <typename T>
constexpr inline construct_fn<T> construct;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................GET..................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
template <std::size_t N>
struct get_fn
{
    template <typename Obj>
    constexpr CB_STATIC
    auto operator()(Obj && obj) CB_CONST noexcept(noexcept(get<N>(std::forward<Obj>(obj))))
        -> decltype(auto)
    { return get<N>(std::forward<Obj>(obj)); }
};

template <std::size_t N>
constexpr inline get_fn<N> get;


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .....................................AT..................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct at_fn
{
    template <typename T, typename ...>
    struct first_ { using type = T; };

    template <typename ...N>
    struct use_member
    {
        using storage_t = std::conditional_t<sizeof...(N) != 1, std::tuple<N...>, typename first_<N...>::type>;
        storage_t indices;

        template <typename Obj>
        constexpr auto operator()(Obj && obj) -> decltype(auto)
        {
            if constexpr (sizeof...(N) == 1) {
                return at_fn{}(FWD(obj), indices);
            } else {
                return std::apply([obj=FWD(obj)](N &&... args) {
                    return at_fn{}(FWD(obj), FWD(args)...);
                }, indices);
            }
        }
    };

#if HAS_MD_SUBSCRIPT
    template <typename ...N>
    struct use_op
    {
        using storage_t = std::conditional_t<sizeof...(N) != 1, std::tuple<N...>, typename first_<N...>::type>;
        storage_t indices;

        template <typename Obj>
        constexpr auto operator()(Obj && obj) -> decltype(auto)
        {
            if constexpr (sizeof...(N) == 1) {
                return at_fn{}[FWD(obj), indices];
            } else {
                return std::apply([obj=FWD(obj)](N &&... args) {
                    return at_fn{}[FWD(obj), FWD(args)...];
                }, indices);
            }
        }
    };
#else
    template <typename N>
    struct use_op
    {
        N index;

        template <typename Obj>
        constexpr auto operator()(Obj && obj) -> decltype(auto)
        {
            return FWD(obj)[index];
        }
    };
#endif


    template <typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator()(N &&... n) CB_CONST noexcept -> decltype(auto)
    { return use_member<std::unwrap_ref_decay_t<N>...>{FWD(n)...}; }

    template <typename Obj, typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator()(Obj && obj, N &&... n) CB_CONST noexcept(noexcept(FWD(obj).at(FWD(n)...)))
        -> decltype(auto)
    { return FWD(obj).at(FWD(n)...); }

#if HAS_MD_SUBSCRIPT
    template <typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator[](N &&... n) CB_CONST noexcept -> decltype(auto)
    { return use_op<std::unwrap_ref_decay_t<N>...>{FWD(n)...}; }

    template <typename Obj, typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator[](Obj && obj, N &&... n) CB_CONST noexcept(noexcept(FWD(obj)[FWD(n)...]))
        -> decltype(auto)
    { return FWD(obj)[FWD(n)...]; }
#else
    template <typename N>
    constexpr CB_STATIC
    auto operator[](N && n) CB_CONST noexcept -> decltype(auto)
    { return use_op<std::unwrap_ref_decay_t<N>>{FWD(n)}; }

    template <typename Obj, typename N>
    constexpr CB_STATIC
    auto operator[](Obj && obj, N && n) CB_CONST noexcept(noexcept(FWD(obj)[FWD(n)]))
        -> decltype(auto)
    { return FWD(obj)[FWD(n)]; }
#endif
};

constexpr inline at_fn at;

#undef FWD
} // namespace brun

#endif /* BRUN_CALLABLES_FUNCTIONS_HPP */
