/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : operators
 * @created     : Monday Dec 13, 2021 13:24:01 CET
 * @description :
 * */

#ifndef BRUN_CALLABLES_ARITHMETIC_HPP
#define BRUN_CALLABLES_ARITHMETIC_HPP

#include "detail/partial.hpp"

namespace brun
{

// plus
// minus
// multiplies
// divides
// negate

#define FWD(x) static_cast<decltype(x) &&>(x)
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................PLUS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct plus_fn : public arithmetic_operator<plus_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) + FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) + FWD(u)))
        -> decltype(auto)
    { return FWD(t) + FWD(u); }

    using binary_fn<plus_fn>::operator();
};

constexpr inline plus_fn plus;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................MINUS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct minus_fn : public arithmetic_operator<minus_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) - FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) - FWD(u)))
        -> decltype(auto)
    { return FWD(t) - FWD(u); }

    using binary_fn<minus_fn>::operator();
};

constexpr inline minus_fn minus;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................MULTIPLIES................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct multiplies_fn : public arithmetic_operator<multiplies_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) * FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) * FWD(u)))
        -> decltype(auto)
    { return FWD(t) * FWD(u); }

    using binary_fn<multiplies_fn>::operator();
};

constexpr inline multiplies_fn multiplies;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................DIVIDES................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct divides_fn : public arithmetic_operator<divides_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) / FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) / FWD(u)))
        -> decltype(auto)
    { return FWD(t) / FWD(u); }

    using binary_fn<divides_fn>::operator();
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
