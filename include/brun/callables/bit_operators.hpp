/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @created     : Friday Dec 23, 2022 21:12:51 CET
 * @description : implementations of partially-applicable bit manipulation functions
 * @license     :
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * */

#ifndef CB_BIT_OPERATORS_HPP
#define CB_BIT_OPERATORS_HPP

#include "detail/partial.hpp"

namespace callables
{

// bit_and
// bit_or
// bit_xor

#define CB_FWD(x) std::forward<decltype(x)>(x)
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_and................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_and_fn : public arithmetic_operator<bit_and_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) & CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) & CB_FWD(u)))
        -> decltype(auto)
    { return CB_FWD(t) & CB_FWD(u); }

    using binary_fn<bit_and_fn>::operator();
};

constexpr inline bit_and_fn bit_and;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_or.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_or_fn : public arithmetic_operator<bit_or_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) | CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) | CB_FWD(u)))
        -> decltype(auto)
    { return CB_FWD(t) | CB_FWD(u); }

    using binary_fn<bit_or_fn>::operator();
};

constexpr inline bit_or_fn bit_or;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_xor................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_xor_fn : public arithmetic_operator<bit_xor_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) ^ CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) ^ CB_FWD(u)))
        -> decltype(auto)
    { return CB_FWD(t) ^ CB_FWD(u); }

    using binary_fn<bit_xor_fn>::operator();
};

constexpr inline bit_xor_fn bit_xor;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_not.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_not_fn
{
    template <typename T>
        requires requires(T && t) { -CB_FWD(t); }
    constexpr CB_STATIC auto operator()(T && t) CB_CONST noexcept(noexcept( ~CB_FWD(t) ))
    { return ~CB_FWD(t); }
};

constexpr inline bit_not_fn bit_not;

#undef CB_FWD
} // namespace callables

#endif /* CB_BIT_OPERATORS_HPP */
