/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : comparison
 * @created     : Wednesday Dec 15, 2021 17:54:27 CET
 * @description :
 * */

#ifndef BRUN_CALLABLES_COMPARISON_HPP
#define BRUN_CALLABLES_COMPARISON_HPP

#include <utility>
#include "detail.hpp"

namespace brun
{

// equal_to
// not_equal_to

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................EQUAL_TO.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct equal_to_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U>
            requires std::regular_invocable<equal_to_fn, T, U>
                 and (not detail::can_select_integral_overload<equal_to_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t == std::forward<U>(u)))
        { return _t == std::forward<U>(u); }

        template <typename U>
            requires std::regular_invocable<equal_to_fn, U, T>
                 and (not detail::can_select_integral_overload<equal_to_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) == _t))
        { return std::forward<U>(u) == _t; }

        template <typename U>
            requires detail::can_select_integral_overload<equal_to_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_equal(_t, std::forward<U>(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<equal_to_fn, U, T> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_equal(std::forward<U>(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) == std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) == std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) == std::forward<U>(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr
    auto operator()(Tuple && p) const noexcept(noexcept(std::apply(*this, std::forward<Tuple>(p))))
        -> decltype(auto)
    { return std::apply(*this, std::forward<Tuple>(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(curried<T, false>{std::forward<T>(t)}))
        -> decltype(auto)
    { return curried<T, true>{std::forward<T>(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(curried<T, false>{std::forward<T>(t)}))
        -> decltype(auto)
    { return curried<T, false>{std::forward<T>(t)}; }

    template <typename T>
    constexpr
    auto operator()(T && t) const noexcept(noexcept(left(std::forward<T>(t))))
        -> decltype(auto)
    { return left(std::forward<T>(t)); }

    using is_transparent = void;
};

constexpr inline equal_to_fn equal_to;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................NOT_EQUAL_TO.............................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct not_equal_to_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U>
            requires std::regular_invocable<not_equal_to_fn, T, U>
                 and (not detail::can_select_integral_overload<not_equal_to_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t != std::forward<U>(u)))
        { return _t != std::forward<U>(u); }

        template <typename U>
            requires std::regular_invocable<not_equal_to_fn, U, T>
                 and (not detail::can_select_integral_overload<not_equal_to_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) != _t))
        { return std::forward<U>(u) != _t; }

        template <typename U>
            requires detail::can_select_integral_overload<not_equal_to_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_not_equal(_t, std::forward<U>(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<not_equal_to_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_not_equal(std::forward<U>(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) != std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) != std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) != std::forward<U>(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr
    auto operator()(Tuple && p) const noexcept(noexcept(std::apply(*this, std::forward<Tuple>(p))))
        -> decltype(auto)
    { return std::apply(*this, std::forward<Tuple>(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(curried<T, true>{std::forward<T>(t)}))
        -> decltype(auto)
    { return curried<T, true>{std::forward<T>(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(curried<T, false>{std::forward<T>(t)}))
        -> decltype(auto)
    { return curried<T, false>{std::forward<T>(t)}; }

    template <typename T>
    constexpr
    auto operator()(T && t) const noexcept(noexcept(left(std::forward<T>(t))))
        -> decltype(auto)
    { return left(std::forward<T>(t)); }

    using is_transparent = void;
};

constexpr inline not_equal_to_fn not_equal_to;

} // namespace brun

#endif /* BRUN_CALLABLES_COMPARISON_HPP */

