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

#define FWD(x) std::forward<decltype(x)>(x)
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................PLUS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct plus_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) noexcept(std::is_nothrow_constructible_v<T, U>) : _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<plus_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t + FWD(u)))
        { return _t + FWD(u); }

        template <typename U> requires std::regular_invocable<plus_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) + _t))
        { return FWD(u) + _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) + FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) + FWD(u)))
        -> decltype(auto)
    { return FWD(t) + FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p)))))
        -> decltype(auto)
    { return operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p))); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<T, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<T, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

constexpr inline plus_fn plus;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................MINUS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct minus_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) noexcept(std::is_nothrow_constructible_v<T, U>) : _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<minus_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t - FWD(u)))
        { return _t - FWD(u); }

        template <typename U> requires std::regular_invocable<minus_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) - _t))
        { return FWD(u) - _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) - FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) - FWD(u)))
        -> decltype(auto)
    { return FWD(t) - FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p)))))
        -> decltype(auto)
    { return operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p))); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<T, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<T, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

constexpr inline minus_fn minus;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................MULTIPLIES................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct multiplies_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u)  noexcept(std::is_nothrow_constructible_v<T, U>): _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<multiplies_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t * FWD(u)))
        { return _t * FWD(u); }

        template <typename U> requires std::regular_invocable<multiplies_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) * _t))
        { return FWD(u) * _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) * FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) * FWD(u)))
        -> decltype(auto)
    { return FWD(t) * FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p)))))
        -> decltype(auto)
    { return operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p))); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<T, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<T, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

constexpr inline multiplies_fn multiplies;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................DIVIDES................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct divides_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) noexcept(std::is_nothrow_constructible_v<T, U>) : _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<divides_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t / FWD(u)))
        { return _t / FWD(u); }

        template <typename U> requires std::regular_invocable<divides_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) / _t))
        { return FWD(u) / _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) / FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST
        noexcept(noexcept(FWD(t) / FWD(u)))
        -> decltype(auto)
    { return FWD(t) / FWD(u); }

    template <typename Tuple>
        requires (detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>)
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST
        noexcept(noexcept(operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p)))))
        -> decltype(auto)
    { return operator()(std::get<0>(FWD(p)), std::get<1>(FWD(p))); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<T, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<T, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

constexpr inline divides_fn divides;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................NEGATE................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct negate_fn
{
    template <typename T>
        requires requires(T && t) { -FWD(t); }
    constexpr CB_STATIC auto operator()(T && t) CB_CONST noexcept(noexcept( -FWD(t) ))
    { return -FWD(t); }
};

constexpr inline negate_fn negate;

#undef FWD
} // namespace brun

#endif /* BRUN_CALLABLES_ARITHMETIC_HPP */
