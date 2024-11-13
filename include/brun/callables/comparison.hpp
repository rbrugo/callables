/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @created     : Wednesday Dec 15, 2021 17:54:27 CET
 * @description : implementations of partially-applicable comparison functions
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

#ifndef CB_COMPARISON_HPP
#define CB_COMPARISON_HPP

#include <utility>
#include "detail/functional.hpp"
#include "detail/partial.hpp"

namespace callables
{

// equal_to
// not_equal_to

#define CB_FWD(x) std::forward<decltype(x)>(x)

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................EQUAL_TO.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct equal_to_fn : public compare_operator<equal_to_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) == CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) == CB_FWD(u)))
        -> decltype(auto)
    {
        if constexpr (detail::numeric<std::remove_cvref_t<T>> and detail::numeric<std::remove_cvref_t<U>>) {
            return std::cmp_equal(t, u);
        } else {
            return CB_FWD(t) == CB_FWD(u);
        }
    }

    using compare_operator<equal_to_fn>::operator();
};

constexpr inline equal_to_fn equal_to;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................NOT_EQUAL_TO.............................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct not_equal_to_fn : public compare_operator<not_equal_to_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { CB_FWD(t) != CB_FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(CB_FWD(t) != CB_FWD(u)))
        -> decltype(auto)
    {
        if constexpr (detail::numeric<std::remove_cvref_t<T>> and detail::numeric<std::remove_cvref_t<U>>) {
            return std::cmp_not_equal(t, u);
        } else {
            return CB_FWD(t) != CB_FWD(u);
        }
    }

    using compare_operator<not_equal_to_fn>::operator();
};

constexpr inline not_equal_to_fn not_equal_to;

#undef CB_FWD
} // namespace callables

#endif /* CB_COMPARISON_HPP */
