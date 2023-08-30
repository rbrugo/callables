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

#define FWD(x) std::forward<decltype(x)>(x)
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_and................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_and_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) : _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<bit_and_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t & FWD(u)))
        { return _t & FWD(u); }

        template <typename U> requires std::regular_invocable<bit_and_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) & _t))
        { return FWD(u) & _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) & FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST
        noexcept(noexcept(FWD(t) & FWD(u)))
        -> decltype(auto)
    { return FWD(t) & FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST
        noexcept(noexcept(std::get<0>(FWD(p)) & std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) & std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

constexpr inline bit_and_fn bit_and;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_or.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_or_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) : _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t | FWD(u)))
        { return _t | FWD(u); }

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) | _t))
        { return FWD(u) | _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) | FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST
        noexcept(noexcept(FWD(t) & FWD(u)))
        -> decltype(auto)
    { return FWD(t) | FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) | std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) | std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

constexpr inline bit_or_fn bit_or;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................bit_xor................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct bit_xor_fn
{
    template <typename T, bool Left>
    struct partial
    {
    private:
        T _t;

    public:
        template <typename U> requires std::constructible_from<T, U>
        constexpr explicit partial(U && u) : _t{FWD(u)} {}

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and Left
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(_t ^ FWD(u)))
        { return _t ^ FWD(u); }

        template <typename U> requires std::regular_invocable<bit_or_fn, T, U> and (not Left)
        constexpr decltype(auto) operator()(U && u) const noexcept(noexcept(FWD(u) ^ _t))
        { return FWD(u) ^ _t; }
    };

    template <typename T, typename U>
        requires requires(T && t, U && u) { FWD(t) ^ FWD(u); }
    constexpr CB_STATIC
    auto operator()(T && t, U && u) CB_CONST noexcept(noexcept(FWD(t) & FWD(u)))
        -> decltype(auto)
    { return FWD(t) ^ FWD(u); }

    template <typename Tuple>
        requires detail::is_tuple_specialization<Tuple> or detail::is_pair_specialization<Tuple>
             and (detail::tuple_size<Tuple> == 2)
    constexpr CB_STATIC
    auto operator()(Tuple && p) CB_CONST noexcept(noexcept(std::get<0>(FWD(p)) ^ std::get<1>(FWD(p))))
        -> decltype(auto)
    { return std::get<0>(FWD(p)) ^ std::get<1>(FWD(p)); }

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, true>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}))
    { return partial<std::unwrap_ref_decay_t<T>, false>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
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
