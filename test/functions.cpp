/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : functions
 * @created     : Monday Aug 21, 2023 17:33:45 CEST
 * @description : 
 */

#include <brun/callables/functions.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

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
}  // namespace test



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
    };

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
}

#undef DECLARE
