/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Wednesday Dec 10, 2025 16:32:24 CET
 * @description : 
 * */

#ifndef CB_COMBINATORS_HPP
#define CB_COMBINATORS_HPP

#include <cstdint>
#include <tuple>
#include "detail/partial.hpp"

#include "detail/_config_begin.hpp"
namespace callables
{

// compose
// on
// flip

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ..................................COMPOSE................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct composed_tag {};

template <typename ...Fns>
class composed : public composed_tag {
    static constexpr auto size = sizeof...(Fns);
    std::tuple<Fns...> _functions;

    template <int64_t Idx, typename Tuple, typename ...Args>
    static constexpr auto step(Tuple && functions, Args &&... args) -> decltype(auto) {
        if constexpr (Idx == size - 1) {
            return std::get<Idx>(CB_FWD(functions))(CB_FWD(args)...);
        } else {
            return std::get<Idx>(CB_FWD(functions))(step<Idx + 1>(CB_FWD(functions), CB_FWD(args)...));
        }
    }

public:
    friend class compose_fn;

    template <typename Self, typename ...Args>
    constexpr auto operator()(this Self && self, Args &&... args) {
        return self.template step<0>(std::forward_like<Self>(self._functions), CB_FWD(args)...);
    }

    constexpr composed(std::tuple<Fns...> fns) : _functions(std::move(fns)) {}
};


struct compose_fn
{
    template <typename T>
    static constexpr auto as_tuple(T && arg) { return std::tuple<std::decay_t<T>>{CB_FWD(arg)}; }
    template <typename Composed> requires std::derived_from<std::remove_cvref_t<Composed>, composed_tag>
    static constexpr auto as_tuple(Composed && arg) { return CB_FWD(arg)._functions; }

    template <typename ...Fns>
        requires (std::derived_from<std::remove_cvref_t<Fns>, composed_tag> or ...)
    constexpr static auto operator()(Fns &&... fns) {
        return composed{std::tuple_cat(as_tuple(CB_FWD(fns))...)};
    }

