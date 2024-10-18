/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : ordering
 * @created     : Monday Dec 13, 2021 13:19:39 CET
 * @description : implementations of partially-applicable arithmetic functions
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

#ifndef CB_ORDERING_HPP
#define CB_ORDERING_HPP

#include <utility>
#include "comparison.hpp"      // IWYU pragma: export
#include "detail/partial.hpp"

namespace callables
{

// less / less_equal
// greater / greater_equal

#define CB_FWD(x) std::forward<decltype(x)>(x)

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................LESS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct less_fn : public compare_operator<less_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) < CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) < CB_FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_less(t, u);
        } else {
            return CB_FWD(t) < CB_FWD(u);
        }
    }

    using compare_operator<less_fn>::operator();
};

constexpr inline less_fn less;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................LESS_EQUAL................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct less_equal_fn : public compare_operator<less_equal_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) <= CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) <= CB_FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_less_equal(t, u);
        } else {
            return CB_FWD(t) <= CB_FWD(u);
        }
    }

    using compare_operator<less_equal_fn>::operator();
};

constexpr inline less_equal_fn less_equal;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................GREATER................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct greater_fn : public compare_operator<greater_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) <= CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) > CB_FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_greater(t, u);
        } else {
            return CB_FWD(t) > CB_FWD(u);
        }
    }

    using compare_operator<greater_fn>::operator();
};

constexpr inline greater_fn greater;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...............................GREATER_EQUAL................................ //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct greater_equal_fn : public compare_operator<greater_equal_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) <= CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) >= CB_FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_greater_equal(t, u);
        } else {
            return CB_FWD(t) >= CB_FWD(u);
        }
    }

    using compare_operator<greater_equal_fn>::operator();
};

constexpr inline greater_equal_fn greater_equal;

#undef CB_FWD
} // namespace callables

#endif /* CB_ORDERING_HPP */
