/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : functions
 * @created     : Sunday Dec 13, 2020 21:54:35 CET
 * @license     : MIT
 * */

#ifndef BRUN_CALLABLES_FUNCTIONS_HPP
#define BRUN_CALLABLES_FUNCTIONS_HPP

#include <utility>
#include <functional>
#include "detail.hpp"
#include "detail/functional.hpp"

namespace brun
{

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................APPLY.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct apply_fn
{
    template <typename Fn>
    struct curried
    {
    private:
        Fn _fn;

    public:
        constexpr explicit curried(Fn fn) : _fn{std::move(fn)} {}

        template <typename Tuple>
            requires detail::applicable<Fn, Tuple>
        constexpr
        auto operator()(Tuple && tp)
            noexcept(noexcept(apply_fn{}(_fn, std::forward<Tuple>(tp))))
            -> decltype(auto)
        { return apply_fn{}(_fn, std::forward<Tuple>(tp)); }
    };

    template <typename Fn, typename Tuple>
        requires detail::direct_applicable<Fn, Tuple>
    constexpr CB_STATIC
    auto operator()(Fn && fn, Tuple && args) CB_CONST
        noexcept(noexcept(apply(std::forward<Fn>(fn), std::forward<Tuple>(args))))
        -> decltype(auto)
    { return apply(std::forward<Fn>(fn), std::forward<Tuple>(args)); }

    template <typename Fn, typename T>
        requires detail::has_member_apply_with<T, Fn>
    constexpr CB_STATIC
    auto operator()(Fn && fn, T && obj) CB_CONST
        noexcept(noexcept(std::forward<T>(obj).apply(std::forward<Fn>(fn))))
        -> decltype(auto)
    { return std::forward<T>(obj).apply(std::forward<Fn>(fn)); }

    template <typename Fn>
    constexpr CB_STATIC
    auto operator()(Fn && fn) CB_CONST
        noexcept(noexcept(curried{std::forward<Fn>(fn)}))
        -> decltype(auto)
    {
        return curried{std::forward<Fn>(fn)};
    }
};

constexpr inline apply_fn apply;

static_assert(apply([](auto x, auto y) { return x * x + y * y; })(std::tuple{4, 3}) == 16 + 9);


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
#           define FWD(x) std::forward<decltype(x)>(x)
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
#           undef FWD
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

static_assert(compose([](auto x) { return x + 1; }, [](auto a, auto b) { return a + b; })(0, 2) == 3);
static_assert(compose([](auto c) { return c - 'z'; }, [](auto c) { return c + 'z'; })('c') == 'c');
static_assert(compose(+[](int a, int b) { return a + b; }, +[](int x) { return x; })(0, 0) == 0);

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................IDENTITY.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct identity_fn
{
    using is_transparent = void;

    template <typename T>
    constexpr CB_STATIC auto operator()(T && t) CB_CONST noexcept
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
    { return T(std::forward<Args>(args)...); }
};

template <typename T>
constexpr inline construct_fn<T> construct;

} // namespace brun

#endif /* BRUN_CALLABLES_FUNCTIONS_HPP */
