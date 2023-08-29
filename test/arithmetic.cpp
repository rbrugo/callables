/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : arithmetic
 * @created     : Monday Aug 21, 2023 14:35:01 CEST
 * @description : 
 * */

#include <string>
#include <filesystem>
#include <brun/callables/arithmetic.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

using namespace std::literals;

auto operator""_p(char const * str, std::size_t)
{ return std::filesystem::path(str); }

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;
    "plus_fn"_test = [] {
        using brun::plus;
        should("immediately evaluable") = [] {
            expect(plus(1, 2) == 3_i);
            expect(plus(1., -1.) == 0._d);
            expect(plus("a"s, "bc"s) == "abc"s);
        };
        should("partial-applicable") = [] {
            expect(plus(1)(2) == 3_i);
            expect(plus(1.)(-1.) == 0._d);
            expect(plus("a"s)("bc"s) == "abc"s);
        };
         should("left-bindable") = [] {
             expect(plus.left(1)(2) == 3_i);
             expect(plus.left(1.)(-1.) == 0._d);
             expect(plus.left("a"s)("bc"s) == "abc"s);
         };
         should("right-bindable") = [] {
             expect(plus.right(1)(2) == 3_i);
             expect(plus.right(1.)(-1.) == 0._d);
             expect(plus.right("a"s)("bc"s) == "bca"s);
         };
         should("callable with a pair argument") = [] {
             expect(plus(std::pair{1, 2}) == 3_i);
             expect(plus(std::pair{1., -1.}) == 0._d);
             expect(plus(std::pair{"a"s, "bc"s}) == "abc"s);

             int x = 0;
             int y = 0;
             expect(plus(std::pair<int const &, int const &>(x, y)) == 0);
         };
         should("callable with a binary tuple argument") = [] {
             expect(plus(std::tuple{1, 2}) == 3_i);
             expect(plus(std::tuple{1., -1.}) == 0._d);
             expect(plus(std::tuple{"a"s, "bc"s}) == "abc"s);
         };
    };

    "minus_fn"_test = [] {
        using brun::minus;
        should("immediately evaluable") = [] {
            expect(minus(1, 2) == -1_i);
            expect(minus(1., -1.) == 2._d);
        };
        should("partial-applicable") = [] {
            expect(minus(1)(2) == -1_i);
            expect(minus(1.)(-1.) == 2._d);
        };
        should("left-bindable") = [] {
            expect(minus.left(1)(2) == -1_i);
            expect(minus.left(1.)(-1.) == 2._d);
        };
        should("right-bindable") = [] {
            expect(minus.right(1)(2) == 1_i);
            expect(minus.right(1.)(-1.) == -2._d);
        };
        should("callable with a pair argument") = [] {
            expect(minus(std::pair{1, 2}) == -1_i);
            expect(minus(std::pair{1., -1.}) == 2._d);
            int x = 0;
            int y = 0;
            expect(minus(std::pair<int const &, int const &>(x, y)) == 0);
        };
        should("callable with a binary tuple argument") = [] {
            expect(minus(std::tuple{1, 2}) == -1_i);
            expect(minus(std::tuple{1., -1.}) == 2._d);
        };
    };

    "multiplies_fn"_test = [] {
        using brun::multiplies;
        should("immediately evaluable") = [] {
            expect(multiplies(1, 2) == 2_i);
            expect(multiplies(1., -1.) == -1._d);
        };
        should("partial-applicable") = [] {
            expect(multiplies(1)(2) == 2_i);
            expect(multiplies(1.)(-1.) == -1._d);
        };
        should("left-bindable") = [] {
            expect(multiplies.left(1)(2) == 2_i);
            expect(multiplies.left(1.)(-1.) == -1._d);
        };
        should("right-bindable") = [] {
            expect(multiplies.right(1)(2) == 2_i);
            expect(multiplies.right(1.)(-1.) == -1._d);
        };
        should("callable with a pair argument") = [] {
            expect(multiplies(std::pair{1, 2}) == 2_i);
            expect(multiplies(std::pair{1., -1.}) == -1._d);
        };
        should("callable with a binary tuple argument") = [] {
            expect(multiplies(std::tuple{1, 2}) == 2_i);
            expect(multiplies(std::tuple{1., -1.}) == -1._d);
        };
    };

    "divides_fn"_test = [] {
        using brun::divides;
        should("immediately evaluable") = [] {
            expect(divides(10, 2) == 5_i);
            expect(divides(20., -20.) == -1._d);
            expect(divides("/usr"_p, "include"_p).string() == "/usr/include"_b);
        };
        should("partial-applicable") = [] {
            expect(divides(10)(2) == 5_i);
            expect(divides(20.)(-20.) == -1._d);
            expect(divides("/usr"_p)("include"_p).string() == "/usr/include"_b);
        };
        should("left-bindable") = [] {
            expect(divides.left(10)(2) == 5_i);
            expect(divides.left(20.)(-20.) == -1._d);
            expect(divides.left("abc"_p)("def").string() == "abc/def"_b);
        };
        should("right-bindable") = [] {
            expect(divides.right(10)(20) == 2_i);
            expect(divides.right(20.)(-20.) == -1._d);
            expect(divides.right("abc")("def"_p).string() == "def/abc"_b) << divides.right("abc")("def"_p);
        };
        should("callable with a pair argument") = [] {
            expect(divides(std::pair{10, 2}) == 5_i);
            expect(divides(std::pair{20., -20.}) == -1._d);
            expect(divides(std::pair{"/usr"_p, "include"_p}).string() == "/usr/include"_b);
        };
        should("callable with a binary tuple argument") = [] {
            expect(divides(std::tuple{10, 2}) == 5_i);
            expect(divides(std::tuple{20., -20.}) == -1._d);
            expect(divides(std::tuple{"/usr"_p, "include"_p}).string() == "/usr/include"_b);
        };
    };

    "negate_fn"_test = [] {
        using brun::negate;
        should("immediately evaluable") = [] {
            expect(negate(10) == -10_i);
            expect(negate(20.) == -20._d);
            expect(negate(uint8_t{1}) == -1_u8);
        };
    };
};
