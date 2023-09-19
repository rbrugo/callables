/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : partial
 * @created     : Tuesday Sep 19, 2023 16:01:09 CEST
 * @description : 
 * */

#ifndef PARTIAL_HPP
#define PARTIAL_HPP

#include <concepts>
#include <utility>
#include "config.hpp"

namespace brun
{

#define FWD(x) static_cast<decltype(x) &&>(x)

namespace detail
{
template <typename T>
constexpr inline auto tuple_size = std::tuple_size_v<std::remove_cvref_t<T>>;
}  // namespace detail

template <typename Fn, typename T>
struct partial
{
private:
    T _t;

public:
    template <typename U> requires std::constructible_from<T, U>
    constexpr explicit partial(U && u) : _t{FWD(u)} {}

    template <typename ...U> requires std::regular_invocable<Fn, T, U...>
    constexpr decltype(auto) operator()(U &&... u) const noexcept(noexcept(Fn{}(_t, FWD(u)...)))
    { return Fn{}(_t, FWD(u)...); }
};

template <typename Fn, typename T>
struct right_partial
{
private:
    T _t;

public:
    template <typename U> requires std::constructible_from<T, U>
    constexpr explicit right_partial(U && u) : _t{FWD(u)} {}
    template <typename ...U> requires std::regular_invocable<Fn, U..., T>
    constexpr decltype(auto) operator()(U &&... u) const noexcept(noexcept(Fn{}(FWD(u)..., _t)))
    { return Fn{}(FWD(u)..., _t); }
};

template <typename Base>
struct binary_fn
{
    using base = Base;
    using is_transparent = void;

    template <typename T>
    constexpr
    static auto left(T && t) noexcept(noexcept(partial<base, std::unwrap_ref_decay_t<T>>{FWD(t)}))
    { return partial<base, std::unwrap_ref_decay_t<T>>{FWD(t)}; }

    template <typename T>
    constexpr
    static auto right(T && t) noexcept(noexcept(right_partial<base, std::unwrap_ref_decay_t<T>>{FWD(t)}))
    { return right_partial<base, std::unwrap_ref_decay_t<T>>{FWD(t)}; }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept(noexcept(left(FWD(t))))
    { return left(FWD(t)); }
};

template <typename Base>
struct compare_tuple_fn
{
    using base = Base;
    template <typename Tuple>
        requires (detail::tuple_size<Tuple> > 1)
    constexpr static
    auto tuple(Tuple && p) -> decltype(auto)
    {
        return [p=FWD(p)]<std::size_t ...I>(std::index_sequence<I...>) {
            return (base{}(get<I>(FWD(p)), get<I + 1>(FWD(p))) and ...);
        }(std::make_index_sequence<detail::tuple_size<Tuple> - 1>{});
    }
};

template <typename Base>
struct compare_operator : public binary_fn<Base>, compare_tuple_fn<Base>
{
    using binary_fn<Base>::operator();
};

template <typename Base>
struct recursive_tuple_fn
{
    using base = Base;

    template <std::size_t I, typename Tuple>
    constexpr static
    auto _impl(Tuple && p) {
        if constexpr (I + 1 < detail::tuple_size<Tuple>) {
            return base{}(get<I>(FWD(p)), _impl<I + 1>(FWD(p)));
        } else {
            return get<I>(FWD(p));
        }
    }

    template <typename Tuple>
        requires (detail::tuple_size<Tuple> > 0)
    constexpr static
    auto tuple(Tuple && p) -> decltype(auto)
    {
        return _impl<0>(FWD(p));
    }
};

template <typename Base>
struct arithmetic_operator : public binary_fn<Base>, recursive_tuple_fn<Base>
{
    using binary_fn<Base>::operator();
};

#undef FWD
} // namespace brun

#endif /* PARTIAL_HPP */
