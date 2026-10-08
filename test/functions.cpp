/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : functions
 * @created     : Monday Aug 21, 2023 17:33:45 CEST
 * @description : 
 */

#include <brun/callables/functions.hpp>
#include <brun/callables/ordering.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"
#include <algorithm>
#include <array>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std::literals;

#define DECLARE(EXPR) std::tuple{EXPR, #EXPR}

namespace test {
struct external_apply { int x; };

template <typename Fn>
auto apply(Fn && fn, external_apply const & obj) noexcept -> decltype(auto)
{ return std::forward<Fn>(fn)(obj.x); }

struct member_apply
{
    int x;
    template <typename Fn>
    constexpr auto apply(Fn && fn) noexcept -> decltype(auto) { return std::forward<Fn>(fn)(x); }
};

struct external_get { std::array<int, 3> x; };
template <std::size_t N>
auto get(external_get obj) { return get<N>(obj.x); }

struct member_get
{
    std::array<int, 3> x;
    template <std::size_t N> auto get() { return std::get<N>(x); }
};

struct point { int x; double y; };
struct flags { unsigned on : 1; int count; };
constexpr auto x_of(point const & p) -> int { return p.x; }
constexpr auto lower(int a, int b) { return a < b; }
constexpr auto make_point(int x) -> point { return {x, 0.}; }

struct counter { int x; constexpr auto minus(int y) const -> int { return x - y; } };
struct counter_call { counter c; int y; };  // a user tuple-like, applied through `get`
template <std::size_t N>
constexpr auto get(counter_call const & cc) { if constexpr (N == 0) { return cc.c; } else { return cc.y; } }
}  // namespace test

template <> struct std::tuple_size<test::counter_call> : std::integral_constant<std::size_t, 2> {};



