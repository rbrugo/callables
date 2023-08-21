/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : bit_operators
 * @created     : Friday Dec 23, 2022 21:12:51 CET
 * @description : 
 * */

#ifndef BRUN_CALLABLES_BIT_OPERATORS_HPP
#define BRUN_CALLABLES_BIT_OPERATORS_HPP

#include "detail.hpp"

namespace brun
{

// bit_and
// bit_or
// bit_xor

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_and................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_and_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<bit_and_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t & std::forward<U>(u)))
        { return _t & std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<bit_and_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) & _t))
        { return std::forward<U>(u) & _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) & std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) & std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) & std::forward<U>(u); }

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
};

constexpr inline bit_and_fn bit_and;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_or.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_or_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t | std::forward<U>(u)))
        { return _t | std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) | _t))
        { return std::forward<U>(u) | _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) | std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) & std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) | std::forward<U>(u); }

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
};

constexpr inline bit_or_fn bit_or;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_xor................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_xor_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t ^ std::forward<U>(u)))
        { return _t ^ std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) ^ _t))
        { return std::forward<U>(u) ^ _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) ^ std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) & std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) ^ std::forward<U>(u); }

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
};

constexpr inline bit_xor_fn bit_xor;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_not................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_not_fn
{
    template <typename T>
        requires requires(T && t) { -std::forward<T>(t); }
    constexpr auto operator()(T && t) const noexcept(noexcept( ~std::forward<T>(t) ))
    { return ~std::forward<T>(t); }
};

constexpr inline bit_not_fn bit_not;

} // namespace brun

#endif /* BRUN_CALLABLES_BIT_OPERATORS_HPP */
