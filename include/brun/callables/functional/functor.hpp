/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Saturday Dec 06, 2025 00:21:25 CET
 * @description : 
 * */

#ifndef CB_FUNCTIONAL_FUNCTOR_HPP
#define CB_FUNCTIONAL_FUNCTOR_HPP

#include <optional>
#include <expected>
#include <vector>

#include "../identity.hpp"
#include "../detail/_config_begin.hpp"

/*
 * Implements the `functor` concept and the `fmap` CPO.
 *
 * A `functor` is an object that can be mapped over using a
 *  function, without changing its structure. We call this
 *  function `fmap` to provide a unified structure to it.
 * In Haskell notation, given a functor `f` and two types
 *  `a` and `b`, `fmap` type signature is:
 *  `fmap :: (a -> b) -> f a -> f b`
 * */

namespace callables
{

namespace _functor {
// Overloads for `std::vector`
template <typename Fn, typename T, typename Alloc>
constexpr auto fmap(Fn && fn, std::vector<T, Alloc> const & from)
{
    using to_t = std::invoke_result_t<Fn &, T &>;
    static_assert(not std::is_reference_v<to_t>, "can't create a vector of references");
    using allocator_t = typename std::allocator_traits<Alloc>::template rebind_alloc<to_t>;
    auto result = std::vector<to_t, allocator_t>();
    result.reserve(from.size());
    for (auto const & elem : from) {
        result.emplace_back(fn(elem));
    }
    return result;
}

template <typename Fn, typename T, typename Alloc>
constexpr auto fmap(Fn && fn, std::vector<T, Alloc> & from)
{
    using to_t = std::invoke_result_t<Fn &, T &>;
    static_assert(not std::is_reference_v<to_t>, "can't create a vector of references");
    using allocator_t = typename std::allocator_traits<Alloc>::template rebind_alloc<to_t>;
    auto result = std::vector<to_t, allocator_t>();
    result.reserve(from.size());
    for (auto & elem : from) {
        result.emplace_back(fn(elem));
    }
    return result;
}

template <typename Fn, typename T, typename Alloc>
constexpr auto fmap(Fn && fn, std::vector<T, Alloc> && from)
{
    using to_t = std::invoke_result_t<Fn &, T &>;
    static_assert(not std::is_reference_v<to_t>, "can't create a vector of references");
    using allocator_t = typename std::allocator_traits<Alloc>::template rebind_alloc<to_t>;
    auto result = std::vector<to_t, allocator_t>();
    result.reserve(from.size());
    for (auto && elem : from) {
        result.emplace_back(fn(std::move(elem)));
    }
    return result;
}

// Overloads for std::array
template <typename From, typename Fn, std::size_t N>
    requires std::invocable<Fn, From const &>
constexpr auto fmap(Fn && fn, std::array<From, N> const & from)
    noexcept(noexcept(fn(std::get<0>(from))))
{
    return [&]<std::size_t ...Idx>(std::index_sequence<Idx...>) {
        return std::array{fn(std::get<Idx>(from))...};
    }(std::make_index_sequence<N>());
}

template <typename From, typename Fn, std::size_t N>
    requires std::invocable<Fn, From &&>
constexpr auto fmap(Fn && fn, std::array<From, N> && from)
    noexcept(noexcept(fn(std::move(std::get<0>(from)))))
{
    return [&]<std::size_t ...Idx>(std::index_sequence<Idx...>) {
        return std::array{fn(std::move(std::get<Idx>(from)))...};
    }(std::make_index_sequence<N>());
}

// Overloads for std::optional
template <typename Fn, typename From>
constexpr auto fmap(Fn && fn, std::optional<From> const & from)
    noexcept(noexcept(from.transform(CB_FWD(fn))))
{
    return from.transform(CB_FWD(fn));
}

template <typename Fn, typename From>
constexpr auto fmap(Fn && fn, std::optional<From> & from)
    noexcept(noexcept(from.transform(CB_FWD(fn))))
{
    return from.transform(CB_FWD(fn));
}

template <typename Fn, typename From>
constexpr auto fmap(Fn && fn, std::optional<From> && from)
    noexcept(noexcept(std::move(from).transform(CB_FWD(fn))))
{
    return std::move(from).transform(CB_FWD(fn));
}

// Overloads for std::expected
template <typename Fn, typename From, typename Err>
constexpr auto fmap(Fn && fn, std::expected<From, Err> const & from)
    noexcept(noexcept(from.transform(CB_FWD(fn))))
{
    return from.transform(CB_FWD(fn));
}

template <typename Fn, typename From, typename Err>
constexpr auto fmap(Fn && fn, std::expected<From, Err> & from)
    noexcept(noexcept(from.transform(CB_FWD(fn))))
{
    return from.transform(CB_FWD(fn));
}

template <typename Fn, typename From, typename Err>
constexpr auto fmap(Fn && fn, std::expected<From, Err> && from)
    noexcept(noexcept(std::move(from).transform(CB_FWD(fn))))
{
    return std::move(from).transform(CB_FWD(fn));
}

// Concepts
template <typename Functor, typename Fn>
concept fmappable_free_function_with = requires(Fn const & fn, Functor f) {
    { fmap(fn, f) };
};

template <typename Functor, typename Fn>
concept fmappable_member_function_with = requires(Fn const & fn, Functor f) {
    { f.fmap(fn) };
};

template <typename Functor, typename Fn>
concept fmappable_with = fmappable_free_function_with<Functor, Fn> or fmappable_member_function_with<Functor, Fn>;

struct fmap_fn
{
    template <typename Fn, typename From>
    static constexpr bool _noexcept_invocable()
    {
        if constexpr (fmappable_member_function_with<From, Fn>) {
            return noexcept(std::declval<From>().fmap(std::declval<Fn>()));
        } else {
            return noexcept(fmap(std::declval<Fn>(), std::declval<From>()));
        }
    }

    template <typename Fn, typename From>
        requires fmappable_with<From, Fn>
    static constexpr auto operator()(Fn && fn, From && from) noexcept(_noexcept_invocable<Fn, From>()) -> decltype(auto)
    {
        if constexpr (fmappable_member_function_with<From, Fn>) {
            return CB_FWD(from).fmap(CB_FWD(fn));
        } else {
            return fmap(CB_FWD(fn), CB_FWD(from));
        }
    }
};

}  // namespace functor

constexpr inline _functor::fmap_fn fmap{};

template <typename Functor, typename Fn>
concept fmappable_with = _functor::fmappable_with<Functor, Fn>;

template <typename T>
concept functor = requires(T f) {
    { fmap(decay_copy, f) } -> std::same_as<std::decay_t<T>>;
};

}  // namespace callables

#include "../detail/_config_end.hpp"  // IWYU pragma: export
#endif /* CB_FUNCTIONAL_FUNCTOR_HPP */