int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;

    "curry_fn"_test = [] {
        using callables::curry;
        auto [sum, sum_expr] = DECLARE(([](int a, int b) { return a + b;}));
        auto [prod, prod_expr] = DECLARE(([](auto ...n) { return (n * ...); }));
        should("bind_front if any argument is passed with the callable to lift") = [&] {
            expect(curry(sum, 1)(2) == 3_i) << sum_expr << "curried with arg 1, then 2";
            expect(curry(sum, 1, 2)() == 3_i) << sum_expr << "curried with arg 1 and 2, then ()";
        };
        should("make the function curriable for any number of args, one time per application") = [&] {
            auto curriable_sum = curry(sum);
            expect(curriable_sum(1)(2) == 3_i) << sum_expr << "with argument 1 and then 2";
            expect(curriable_sum(1, 2)() == 3_i) << sum_expr << "with arguments 1, 2 and then ()";

            auto curriable_prod = curry(prod);
            auto twice = curry(curriable_prod);
            expect(curriable_prod(2)(2) == 4_i) << prod_expr << "with argument 2 and then 2";
            expect(twice(2)(2)(3) == 12_i) << prod_expr << "curried trice with arg 2, then 2, then 3";
            auto beast = curry(curry(curriable_prod, 1), 2);
            expect(beast(3)(4) == 24_i) << prod_expr << "mixed compositions of curries";
        };
        should("support move-only bound arguments, even when re-currying an already curried callable") = [] {
            auto move_only_sum = [](std::unique_ptr<int> a, std::unique_ptr<int> b) { return *a + *b; };

            auto once = curry(move_only_sum, std::make_unique<int>(3));
            expect(std::move(once)(std::make_unique<int>(4)) == 7_i) << "curry with a move-only bound argument";

            auto again = curry(move_only_sum, std::make_unique<int>(3));
            auto twice = curry(std::move(again), std::make_unique<int>(10));
            expect(std::move(twice)() == 13_i)
                << "re-currying an already-curried callable with a move-only bound argument";
        };

        should("accept member pointers and plain functions") = [] {
            auto const c = test::counter{10};
            expect(curry(&test::counter::minus, c)(3) == 7_i);
            expect(curry(test::lower, 1)(2));
        };
    };

    "apply_fn"_test = [] {
        using callables::apply;
        auto [sum, sum_expr] = DECLARE(([](int a, int b) { return a + b;}));
        auto [prod, prod_expr] = DECLARE(([](auto ...n) { return (n * ...); }));

        should("apply its arguments to the stored function") = [&] {
            expect(apply(sum)(std::tuple{1, 2}) == 3_i) << sum_expr << "with arguments 1, 2";
            expect(apply(prod)(std::tuple{1, 2, 3}) == 6_i) << prod_expr << "with arguments 1, 2, 3";
            expect(apply(prod)(std::tuple{2, 2, 2, 2}) == 16_i) << prod_expr << "with arguments 2, 2, 2, 2";
        };
        should("apply choose an ADL or member overload of `apply` if available") = [&] {
            auto stuff_1 = test::external_apply{10};
            auto stuff_2 = test::member_apply{10};
            expect(apply([](auto x) { return x; })(stuff_1) == 10_i);
            expect(apply([](auto x) { return x; })(stuff_2) == 10_i);
        };

        should("accept member pointers") = [] {
            auto const c = test::counter{10};
            expect(apply(&test::counter::minus, std::tuple{c, 3}) == 7_i);
            expect(apply(&test::counter::minus, test::counter_call{c, 3}) == 7_i);
        };

        should("accept lvalue user tuple-likes") = [] {
            auto const cc = test::counter_call{test::counter{10}, 3};
            expect(apply(&test::counter::minus, cc) == 7_i);
        };

        should("forward the elements like `std::get`") = [] {
            constexpr auto types = []<typename ...A>(A &&...) { return std::type_identity<std::tuple<A &&...>>{}; };
            auto a = 0;
            auto b = 0;
            auto t = std::tuple<int, int &, int &&, int const>{1, a, std::move(b), 2};
            static_assert(std::is_same_v<decltype(apply(types, t)),
                          std::type_identity<std::tuple<int &, int &, int &, int const &>>>);
            static_assert(std::is_same_v<decltype(apply(types, std::as_const(t))),
                          std::type_identity<std::tuple<int const &, int &, int &, int const &>>>);
            static_assert(std::is_same_v<decltype(apply(types, std::move(t))),
                          std::type_identity<std::tuple<int &&, int &, int &&, int const &&>>>);
        };

        should("keep lvalue references of an rvalue tuple") = [] {
            auto x = 1;
            apply([](int & r) { r = 5; }, std::forward_as_tuple(x));
            expect(x == 5_i);
        };

        should("move the elements out of an rvalue tuple") = [] {
            auto const deref = [](std::unique_ptr<int> p) { return *p; };
            expect(apply(deref, std::tuple{std::make_unique<int>(4)}) == 4_i);
        };

        should("accept pairs, arrays and empty tuples") = [&] {
            expect(apply(sum, std::pair{1, 2}) == 3_i);
            expect(apply(prod, std::array{2, 3}) == 6_i);
            expect(apply([] { return 7; }, std::tuple<>{}) == 7_i);
        };

        should("be evaluable at compile time") = [] {
            constexpr auto add = [](int x, int y) { return x + y; };
            static_assert(apply(add, std::tuple{1, 2}) == 3);
            static_assert(apply(add)(std::pair{3, 4}) == 7);
        };
    };

    "compose_fn"_test = [] {
        using callables::compose;
        auto [twice, twice_expr] = DECLARE(([](auto n) { return 2 * n; }));
        auto [square, square_expr] = DECLARE(([](auto n) { return n * n; }));
        auto [sum, sum_expr] = DECLARE(([](auto ...args) { return (args + ...);}));
        should("compose(f(x), g(args))(x, y, ...) = f(g(x y, ...))") = [&] {
            expect(compose(twice, square)(2) == 8_i) << twice_expr << ", " << square_expr << "with 2";
            expect(compose(twice, square)(3) == 18_i) << twice_expr << ", " << square_expr << "with 3";
            expect(compose(twice, sum)(2, 3) == 10_i) << twice_expr << ", " << sum_expr << "with 2, 3";
            expect(compose(twice, sum)(1, 2, 3) == 12_i) << twice_expr << ", " << sum_expr << "with 1, 2, 3";
        };
        should("accept member pointers and plain functions") = [&] {
            auto const p = test::point{3, 2.5};
            expect(compose(twice, &test::point::x)(p) == 6_i);
            expect(compose(twice, test::x_of)(p) == 6_i);
        };
        should("not return references into temporaries") = [] {
            // x(make_point(5)): a reference into the temporary would not be a constant expression
            static_assert(compose(&test::point::x, test::make_point)(5) == 5);
            expect(compose(&test::point::x, test::make_point)(5) == 5_i);
        };
    };

    "on_fn"_test = [] {
        using callables::on;

        auto [twice, twice_expr] = DECLARE(([](auto n) { return 2 * n; }));
        auto [square, square_expr] = DECLARE(([](auto n) { return n * n; }));
        auto [sum, sum_expr] = DECLARE(([](auto ...args) { return (args + ...);}));
        should("on(f(args), g(x))(x, y, ...) = f(g(x), g(y), ...)") = [&] {
            expect(on(twice, sum)(3, 4) == 14_i) << sum_expr << ", " << twice_expr << "with 3, 4";
            expect(on(square, sum)(-2, -3) == 13_i) << sum_expr << ", " << square_expr << "with -2, -3";
            expect(on(twice, sum)(0, 1, 2, 3) == 12_i) << twice_expr << ", " << sum_expr << "with 0, 1, 2, 3";
        };
        should("accept member pointers and plain functions") = [] {
            auto const a = test::point{1, 0.};
            auto const b = test::point{2, 0.};
            expect(on(&test::point::x, callables::less_than)(a, b));
            expect(on(test::x_of, test::lower)(a, b));
            expect(on(test::x_of)(callables::less_than)(a, b));
            expect(on(&test::point::x)(test::lower)(a, b));
        };
    };

    "flip_fn"_test = [] {
        using callables::flip;
        should("accept member pointers and plain functions") = [] {
            auto const c = test::counter{10};
            expect(flip(&test::counter::minus, 3, c) == 7_i);
            expect(flip(&test::counter::minus)(3, c) == 7_i);
            expect(flip(test::lower, 2, 1));
        };

        should("store the callable by value") = [] {
            auto const offset = [k = 1](int a, int b) { return a - b + k; };
            static_assert(std::is_same_v<decltype(flip(offset)), callables::flip_fn::capture<std::decay_t<decltype(offset)>>>);
            expect(flip(offset)(1, 10) == 10_i);
        };
    };

    "operators"_test = [] {
        using callables::operators::operator*, callables::operators::operator+;
        should("compose with binary *") = [] {
            auto const twice = [](auto n) { return 2 * n; };
            auto const inc = [](auto n) { return n + 1; };
            expect((twice * inc)(3) == 8_i);
        };

        should("compose plain functions and member pointers with binary *") = [] {
            auto const twice = [](auto n) { return 2 * n; };
            auto const p = test::point{3, 2.5};
            auto const c = test::counter{10};
            expect((twice * test::x_of)(p) == 6_i);
            expect((twice * &test::x_of)(p) == 6_i);
            expect((twice * &test::point::x)(p) == 6_i);
            expect((twice * &test::counter::minus)(c, 4) == 12_i);
            constexpr auto make = [](int x) { return test::point{x, 0.}; };
            static_assert((&test::point::x * make)(5) == 5);
        };

        should("apply with unary +") = [] {
            auto const sum = [](auto a, auto b) { return a + b; };
            expect((+sum)(std::pair{1, 2}) == 3_i);
        };
    };

    "identity_fn"_test = [] {
        using callables::identity;
        should("return its argument unchanged") = [] {
            expect(identity(1) == 1_i);
            expect(identity(std::string("hi")) == "hi"_b);
        };
    };

    "construct_fn"_test = [] {
        using callables::construct;
        struct empty {
            empty() {}
            bool operator==(empty const &) const = default;
        };
        struct single {
            int ct;
            single(int x = 0) : ct{x} {}
            bool operator==(single const &) const = default;
        };
        struct multiple {
            int ct = -1;
            multiple(int x, int y) : ct{x + y} {}
            bool operator==(multiple const &) const = default;
        };
        struct overloaded {
            int ct = -1;
            overloaded(int x) : ct{x} {}
            overloaded(empty) {}
            bool operator==(overloaded const &) const = default;
        };

        should("construct an object of the given type") = [] {
            expect(construct<empty>() == empty());
            expect(construct<single>() == single());
            expect(construct<single>(12) == single(12));
            expect(construct<multiple>(1, 3) == multiple(3, 1));
            expect(construct<overloaded>(10) == overloaded(10));
            expect(construct<overloaded>(empty{}) == overloaded{empty{}});
        };

        should("construct an object of the given type from a tuple of arguments") = [] {
            expect(construct<empty>.from_tuple(std::tuple<>{}) == empty());
            expect(construct<single>.from_tuple(std::tuple{}) == single());
            expect(construct<single>.from_tuple(std::tuple{12}) == single(12));
            expect(construct<multiple>.from_tuple(std::tuple{1, 3}) == multiple(3, 1));
            expect(construct<overloaded>.from_tuple(std::tuple{10}) == overloaded(10));
            expect(construct<overloaded>.from_tuple(std::tuple{empty{}}) == overloaded{empty{}});
        };
    };

    "get_fn"_test = [] {
        should("extract N-th value using get") = []{
            auto example = std::tuple{1, 2., std::string_view{"three"}};
            auto external_get = test::external_get{{1, 2, 3}};
            auto member_get = test::external_get{{1, 2, 3}};
            expect(callables::get<0>(example) == 1_i);
            expect(callables::get<1>(example) == 2._d);
            expect(callables::get<2>(example) == std::string_view{"three"});
            expect(callables::get<0>(external_get) == 1_i);
            expect(callables::get<1>(external_get) == 2_i);
            expect(callables::get<2>(external_get) == 3_i);
            expect(callables::get<0>(member_get) == 1_i);
            expect(callables::get<1>(member_get) == 2_i);
            expect(callables::get<2>(member_get) == 3_i);
        };

        should("eventually return a reference") = [] {
            auto tup = std::tuple{1, 2, 3};
            callables::get<0>(tup) = 2;
            callables::get<1>(tup) = 4;
            callables::get<2>(tup) = 6;
            expect(tup == std::tuple{2, 4, 6});
        };

        should("extract a tuple of values using get") = [] {
            auto example = std::tuple{1, 2., std::string_view{"three"}};
            auto external_get = test::external_get{{1, 2, 3}};
            auto member_get = test::external_get{{1, 2, 3}};
            expect(callables::get<0, 1>(example) == std::tuple{1, 2.});
            expect(callables::get<0, 2>(external_get) == std::tuple{1, 3});
            expect(callables::get<1, 2>(member_get) == std::tuple{2, 3});
        };

        should("eventually return a tuple of references") = []{
            auto tup = std::tuple{1, 2, 3};
            callables::get<0, 1, 2>(tup) = std::tuple{2, 4, 6};
            expect(tup == std::tuple{2, 4, 6});
        };
    };

