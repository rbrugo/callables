/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : bit_operators
 * @created     : Friday Dec 23, 2022 21:12:51 CET
 * @description : 
 * */

#ifndef BRUN_CALLABLES_BIT_OPERATORS_HPP
#define BRUN_CALLABLES_BIT_OPERATORS_HPP

#include "detail/partial.hpp"

namespace brun
{

// bit_and
// bit_or
// bit_xor

#define FWD(x) std::forward<decltype(x)>(x)
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_and................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_and_fn : public arithmetic_operator<bit_and_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) & FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) & FWD(u)))
        -> decltype(auto)
    { return FWD(t) & FWD(u); }

    using binary_fn<bit_and_fn>::operator();
};

constexpr inline bit_and_fn bit_and;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_or.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_or_fn : public arithmetic_operator<bit_or_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) | FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) | FWD(u)))
        -> decltype(auto)
    { return FWD(t) | FWD(u); }

    using binary_fn<bit_or_fn>::operator();
};

constexpr inline bit_or_fn bit_or;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_xor................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_xor_fn : public arithmetic_operator<bit_xor_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) ^ FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) ^ FWD(u)))
        -> decltype(auto)
    { return FWD(t) ^ FWD(u); }

    using binary_fn<bit_xor_fn>::operator();
};

constexpr inline bit_xor_fn bit_xor;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_not.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_not_fn
{
    template <typename T>
        requires requires(T && t) { -FWD(t); }
    constexpr CB_STATIC auto operator()(T && t) CB_CONST noexcept(noexcept( ~FWD(t) ))
    { return ~FWD(t); }
};

constexpr inline bit_not_fn bit_not;

#undef FWD
} // namespace brun

#endif /* BRUN_CALLABLES_BIT_OPERATORS_HPP */
