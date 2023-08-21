/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : ordering
 * @created     : Monday Aug 21, 2023 19:40:58 CEST
 * @description : 
 */


#include <brun/callables/ordering.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

using namespace std::literals;

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;

    "less_fn"_test = [] {
        using brun::less;
        should("immediately evaluable") = [=] {
            expect(less(1, 2)) << "int";
            expect(less(1., 2)) << "double < int";
            expect(less("a"s, "aa")) << "string < char const *";

            expect(not less(1, 1)) << "int";
            expect(not less(1., 1)) << "double < int";
            expect(not less("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(less(1)(2)) << "int";
            expect(less(1.)(2.)) << "double";
            expect(less("a"s)("aa"s)) << "string";

            expect(not less(1)(1)) << "int";
            expect(not less(1)(1)) << "double < int";
            expect(not less("x"s)("x")) << "string < char const *";
        };
        should("left-bindable") = [=] {
            expect(less.left(1)(2)) << "int";
            expect(less.left(1.)(2.)) << "double";
            expect(less.left("a"s)("aa"s)) << "string";

            expect(not less.left(1)(1)) << "int";
            expect(not less.left(1)(1)) << "double < int";
            expect(not less.left("x"s)("x")) << "string < char const *";
        };
        should("right-bindable") = [=] {
            expect(less.right(2)(1)) << "int";
            expect(less.right(2.)(1.)) << "double";
            expect(less.right("ab"s)("aa"s)) << "string";

            expect(not less.right(1)(1)) << "int";
            expect(not less.right(1)(1)) << "double < int";
            expect(not less.right("x"s)("x")) << "string < char const *";
        };
        should("be callable with a pair argument") = [=] {
            expect(less(std::pair{1, 2})) << "int";
            expect(less(std::pair{1., 2.})) << "double";
            expect(less(std::pair{"a"s, "aa"})) << "string";

            expect(not less(std::pair{1, 1})) << "int";
            expect(not less(std::pair{1, 1})) << "double < int";
            expect(not less(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(less(std::tuple{1, 2})) << "int";
            expect(less(std::tuple{1., 2.})) << "double";
            expect(less(std::tuple{"a"s, "aa"})) << "string";
        };
    };

    "less_equal_fn"_test = [] {
        using brun::less_equal;
        should("immediately evaluable") = [=] {
            expect(less_equal(1, 2)) << "int";
            expect(less_equal(1., 2)) << "double < int";
            expect(less_equal("a"s, "aa")) << "string < char const *";

            expect(less_equal(1, 1)) << "int";
            expect(less_equal(1., 1)) << "double < int";
            expect(less_equal("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(less_equal(1)(2)) << "int";
            expect(less_equal(1.)(2.)) << "double";
            expect(less_equal("a"s)("aa"s)) << "string";

            expect(less_equal(1)(1)) << "int";
            expect(less_equal(1)(1)) << "double < int";
            expect(less_equal("x"s)("x")) << "string < char const *";
        };
        should("left-bindable") = [=] {
            expect(less_equal.left(1)(2)) << "int";
            expect(less_equal.left(1.)(2.)) << "double";
            expect(less_equal.left("a"s)("aa"s)) << "string";

            expect(less_equal.left(1)(1)) << "int";
            expect(less_equal.left(1)(1)) << "double < int";
            expect(less_equal.left("x"s)("x")) << "string < char const *";
        };
        should("right-bindable") = [=] {
            expect(less_equal.right(2)(1)) << "int";
            expect(less_equal.right(2.)(1.)) << "double";
            expect(less_equal.right("ab"s)("aa"s)) << "string";

            expect(less_equal.right(1)(1)) << "int";
            expect(less_equal.right(1)(1)) << "double < int";
            expect(less_equal.right("x"s)("x")) << "string < char const *";
        };
        should("be callable with a pair argument") = [=] {
            expect(less_equal(std::pair{1, 2})) << "int";
            expect(less_equal(std::pair{1., 2.})) << "double";
            expect(less_equal(std::pair{"a"s, "aa"})) << "string";

            expect(less_equal(std::pair{1, 1})) << "int";
            expect(less_equal(std::pair{1, 1})) << "double < int";
            expect(less_equal(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(less_equal(std::tuple{1, 2})) << "int";
            expect(less_equal(std::tuple{1., 2.})) << "double";
            expect(less_equal(std::tuple{"a"s, "aa"})) << "string";
        };
    };


    "greater_fn"_test = [] {
        using brun::greater;
        should("immediately evaluable") = [=] {
            expect(greater(3, 2)) << "int";
            expect(greater(3., 2)) << "double < int";
            expect(greater("aaa"s, "aa")) << "string < char const *";

            expect(not greater(1, 1)) << "int";
            expect(not greater(1., 1)) << "double < int";
            expect(not greater("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(greater(3)(2)) << "int";
            expect(greater(3.)(2.)) << "double";
            expect(greater("aaa"s)("aa"s)) << "string";

            expect(not greater(1)(1)) << "int";
            expect(not greater(1)(1)) << "double < int";
            expect(not greater("x"s)("x")) << "string < char const *";
        };
        should("left-bindable") = [=] {
            expect(greater.left(3)(2)) << "int";
            expect(greater.left(3.)(2.)) << "double";
            expect(greater.left("aaa"s)("aa"s)) << "string";

            expect(not greater.left(1)(1)) << "int";
            expect(not greater.left(1)(1)) << "double < int";
            expect(not greater.left("x"s)("x")) << "string < char const *";
        };
        should("right-bindable") = [=] {
            expect(greater.right(2)(3)) << "int";
            expect(greater.right(2.)(3.)) << "double";
            expect(greater.right("ab"s)("aba"s)) << "string";

            expect(not greater.right(1)(1)) << "int";
            expect(not greater.right(1)(1)) << "double < int";
            expect(not greater.right("x"s)("x")) << "string < char const *";
        };
        should("be callable with a pair argument") = [=] {
            expect(greater(std::pair{3, 2})) << "int";
            expect(greater(std::pair{3., 2.})) << "double";
            expect(greater(std::pair{"aaa"s, "aa"})) << "string";

            expect(not greater(std::pair{1, 1})) << "int";
            expect(not greater(std::pair{1, 1})) << "double < int";
            expect(not greater(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(greater(std::tuple{3, 2})) << "int";
            expect(greater(std::tuple{3., 2.})) << "double";
            expect(greater(std::tuple{"aaa"s, "aa"})) << "string";
        };
    };

    "greater_equal_fn"_test = [] {
        using brun::greater_equal;
        should("immediately evaluable") = [=] {
            expect(greater_equal(3, 2)) << "int";
            expect(greater_equal(3., 2)) << "double < int";
            expect(greater_equal("aaa"s, "aa")) << "string < char const *";

            expect(greater_equal(1, 1)) << "int";
            expect(greater_equal(1., 1)) << "double < int";
            expect(greater_equal("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(greater_equal(3)(2)) << "int";
            expect(greater_equal(3.)(2.)) << "double";
            expect(greater_equal("aaa"s)("aa"s)) << "string";

            expect(greater_equal(1)(1)) << "int";
            expect(greater_equal(1)(1)) << "double < int";
            expect(greater_equal("x"s)("x")) << "string < char const *";
        };
        should("left-bindable") = [=] {
            expect(greater_equal.left(3)(2)) << "int";
            expect(greater_equal.left(3.)(2.)) << "double";
            expect(greater_equal.left("aaa"s)("aa"s)) << "string";

            expect(greater_equal.left(1)(1)) << "int";
            expect(greater_equal.left(1)(1)) << "double < int";
            expect(greater_equal.left("x"s)("x")) << "string < char const *";
        };
        should("right-bindable") = [=] {
            expect(greater_equal.right(2)(3)) << "int";
            expect(greater_equal.right(2.)(3.)) << "double";
            expect(greater_equal.right("ab"s)("aba"s)) << "string";

            expect(greater_equal.right(1)(1)) << "int";
            expect(greater_equal.right(1)(1)) << "double < int";
            expect(greater_equal.right("x"s)("x")) << "string < char const *";
        };
        should("be callable with a pair argument") = [=] {
            expect(greater_equal(std::pair{3, 2})) << "int";
            expect(greater_equal(std::pair{3., 2.})) << "double";
            expect(greater_equal(std::pair{"aaa"s, "aa"})) << "string";

            expect(greater_equal(std::pair{1, 1})) << "int";
            expect(greater_equal(std::pair{1, 1})) << "double < int";
            expect(greater_equal(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(greater_equal(std::tuple{3, 2})) << "int";
            expect(greater_equal(std::tuple{3., 2.})) << "double";
            expect(greater_equal(std::tuple{"aaa"s, "aa"})) << "string";
        };
    };
}
