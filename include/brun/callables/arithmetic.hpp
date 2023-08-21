/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : operators
 * @created     : Monday Dec 13, 2021 13:24:01 CET
 * @description : 
 * */

#ifndef BRUN_CALLABLES_ARITHMETIC_HPP
#define BRUN_CALLABLES_ARITHMETIC_HPP

#include "detail.hpp"

namespace brun
{

// plus
// minus
// multiplies
// divides
// negate

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................PLUS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct plus_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<plus_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t + std::forward<U>(u)))
        { return _t + std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<plus_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) + _t))
        { return std::forward<U>(u) + _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) + std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) + std::forward<U>(u); }

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

constexpr inline plus_fn plus;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................MINUS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct minus_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<minus_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t - std::forward<U>(u)))
        { return _t - std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<minus_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) - _t))
        { return std::forward<U>(u) - _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) - std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) - std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) - std::forward<U>(u); }

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

constexpr inline minus_fn minus;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................MULTIPLIES................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct multiplies_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<multiplies_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t * std::forward<U>(u)))
        { return _t * std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<multiplies_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) * _t))
        { return std::forward<U>(u) * _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) * std::forward<U>(u); }
    constexpr
    auto operator()(T && t, U && u) const noexcept(noexcept(std::forward<T>(t) * std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) * std::forward<U>(u); }

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

constexpr inline multiplies_fn multiplies;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................DIVIDES................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct divides_fn
{
    template <typename T, bool Left>
    struct curried
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit curried(U && u) : _t{std::forward<U>(u)} {}

        template <typename U> requires std::regular_invocable<divides_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t / std::forward<U>(u)))
        { return _t / std::forward<U>(u); }

        template <typename U> requires std::regular_invocable<divides_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(std::forward<U>(u) / _t))
        { return std::forward<U>(u) / _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { std::forward<T>(t) / std::forward<U>(u); }
    constexpr auto operator()(T && t, U && u) const
        noexcept(noexcept(std::forward<T>(t) / std::forward<U>(u)))
        -> decltype(auto)
    { return std::forward<T>(t) / std::forward<U>(u); }

    template <typename Tuple>
        requires (detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>)
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

constexpr inline divides_fn divides;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................NEGATE................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct negate_fn
{
    template <typename T>
        requires requires(T && t) { -std::forward<T>(t); }
    constexpr auto operator()(T && t) const noexcept(noexcept( -std::forward<T>(t) ))
    { return -std::forward<T>(t); }
};

constexpr inline negate_fn negate;

} // namespace brun

#endif /* BRUN_CALLABLES_ARITHMETIC_HPP */
