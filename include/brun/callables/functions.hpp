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
        constexpr auto operator()(Tuple && tp) const noexcept(noexcept(std::apply(_fn, std::forward<Tuple>(tp))))
            -> decltype(auto)
        { return std::apply(_fn, std::forward<Tuple>(tp)); }
    };

    template <typename Fn, typename Tuple>
        requires detail::applicable<Fn, Tuple>
    constexpr
    auto operator()(Fn && fn, Tuple && args) const noexcept(noexcept(std::apply(std::forward<Fn>(fn), std::forward<Tuple>(args))))
        -> decltype(auto)
    { return std::apply(std::forward<Fn>(fn), std::forward<Tuple>(args)); }

    template <typename Fn>
    constexpr
    auto operator()(Fn && fn) const noexcept(noexcept(curried{std::forward<Fn>(fn)}))
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
    template <typename Fn1, typename Fn2>
    struct compose_fn_capture
    {
    private:
        Fn1 _fn1;
        Fn2 _fn2;

    public:
        template <typename F1, typename F2>
            requires std::constructible_from<Fn1, F1>
                 and std::constructible_from<Fn2, F2>
        constexpr
        compose_fn_capture(F1 && f1, F2 && f2) :
            _fn1{std::forward<F1>(f1)},
            _fn2{std::forward<F2>(f2)}
        {}

        template <typename ...Args>
            requires std::invocable<Fn2, Args...>
                 and std::invocable<Fn1, std::invoke_result_t<Fn2, Args...>>
        constexpr
        auto operator()(Args &&... args) const
            noexcept(std::is_nothrow_invocable_v<Fn1, std::invoke_result_t<Fn2, Args...>>)
            -> decltype(auto)
        {
            return std::invoke(_fn1, std::invoke(_fn2, std::forward<Args>(args)...));
        }
    };

    template <typename Fn1, typename Fn2>
    constexpr
    auto operator()(Fn1 && fn1, Fn2 && fn2) const noexcept
    {
        return compose_fn_capture<Fn1, Fn2>{std::forward<Fn1>(fn1), std::forward<Fn2>(fn2)};
    }
};

constexpr inline compose_fn compose;

static_assert(compose([](auto x) { return x + 1; }, [](auto a, auto b) { return a + b; })(0, 2) == 3);
static_assert(compose([](auto c) { return c - 'z'; }, [](auto c) { return c + 'z'; })('c') == 'c');

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................IDENTITY.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct identity_fn
{
    using is_transparent = void;

    template <typename T>
    constexpr auto operator()(T && t) const noexcept
    { return std::forward<T &&>(t); }
};

constexpr inline identity_fn identity;

} // namespace brun

#endif /* BRUN_CALLABLES_FUNCTIONS_HPP */
