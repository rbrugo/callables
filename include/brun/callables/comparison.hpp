/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : comparison
 * @created     : Wednesday Dec 15, 2021 17:54:27 CET
 * @description :
 * */

#ifndef BRUN_CALLABLES_COMPARISON_HPP
#define BRUN_CALLABLES_COMPARISON_HPP

#include <utility>
#include "detail/partial.hpp"

namespace brun
{

// equal_to
// not_equal_to

#define FWD(x) std::forward<decltype(x)>(x)

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................EQUAL_TO.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct equal_to_fn : public compare_operator<equal_to_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) == FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) == FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_equal(t, u);
        } else {
            return FWD(t) == FWD(u);
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
        requires requires(T && t, U && u) { FWD(t) != FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) != FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_not_equal(t, u);
        } else {
            return FWD(t) != FWD(u);
        }
    }

    using compare_operator<not_equal_to_fn>::operator();
};

constexpr inline not_equal_to_fn not_equal_to;

#undef FWD
} // namespace brun

#endif /* BRUN_CALLABLES_COMPARISON_HPP */