    template <typename ...Fns>
        requires (not std::derived_from<std::remove_cvref_t<Fns>, composed_tag> and ...)
    constexpr static auto operator()(Fns &&... fns) {
        return composed{std::make_tuple(CB_FWD(fns)...)};
    }
};

constexpr inline compose_fn compose;

namespace operators
{
template <typename LeftFn, typename RightFn>
auto operator*(LeftFn && lhs, RightFn && rhs)
{
    return compose(CB_FWD(lhs), CB_FWD(rhs));
}
}  // namespace operators;

#if defined CB_TESTING_COMPOSE
static_assert(compose([](auto x) { return x + 1; }, [](auto a, auto b) { return a + b; })(0, 2) == 3);
static_assert(compose([](auto c) { return c - 'z'; }, [](auto c) { return c + 'z'; })('c') == 'c');
// static_assert(compose(+[](int a, int b) { return a + b; }, +[](int x) { return x; })(0, 0) == 0);  // no longer works
// static_assert(literals::operator*([]()));
#endif


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .....................................ON..................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct on_fn
{
    template <typename UnaryFn, typename BinaryFn>
    struct inner : public binary_fn<inner<UnaryFn, BinaryFn>>, applicable_on_tuples<inner<UnaryFn, BinaryFn>>
    {
        [[no_unique_address]] UnaryFn _un;
        [[no_unique_address]] BinaryFn _bin;

        template <typename T, typename U>
            requires requires(T && t, U && u) {
                { _un(t) };
                { _un(u) };
                { _bin(_un(t), _un(u)) };
            }
        [[nodiscard]] constexpr auto operator()(T && t, U && u) const -> decltype(auto)
        {
            return _bin(_un(CB_FWD(t)), _un(CB_FWD(u)));
        }

        template <typename ...Args>
            requires (std::invocable<UnaryFn, Args> and ...)
            and requires (Args ...args) {
                { _bin(_un(args)...) };
            }
        [[nodiscard]] constexpr auto operator()(Args &&... args) const -> decltype(auto)
        {
            return _bin(_un(CB_FWD(args))...);
        }

        using inner::binary_fn::operator();
        // template <typename Arg>
        // constexpr auto operator()(Arg && arg) {
        //     return partial<on_fn::inner
        // }
    };

    template <typename UnaryFn>
    struct outer
    {
        [[no_unique_address]] UnaryFn _un;

        template <typename BinaryFn>
        [[nodiscard]] constexpr auto operator()(BinaryFn && binary) const & -> decltype(auto)
        {
            return inner<UnaryFn, std::remove_cvref_t<BinaryFn>>{{}, _un, CB_FWD(binary)};
        }

        template <typename BinaryFn>
        [[nodiscard]] constexpr auto operator()(BinaryFn && binary) && -> decltype(auto)
        {
            return inner<UnaryFn, std::remove_cvref_t<BinaryFn>>{
                {}, {}, std::move(_un), CB_FWD(binary)
            };
        }
    };

    template <typename UnaryFn>
    [[nodiscard]] constexpr static auto operator()(UnaryFn && fn) noexcept
    { return outer<std::remove_cvref_t<UnaryFn>>{CB_FWD(fn)}; }

    template <typename UnaryFn, typename BinaryFn>
    [[nodiscard]] constexpr static auto operator()(UnaryFn && unary, BinaryFn && binary) noexcept
    {
        return inner<std::remove_cvref_t<UnaryFn>, std::remove_cvref_t<BinaryFn>>{
            {}, {}, CB_FWD(unary), CB_FWD(binary)
        };
    }
};

constexpr inline on_fn on;

#if defined CB_TESTING_ON
namespace _test
{
constexpr inline auto mod = [](auto const & x) static { return x % 2; };
constexpr inline auto eq = [](auto const & a, auto const & b) static { return a == b; };
constexpr inline auto ne = [](auto const & a, auto const & b) static { return a != b; };
constexpr inline auto len = [](auto const & x) static { return size(x); };
constexpr inline auto gt = [](auto const & a, auto const & b) static { return a > b; };
constexpr inline auto lt = [](auto const & a, auto const & b) static { return a < b; };

using std::string_view_literals::operator""sv;

static_assert(on(mod, eq)(0, 2));
static_assert(on(mod, ne)(0, 3));
static_assert(on(mod, eq)(0)(2));
static_assert(on(len)(eq)("ciao"sv)("hola"sv));
static_assert(on(len)(eq)("ciao"sv)("hola"sv));
static_assert(on(len)(lt).left("ciao"sv)("hello"sv));
static_assert(on(len)(gt).right("ciao"sv)("hello"sv));
}  // namespace _test
#endif


// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................FLIP.................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct flip_fn
{
    template <typename Fn, typename ...Args>
        requires (sizeof...(Args) > 0)
    [[nodiscard]] constexpr
    CB_STATIC auto operator()(Fn && fn, Args &&... args) CB_CONST -> decltype(auto)
    {
        return [fn=CB_FWD(fn),tp=std::forward_as_tuple(args...)]<std::size_t ...Idx>(std::index_sequence<Idx...>) mutable {
            return CB_FWD(fn)(std::get<sizeof...(Args) - Idx - 1>(tp)...);
        }(std::make_index_sequence<sizeof...(Args)>{});
    }

    template <typename Fn>
    struct capture
    {
        [[no_unique_address]] Fn _fn;

        template <typename ...Args>
        constexpr auto operator()(Args &&... args) const {
            return flip_fn{}(_fn, CB_FWD(args)...);
        }
    };

    template <typename Fn>
        requires std::is_empty_v<Fn>
    struct capture<Fn>
    {
        template <typename ...Args>
        constexpr auto operator()(Args &&... args) const {
            return flip_fn{}(Fn{}, CB_FWD(args)...);
        }
    };

    template <typename Fn>
    [[nodiscard]] constexpr
    CB_STATIC auto operator()(Fn && fn) CB_CONST
    {
        if constexpr (std::is_empty_v<Fn>) {
            return capture<Fn>{};
        } else {
            return capture<Fn>{CB_FWD(fn)};
        }
    }
};

constexpr inline flip_fn flip;

static_assert(flip([](auto a, auto b) { return b; })(0, 1) == 0);
static_assert(flip([](auto a, auto b, auto c) { return c; })(0, 1, 2) == 0);

}  // namespace callables

#include "detail/_config_end.hpp"  // IWYU pragma: export
#endif /* CB_COMBINATORS_HPP */
