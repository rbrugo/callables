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

#define FWD(x) std::forward<decltype(x)>(x)
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
        constexpr explicit curried(U && u) : _t{FWD(u)} {}

        template <typename U>
            requires std::regular_invocable<less_fn, T, U>
                 and (not detail::can_select_integral_overload<less_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t < FWD(u)))
        { return _t < FWD(u); }

        template <typename U>
            requires std::regular_invocable<less_fn, U, T>
                 and (not detail::can_select_integral_overload<less_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) < _t))
        { return FWD(u) < _t; }

        template <typename U>
            requires detail::can_select_integral_overload<less_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less(_t, FWD(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<less_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less(FWD(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) < FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) < FWD(u)))
        -> decltype(auto)
    { return FWD(t) < FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) < std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) < std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(curried<T, true>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(curried<T, false>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
        -> decltype(auto)
    { return left(FWD(t)); }

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
        constexpr explicit curried(U && u) : _t{FWD(u)} {}

        template <typename U>
            requires std::regular_invocable<less_equal_fn, T, U>
                 and (not detail::can_select_integral_overload<less_equal_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t <= FWD(u)))
        { return _t <= FWD(u); }

        template <typename U>
            requires std::regular_invocable<less_equal_fn, T, U>
                 and (not detail::can_select_integral_overload<less_equal_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) <= _t))
        { return FWD(u) <= _t; }

        template <typename U>
            requires detail::can_select_integral_overload<less_equal_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less_equal(_t, FWD(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<less_equal_fn, U, T> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_less_equal(FWD(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) <= FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) <= FWD(u)))
        -> decltype(auto)
    { return FWD(t) <= FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) <= std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) <= std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(curried<T, true>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(curried<T, false>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
        -> decltype(auto)
    { return left(FWD(t)); }

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
        constexpr explicit curried(U && u) : _t{FWD(u)} {}

        template <typename U>
            requires std::regular_invocable<greater_fn, T, U>
                 and (not detail::can_select_integral_overload<greater_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t > FWD(u)))
        { return _t > FWD(u); }

        template <typename U>
            requires std::regular_invocable<greater_fn, T, U>
                 and (not detail::can_select_integral_overload<greater_fn, U, T>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) > _t))
        { return FWD(u) > _t; }

        template <typename U>
            requires detail::can_select_integral_overload<greater_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater(_t, FWD(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<greater_fn, U, T> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater(FWD(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) > FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) > FWD(u)))
        -> decltype(auto)
    { return FWD(t) > FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) > std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) > std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(curried<T, true>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(curried<T, false>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
        -> decltype(auto)
    { return left(FWD(t)); }

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
        constexpr explicit curried(U && u) : _t{FWD(u)} {}

        template <typename U>
            requires std::regular_invocable<greater_equal_fn, T, U>
                 and (not detail::can_select_integral_overload<greater_equal_fn, T, U>)
                 and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t >= FWD(u)))
        { return _t >= FWD(u); }

        template <typename U>
            requires std::regular_invocable<greater_equal_fn, U, T>
                 and (not detail::can_select_integral_overload<greater_equal_fn, T, U>)
                 and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) >= _t))
        { return FWD(u) >= _t; }

        template <typename U>
            requires detail::can_select_integral_overload<greater_equal_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater_equal(_t, FWD(u)); }

        template <typename U>
            requires detail::can_select_integral_overload<greater_equal_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept
        { return std::cmp_greater_equal(FWD(u), _t); }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) >= FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) >= FWD(u)))
        -> decltype(auto)
    { return FWD(t) >= FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr
    auto operator()(Tuple && p) const noexcept(noexcept(std::get<0>(FWD(p)) >= std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) >= std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(curried<T, true>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(curried<T, false>{FWD(t)}))
        -> decltype(auto)
    { return curried<T, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
        -> decltype(auto)
    { return left(FWD(t)); }

    using is_transparent = void;
};

constexpr inline greater_equal_fn greater_equal;

#undef FWD
} // namespace brun

#endif /* BRUN_CALLABLES_ORDERING_HPP */
