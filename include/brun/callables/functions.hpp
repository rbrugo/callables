/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : functions
 * @created     : Sunday Dec 13, 2020 21:54:35 CET
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

#ifndef CB_FUNCTIONS_HPP
#define CB_FUNCTIONS_HPP

#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
#define HAS_MD_SUBSCRIPT 1
#else
#define HAS_MD_SUBSCRIPT 0
#endif

#include <span>
#include <utility>
#include <optional>
#include <functional>
#include "detail/partial.hpp"
#include "detail/functional.hpp"

namespace callables
{
#define CB_FWD(x) std::forward<decltype(x)>(x)

// apply
// compose
// identity
// construct
// get
// at
// value_or
// from_container
// addressof
// dereference

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................APPLY.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct apply_fn : public binary_fn<apply_fn>
{
    template <typename Fn, typename Tuple>
        requires detail::direct_applicable<Fn, Tuple>
    constexpr CB_STATIC
    auto operator()(Fn && fn, Tuple && args) CB_CONST
        noexcept(noexcept(apply(CB_FWD(fn), CB_FWD(args))))
        -> decltype(auto)
    { return apply(CB_FWD(fn), CB_FWD(args)); }

    template <typename Fn, typename T>
        requires detail::has_member_apply_with<T, Fn>
    constexpr CB_STATIC
    auto operator()(Fn && fn, T && obj) CB_CONST
        noexcept(noexcept(CB_FWD(obj).apply(CB_FWD(fn))))
        -> decltype(auto)
    { return CB_FWD(obj).apply(CB_FWD(fn)); }

    using binary_fn<apply_fn>::operator();
};

constexpr inline apply_fn apply;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................COMPOSE................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct compose_fn
{
    template <typename ...Fns>
    struct compose_fn_capture
    {
    private:
        std::tuple<Fns...> _fns;

        struct uncallable {};

        template <typename ...Args>
        constexpr
        static auto call_with_args(std::tuple<Fns...> & tp, Args &&... args) -> decltype(auto)
        { return _call_impl<0>(tp, std::forward<Args>(args)...); }

        template <typename ...Args>
        constexpr
        static auto call_with_args(std::tuple<Fns...> const & tp, Args &&... args) -> decltype(auto)
        { return _call_impl<0>(tp, std::forward<Args>(args)...); }

        template <std::size_t I, typename Tuple, typename ...Args>
        constexpr
        static auto _call_impl(Tuple && fns, Args &&... args) -> decltype(auto)
        {
#           define GET(idx, tpl) std::get<idx>(std::forward<Tuple>(tpl))
            if constexpr (I == sizeof...(Fns) - 1) {
                if constexpr (std::invocable<std::tuple_element_t<I, std::remove_cvref_t<Tuple>>, Args...>) {
                    return GET(I, fns)(CB_FWD(args)...);
                } else if constexpr (sizeof...(Fns) != 1) {
                    return uncallable{};
                }
            } else {
                using nested_call_result = decltype(_call_impl<I + 1>(CB_FWD(fns), CB_FWD(args)...));
                if constexpr (not std::same_as<nested_call_result, uncallable>) {
                    return GET(I, fns)(_call_impl<I + 1>(CB_FWD(fns), CB_FWD(args)...));
                } else {
                    using fn_t = std::tuple_element_t<I, std::remove_cvref_t<Tuple>>;
                    if constexpr ((std::invocable<fn_t, decltype(_call_impl<I + 1>(CB_FWD(fns), CB_FWD(args)))...>)) {
                        return GET(I, fns)(_call_impl<I + 1>(CB_FWD(fns),CB_FWD(args))...);
                    } else if constexpr (I != 0) {
                        return uncallable{};
                    }
                }
            }
#           undef GET
        }
    public:
        template <typename ...Fns2>
            requires detail::pairwise_constructible<Fns...>::template from<Fns2...>
        constexpr
        compose_fn_capture(Fns2 &&... fns) : _fns{std::forward<Fns2>(fns)...} {}

        template <typename ...Args>
        constexpr auto operator()(Args &&... args) const -> decltype(auto)
        { return call_with_args(_fns, std::forward<Args>(args)...); }

        template <typename ...Args>
        constexpr auto operator()(Args &&... args) -> decltype(auto)
        { return call_with_args(_fns, std::forward<Args>(args)...); }
    };

    template <typename ...Fns>
    constexpr CB_STATIC
    auto operator()(Fns &&... fns) CB_CONST noexcept
    {
        return compose_fn_capture<Fns...>{std::forward<Fns>(fns)...};
    }
};

constexpr inline compose_fn compose;

#if 0
static_assert(compose([](auto x) { return x + 1; }, [](auto a, auto b) { return a + b; })(0, 2) == 3);
static_assert(compose([](auto c) { return c - 'z'; }, [](auto c) { return c + 'z'; })('c') == 'c');
static_assert(compose(+[](int a, int b) { return a + b; }, +[](int x) { return x; })(0, 0) == 0);
#endif

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................IDENTITY.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct identity_fn
{
    using is_transparent = void;

    template <typename T>
    constexpr CB_STATIC auto operator()(T && t) CB_CONST noexcept
        -> decltype(auto)
    { return std::forward<T &&>(t); }
};

constexpr inline identity_fn identity;


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................ADDRESSOF.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct addressof_fn {
    constexpr CB_STATIC
    auto operator()(auto & obj) CB_CONST noexcept -> auto * { return std::addressof(obj); }
    constexpr CB_STATIC
    auto operator()(auto const & obj) CB_CONST noexcept -> auto const * { return std::addressof(obj); }
};

constexpr inline addressof_fn addressof;


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ................................DEREFERENCE................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct dereference_fn {
    template <typename T>
        requires requires(T && t) { *std::forward<T>(t); }
    constexpr CB_STATIC
    auto operator()(T && obj) CB_CONST noexcept -> decltype(auto)
    { return *std::forward<T>(obj); }
};

constexpr inline dereference_fn dereference;


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................CONSTRUCT.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
template <typename T>
struct construct_fn
{
private:
    struct from_tuple_fn
    {
        template <typename ...Args>
            requires std::constructible_from<T, Args...>
        constexpr CB_STATIC
        auto operator()(std::tuple<Args...> && args) CB_CONST
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            -> decltype(auto)
        {
            return [args=std::move(args)]<std::size_t ...I>(std::index_sequence<I...>) {
                return T(std::get<I>(args)...);
            }(std::make_index_sequence<sizeof...(Args)>{});
        }