#if defined(__cpp_impl_reflection) && __cpp_impl_reflection >= 202506L
    "member_fn"_test = [] {
        using callables::member;
        should("extract a data member by name") = [] {
            auto const p = test::point{1, 2.5};
            expect(member<"x">(p) == 1_i);
            expect(member<"y">(p) == 2.5_d);
        };

        should("return a reference, like get") = [] {
            auto p = test::point{1, 2.5};
            member<"x">(p) = 3;
            expect(p.x == 3_i);
            static_assert(std::is_same_v<decltype(member<"x">(p)), int &>);
            static_assert(std::is_same_v<decltype(member<"x">(std::as_const(p))), int const &>);
            static_assert(std::is_same_v<decltype(member<"x">(test::point{})), int &&>);
        };

        should("extract a tuple of references") = [] {
            auto p = test::point{1, 2.5};
            member<"x", "y">(p) = std::tuple{4, 5.5};
            expect(p.x == 4_i);
            expect(p.y == 5.5_d);
            static_assert(std::is_same_v<decltype(member<"x", "y">(p)), std::tuple<int &, double &>>);
        };

        should("return a copy of a bit-field") = [] {
            auto const f = test::flags{1, 2};
            expect(member<"on">(f) == 1_u);
            static_assert(std::is_same_v<decltype(member<"on">(f)), unsigned>);
        };

        should("work as a projection") = [] {
            auto points = std::vector<test::point>{{3, 0.}, {1, 0.}, {2, 0.}};
            std::ranges::sort(points, {}, member<"x">);
            expect(points[0].x == 1_i);
            expect(points[1].x == 2_i);
            expect(points[2].x == 3_i);
        };

        should("be evaluable at compile time") = [] {
            static_assert(member<"y">(test::point{1, 2.5}) == 2.5);
        };
    };
