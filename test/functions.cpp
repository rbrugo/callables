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


int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;

    "apply_fn"_test = [] {
        using brun::apply;
        auto [sum, sum_expr] = DECLARE(([](int a, int b) { return a + b;}));
        auto [prod, prod_expr] = DECLARE(([](auto ...n) { return (n * ...); }));

        should("apply its arguments to the stored function") = [&] {
            expect(apply(sum)(std::tuple{1, 2}) == 3_i) << sum_expr << "with arguments 1, 2";
            expect(apply(prod)(std::tuple{1, 2, 3}) == 6_i) << prod_expr << "with arguments 1, 2, 3";
            expect(apply(prod)(std::tuple{2, 2, 2, 2}) == 16_i) << prod_expr << "with arguments 2, 2, 2, 2";
        };
    };

    "compose_fn"_test = [] {
        using brun::compose;
        auto [twice, twice_expr] = DECLARE(([](auto n) { return 2 * n; }));
        auto [square, square_expr] = DECLARE(([](auto n) { return n * n; }));
        auto [sum, sum_expr] = DECLARE(([](int a, int b) { return a + b;}));
        should("compose(f(x), g(args))(x, y, ...) = f(g(x y, ...))") = [&] {
            expect(compose(twice, square)(2) == 8_i) << twice_expr << ", " << square_expr << "with 2";
            expect(compose(twice, square)(3) == 18_i) << twice_expr << ", " << square_expr << "with 3";
            expect(compose(twice, sum)(2, 3) == 10_i) << twice_expr << ", " << sum_expr << "with 2, 3";
        };
        should("compose(f(args), g(x))(x, y, ...) = f(g(x), g(y), ...)") = [&] {
            expect(compose(sum, twice)(3, 4) == 14_i) << sum_expr << ", " << twice_expr << "with 3, 4";
            expect(compose(sum, square)(-2, -3) == 13_i) << sum_expr << ", " << square_expr << "with -2, -3";
        };
    };

    "identity_fn"_test = [] {
        using brun::identity;
        should("return its argument unchanged") = [] {
            expect(identity(1) == 1_i);
            expect(identity(std::string("hi")) == "hi"_b);
        };
    };
}

#undef DECLARE