        template <typename ...Args>
            requires std::constructible_from<T, Args...>
        constexpr CB_STATIC
        auto operator()(std::tuple<Args...> const & args) CB_CONST
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            -> decltype(auto)
        {
            return [args=std::move(args)]<std::size_t ...I>(std::index_sequence<I...>) {
                return T(std::get<I>(args)...);
            }(std::make_index_sequence<sizeof...(Args)>{});
        }
    };

public:
    [[no_unique_address]] from_tuple_fn from_tuple;

    template <typename ...Args>
        requires std::constructible_from<T, Args...>
    constexpr CB_STATIC
    auto operator()(Args &&... args) CB_CONST
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
        -> decltype(auto)
    { return T(std::forward<Args>(args)...); }
};

template <typename T>
constexpr inline construct_fn<T> construct;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................GET..................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
template <std::size_t N>
struct get_fn
{
    template <typename Obj>
    constexpr CB_STATIC
    auto operator()(Obj && obj) CB_CONST noexcept(noexcept(get<N>(std::forward<Obj>(obj))))
        -> decltype(auto)
    { return get<N>(std::forward<Obj>(obj)); }
};

template <std::size_t N>
constexpr inline get_fn<N> get;


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .....................................AT..................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct at_fn
{
    template <typename T, typename ...>
    struct first_ { using type = T; };

    template <typename ...N>
    struct use_member
    {
        using storage_t = std::conditional_t<sizeof...(N) != 1, std::tuple<N...>, typename first_<N...>::type>;
        storage_t indices;

        template <typename Obj>
        constexpr auto operator()(Obj && obj) -> decltype(auto)
        {
            if constexpr (sizeof...(N) == 1) {
                return at_fn{}(CB_FWD(obj), indices);
            } else {
                return std::apply([obj=CB_FWD(obj)](N &&... args) {
                    return at_fn{}(CB_FWD(obj), CB_FWD(args)...);
                }, indices);
            }
        }
    };

#if HAS_MD_SUBSCRIPT
    template <typename ...N>
    struct use_op
    {
        using storage_t = std::conditional_t<sizeof...(N) != 1, std::tuple<N...>, typename first_<N...>::type>;
        storage_t indices;

        template <typename Obj>
        constexpr auto operator()(Obj && obj) -> decltype(auto)
        {
            if constexpr (sizeof...(N) == 1) {
                return at_fn{}[CB_FWD(obj), indices];
            } else {
                return std::apply([obj=CB_FWD(obj)](N &&... args) {
                    return at_fn{}[CB_FWD(obj), CB_FWD(args)...];
                }, indices);
            }
        }
    };
#else
    template <typename N>
    struct use_op
    {
        N index;

        template <typename Obj>
        constexpr auto operator()(Obj && obj) -> decltype(auto)
        {
            return CB_FWD(obj)[index];
        }
    };
#endif


    template <typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator()(N &&... n) CB_CONST noexcept -> decltype(auto)
    { return use_member<std::unwrap_ref_decay_t<N>...>{CB_FWD(n)...}; }

    template <typename Obj, typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator()(Obj && obj, N &&... n) CB_CONST noexcept(noexcept(CB_FWD(obj).at(CB_FWD(n)...)))
        -> decltype(auto)
    { return CB_FWD(obj).at(CB_FWD(n)...); }

#if HAS_MD_SUBSCRIPT
    template <typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator[](N &&... n) CB_CONST noexcept -> decltype(auto)
    { return use_op<std::unwrap_ref_decay_t<N>...>{CB_FWD(n)...}; }

    template <typename Obj, typename ...N>
        requires (sizeof...(N) > 0)
    constexpr CB_STATIC
    auto operator[](Obj && obj, N &&... n) CB_CONST noexcept(noexcept(CB_FWD(obj)[CB_FWD(n)...]))
        -> decltype(auto)
    { return CB_FWD(obj)[CB_FWD(n)...]; }
#else
    template <typename N>
    constexpr CB_STATIC
    auto operator[](N && n) CB_CONST noexcept -> decltype(auto)
    { return use_op<std::unwrap_ref_decay_t<N>>{CB_FWD(n)}; }

    template <typename Obj, typename N>
    constexpr CB_STATIC
    auto operator[](Obj && obj, N && n) CB_CONST noexcept(noexcept(CB_FWD(obj)[CB_FWD(n)]))
        -> decltype(auto)
    { return CB_FWD(obj)[CB_FWD(n)]; }
#endif
};

constexpr inline at_fn at;


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...............................FROM_CONTAINER............................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct from_container_fn
{
    template <std::ranges::range Cont, typename Idx>
    static constexpr
    auto use_iterators(Cont && cont, Idx && idx) -> decltype(auto)
    {
        return *std::ranges::next(
            std::ranges::begin(cont),
            CB_FWD(idx),
            std::ranges::end(cont)
        );
    }

    template <typename Cont, typename Idx>
    requires requires(Cont && c, Idx && i) { CB_FWD(c).at(CB_FWD(i)); }
    static constexpr
    auto use_at(Cont && c, Idx && i) -> decltype(auto) {
        return CB_FWD(c).at(CB_FWD(i));
    }

    template <typename Cont, typename Idx>
    requires requires(Cont && c, Idx && i) { CB_FWD(c)[CB_FWD(i)]; }
    static constexpr
    auto use_op(Cont && c, Idx && i) -> decltype(auto) {
        return CB_FWD(c)[CB_FWD(i)];
    }

    template <typename Cont, typename Idx>
    constexpr CB_STATIC
    auto try_all(Cont && c, Idx && i) CB_CONST -> decltype(auto)
    {
        if constexpr (requires { CB_FWD(c).at(CB_FWD(i)); }) {
            return use_at(CB_FWD(c), CB_FWD(i));
        } else if constexpr (requires { CB_FWD(c)[CB_FWD(i)]; }) {
            return use_op(CB_FWD(c), CB_FWD(i));
        } else if (std::ranges::range<Cont>) {
            return use_iterators(CB_FWD(c), CB_FWD(i));
        } else {
            static_assert(false, "the container must be indexable via op[], .at or iterators");
        }
    }

    template <typename Cont, typename Idx>
    constexpr CB_STATIC
    auto try_unsafe(Cont && c, Idx && i) CB_CONST -> decltype(auto)
    {
        if constexpr (requires { CB_FWD(c)[CB_FWD(i)]; }) {
            return use_op(CB_FWD(c), CB_FWD(i));
        } else if (std::ranges::range<Cont>) {
            return use_iterators(CB_FWD(c), CB_FWD(i));
        } else {
            static_assert(false, "the container must be indexable via op[] or iterators");
        }
    }

    template <typename Cont, typename Idx>
    constexpr CB_STATIC
    auto operator()(Cont && c, Idx && i) CB_CONST -> decltype(auto)
    {
        return try_all(CB_FWD(c), CB_FWD(i));
    }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept
    {
        return partial<from_container_fn, std::unwrap_ref_decay_t<T>>{CB_FWD(t)};
    }

    template <typename T, std::size_t N>
    constexpr CB_STATIC
    auto operator()(T (&t)[N]) CB_CONST noexcept
    {
        return partial<from_container_fn, std::span<T>>{std::span<T>{t, N}};
    }
};

constexpr inline from_container_fn from_container;




// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................VALUE_OR.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct value_or_fn
{
    template <class Opt, typename T>
    requires requires(Opt opt, T t) {
        { static_cast<bool>(opt) };
        { *opt } -> std::convertible_to<T>;
    }
    constexpr CB_STATIC
    auto operator()(Opt && opt, T && t) CB_CONST noexcept
        -> std::remove_cvref_t<decltype(*opt)>
    {
        if (static_cast<bool>(opt)) {
            return *CB_FWD(opt);
        }
        return CB_FWD(t);
    }

    template <typename Null, typename T>
    requires std::same_as<Null, std::nullptr_t> or std::same_as<Null, std::nullopt_t>
    constexpr CB_STATIC
    auto operator()(Null &&, T && t) CB_CONST noexcept -> decltype(auto) { return CB_FWD(t); }

    template <typename T>
    constexpr CB_STATIC
    auto operator()(T && t) CB_CONST noexcept
    {
        return right_partial<value_or_fn, std::unwrap_ref_decay_t<T>>{CB_FWD(t)};
    }
};

constexpr inline value_or_fn value_or;

static_assert(value_or(nullptr, 10) == 10);

#undef CB_FWD
} // namespace callables

#endif /* CB_FUNCTIONS_HPP */