#endif

    "at_fn"_test = [] {
        auto ct = std::vector{1, 2, 3};
        should("get the N-th value using `at`") = [&ct] {
            expect(callables::at(0)(ct) == 1_i);
            expect(callables::at(1)(ct) == 2_i);
            expect(callables::at(2)(ct) == 3_i);
            expect(throws([&]{ callables::at(3)(ct); })) << "index 3 is out of range";
        };
        should("get the N-th value using `[]`") = [&ct] {
            expect(callables::at[0](ct) == 1_i);
            expect(callables::at[1](ct) == 2_i);
            expect(callables::at[2](ct) == 3_i);
        };
        should("forward-return the correct type") = [] {
            auto v = std::vector{0, 1, 2, 3};
            for (auto i = 0u; i < v.size(); ++i) {
                callables::at(v, i) = i * i;
                expect(callables::at[i](v) == _i(i * i));
            }
        };
    };
    "from_container_fn"_test = [] {
        using callables::from_container;
        using capture = callables::from_container_fn;
        using vec = std::vector<int>;

        should("access the container via `at`, `[]` or iterators") = [] {
            auto v = std::vector{1, 2, 3};
            auto m = std::map<std::string, int>{{"a", 1}, {"b", 2}};
            auto l = std::list{1, 2, 3};
            expect(from_container(v, 1) == 2_i);
            expect(from_container(m, "b"s) == 2_i);
            expect(from_container(l, 2) == 3_i);
            expect(throws([&]{ from_container(v, 3); })) << "index 3 is out of range";
        };

        should("capture lvalues by reference") = [] {
            auto v = std::vector{1, 2, 3};
            auto const fn = from_container(v);
            v[0] = 42;
            expect(fn(0) == 42_i);
            static_assert(std::is_same_v<capture::capture_t<vec &>, vec &>);
            static_assert(std::is_same_v<capture::capture_t<vec const &>, vec const &>);
        };

        should("keep the interface of the captured container") = [] {
            auto m = std::map<std::string, int>{{"a", 1}};
            auto d = std::deque{1, 2, 3};
            auto const by_key = from_container(m);
            auto const checked = from_container(d);
            m["a"] = 2;
            expect(by_key("a"s) == 2_i);
            expect(throws([&]{ std::ignore = checked(3); })) << "deque::at should still be used";
        };

        should("capture rvalues by value") = [] {
            auto v = std::vector{1, 2, 3};
            auto const copy = from_container(auto{v});
            auto const owned = from_container(std::vector{7, 8});
            v[0] = 42;
            expect(copy(0) == 1_i);
            expect(owned(1) == 8_i);
            static_assert(std::is_same_v<capture::capture_t<vec>, vec>);
        };

        should("unwrap reference_wrappers") = [] {
            auto v = std::vector{1, 2, 3};
            auto r = std::ref(v);
            auto const from_temporary = from_container(std::ref(v));
            auto const from_named = from_container(r);
            v[0] = 42;
            expect(from_temporary(0) == 42_i);
            expect(from_named(0) == 42_i);
            static_assert(std::is_same_v<capture::capture_t<std::reference_wrapper<vec>>, vec &>);
            static_assert(std::is_same_v<capture::capture_t<std::reference_wrapper<vec> &>, vec &>);
            static_assert(std::is_same_v<capture::capture_t<std::reference_wrapper<vec> const &>, vec &>);
            static_assert(std::is_same_v<capture::capture_t<std::reference_wrapper<vec const> &>, vec const &>);
        };

        should("accept views and built-in arrays") = [] {
            int arr[] = {1, 2, 3};
            auto const from_array = from_container(arr);
            auto const from_iota = from_container(std::views::iota(0, 5));
            arr[0] = 42;
            expect(from_array(0) == 42_i);
            expect(from_iota(3) == 3_i);
        };

        should("work as a projection in a pipeline") = [] {
            auto const names = std::vector{"zero"s, "one"s, "two"s};
            auto const picked = std::vector{2, 0}
                              | std::views::transform(from_container(names))
                              | std::ranges::to<std::vector>();
            expect(picked == std::vector{"two"s, "zero"s});
        };
    };

    "not_fn"_test = [] {
        using callables::not_;
        auto [p1, p1_expr] = DECLARE([](int a, int b) { return a == b; });
        auto [p2, p2_expr] = DECLARE([](auto x) { return x.size() == 1; });
        should("negate function results immediately") = [&]{
            expect(!not_(p1, 1, 1)) << p1_expr << "with arguments 1, 1";
            expect(not_(p1, 1, 2)) << "not" << p1_expr << "with arguments 1, 2";
            expect(!not_(p2, std::array<float, 1>{})) << p2_expr << "with arg array<float, 1>{}";
            expect(not_(p2, std::array<float, 2>{})) << "not" << p2_expr << "with arg array<float, 2>{}";
        };
        should("negate function results with currying") = [&]{
            auto sa = not_(p1);
            expect(!not_(p1)(1, 1)) << p1_expr << "with arguments 1, 1";
            expect(not_(p1, 1, 2)) << "not" << p1_expr << "with arguments 1, 2";
            expect(!not_(p2, std::array<float, 1>{})) << p2_expr << "with arg array<float, 1>{}";
            expect(not_(p2, std::array<float, 2>{})) << "not" << p2_expr << "with arg array<float, 2>{}";
        };
    };

    "graph_fn"_test = [] {
        using callables::graph;
        auto [cmp, cmp_expr] = DECLARE([](auto a, auto b) { return a == b; });
        auto [square, square_expr] = DECLARE(([](auto n) { return n * n; }));

        should("return a tuple containing the arguments and the invocation result") = [&] {
            expect(graph(square, 3) == std::tuple{3, 9}) << square_expr << ", 3";
            expect(graph(square, -9.) == std::tuple{-9., 81.}) << square_expr << ", -9.";
            expect(graph(cmp, 3, 3) == std::tuple{3, 3, true}) << cmp_expr << ", 3, 3";
            expect(graph(cmp, std::string_view{"42"}, "7") == std::tuple{std::string_view{"42"}, "7", false}) << cmp_expr << ", \"42\"sv, \"7\"";
        };

        should("be partially-applicable") = [&]{
            expect(graph(square)(3) == std::tuple{3, 9}) << square_expr << ", 3";
            expect(graph(square)(-9.) == std::tuple{-9., 81.}) << square_expr << ", -9.";
            expect(graph(cmp)(3, 3) == std::tuple{3, 3, true}) << cmp_expr << ", 3, 3";
            expect(graph(cmp)(std::string_view{"42"}, "7") == std::tuple{std::string_view{"42"}, "7", false}) << cmp_expr << ", \"42\"sv, \"7\"";
        };

        should("accept member pointers") = [] {
            auto const c = test::counter{10};
            expect(std::get<1>(graph(&test::counter::x, c)) == 10_i);
        };

        should("not move from the arguments it returns") = [] {
            auto const size = [](std::string s) { return s.size(); };
            expect(graph(size, std::string{"hello"}) == std::tuple{std::string{"hello"}, std::size_t{5}});
        };
    };
}

#undef DECLARE
