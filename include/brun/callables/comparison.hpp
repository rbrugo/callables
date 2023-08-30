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

#define FWD(x) std::forward<decltype(x)>(x)
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................EQUAL_TO.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct equal_to_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) : _t{FWD(u)} {}

        template <typename U>
            requires std::regular_invocable<equal_to_fn, T, U>
                 and (not detail::can_select_integral_overload<equal_to_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t == FWD(u)))
        { return _t == FWD(u); }

        template <typename U>
            requires std::regular_invocable<equal_to_fn, U, T>
                 and (not detail::can_select_integral_overload<equal_to_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) == _t))
        { return FWD(u) == _t; }

        template <typename U>
            requires detail::can_select_integral_overload<equal_to_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_equal(_t, FWD(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<equal_to_fn, U, T> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_equal(FWD(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) == FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) == FWD(u)))
        -> decltype(auto)
    { return FWD(t) == FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) == std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) == std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }

    using is_transparent = void;
};

constexpr inline equal_to_fn equal_to;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................NOT_EQUAL_TO.............................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct not_equal_to_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) : _t{FWD(u)} {}

        template <typename U>
            requires std::regular_invocable<not_equal_to_fn, T, U>
                 and (not detail::can_select_integral_overload<not_equal_to_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t != FWD(u)))
        { return _t != FWD(u); }

        template <typename U>
            requires std::regular_invocable<not_equal_to_fn, U, T>
                 and (not detail::can_select_integral_overload<not_equal_to_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) != _t))
        { return FWD(u) != _t; }

        template <typename U>
            requires detail::can_select_integral_overload<not_equal_to_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_not_equal(_t, FWD(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<not_equal_to_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_not_equal(FWD(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) != FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) != FWD(u)))
        -> decltype(auto)
    { return FWD(t) != FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) != std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) != std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }

    using is_transparent = void;
};

constexpr inline not_equal_to_fn not_equal_to;

} // namespace brun

#endif /* BRUN_CALLABLES_COMPARISON_HPP */
