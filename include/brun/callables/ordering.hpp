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
#include "detail/partial.hpp"

namespace brun
{

// less / less_equal
// greater / greater_equal

#define FWD(x) std::forward<decltype(x)>(x)

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................LESS.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct less_fn : public compare_operator<less_fn>
{
    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) < FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) < FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_less(t, u);
        } else {
            return FWD(t) < FWD(u);
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
        requires requires(T && t, U && u) { FWD(t) <= FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) <= FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_less_equal(t, u);
        } else {
            return FWD(t) <= FWD(u);
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
        requires requires(T && t, U && u) { FWD(t) <= FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) > FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_greater(t, u);
        } else {
            return FWD(t) > FWD(u);
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
        requires requires(T && t, U && u) { FWD(t) <= FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) >= FWD(u)))
        -> decltype(auto)
    {
        if constexpr (std::integral<std::remove_cvref_t<T>> and std::integral<std::remove_cvref_t<U>>) {
            return std::cmp_greater_equal(t, u);
        } else {
            return FWD(t) >= FWD(u);
        }
    }

    using compare_operator<greater_equal_fn>::operator();
};

constexpr inline greater_equal_fn greater_equal;

#undef FWD
} // namespace brun

#endif /* BRUN_CALLABLES_ORDERING_HPP */
