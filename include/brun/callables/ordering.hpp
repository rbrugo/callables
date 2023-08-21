/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : ordering
 * @created     : Monday Dec 13, 2021 13:19:39 CET
 * @description :
 * */

#ifndef BRUN_CALLABLES_ORDERING_HPP
#define BRUN_CALLABLES_ORDERING_HPP

#include <utility>
#include "comparison.hpp"
#include "detail.hpp"

namespace brun
{

// less / less_equal
// greater / greater_equal

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................LESS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct less_fn
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
            requires std::regular_invocable<less_fn, T, U>
                 and (not detail::can_select_integral_overload<less_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t < std::forward<U>(u)))
        { return _t < std::forward<U>(u); }

        template <typename U>
            requires std::regular_invocable<less_fn, U, T>
                 and (not detail::can_select_integral_overload<less_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) < _t))
        { return std::forward<U>(u) < _t; }

        template <typename U>
            requires detail::can_select_integral_overload<less_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less(_t, std::forward<U>(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<less_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less(std::forward<U>(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) < std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) < std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) < std::forward<U>(u); }

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

constexpr inline less_fn less;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................LESS_EQUAL................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct less_equal_fn
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
            requires std::regular_invocable<less_equal_fn, T, U>
                 and (not detail::can_select_integral_overload<less_equal_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t <= std::forward<U>(u)))
        { return _t <= std::forward<U>(u); }

        template <typename U>
            requires std::regular_invocable<less_equal_fn, T, U>
                 and (not detail::can_select_integral_overload<less_equal_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) <= _t))
        { return std::forward<U>(u) <= _t; }

        template <typename U>
            requires detail::can_select_integral_overload<less_equal_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less_equal(_t, std::forward<U>(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<less_equal_fn, U, T> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less_equal(std::forward<U>(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) <= std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) <= std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) <= std::forward<U>(u); }

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

constexpr inline less_equal_fn less_equal;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................GREATER................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct greater_fn
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
            requires std::regular_invocable<greater_fn, T, U>
                 and (not detail::can_select_integral_overload<greater_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t > std::forward<U>(u)))
        { return _t > std::forward<U>(u); }

        template <typename U>
            requires std::regular_invocable<greater_fn, T, U>
                 and (not detail::can_select_integral_overload<greater_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) > _t))
        { return std::forward<U>(u) > _t; }

        template <typename U>
            requires detail::can_select_integral_overload<greater_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater(_t, std::forward<U>(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<greater_fn, U, T> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater(std::forward<U>(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) > std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) > std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) > std::forward<U>(u); }

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

constexpr inline greater_fn greater;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...............................GREATER_EQUAL................................ //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct greater_equal_fn
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
            requires std::regular_invocable<greater_equal_fn, T, U>
                 and (not detail::can_select_integral_overload<greater_equal_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t >= std::forward<U>(u)))
        { return _t >= std::forward<U>(u); }

        template <typename U>
            requires std::regular_invocable<greater_equal_fn, U, T>
                 and (not detail::can_select_integral_overload<greater_equal_fn, T, U>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) >= _t))
        { return std::forward<U>(u) >= _t; }

        template <typename U>
            requires detail::can_select_integral_overload<greater_equal_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater_equal(_t, std::forward<U>(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<greater_equal_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater_equal(std::forward<U>(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) >= std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) >= std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) >= std::forward<U>(u); }

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

constexpr inline greater_equal_fn greater_equal;


} // namespace brun

#endif /* BRUN_CALLABLES_ORDERING_HPP */
