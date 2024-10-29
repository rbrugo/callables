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
        using callables::less_than;
        should("immediately evaluable") = [=] {
            expect(less_than(1, 2)) << "int";
            expect(less_than(1., 2)) << "double < int";
            expect(less_than("a"s, "aa")) << "string < char const *";

            expect(not less_than(1, 1)) << "int";
            expect(not less_than(1., 1)) << "double < int";
            expect(not less_than("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(less_than(2)(1)) << "int";
            expect(less_than(2.)(1.)) << "double";
            expect(less_than("aa"s)("a"s)) << "string";

            expect(not less_than(1)(1)) << "int";
            expect(not less_than(1)(1)) << "double < int";
            expect(not less_than("x"s)("x")) << "string < char const *";
        };
        should("left-bindable") = [=] {
            expect(less_than.left(1)(2)) << "int";
            expect(less_than.left(1.)(2.)) << "double";
            expect(less_than.left("a"s)("aa"s)) << "string";

            expect(not less_than.left(1)(1)) << "int";
            expect(not less_than.left(1)(1)) << "double < int";
            expect(not less_than.left("x"s)("x")) << "string < char const *";
        };
        should("right-bindable") = [=] {
            expect(less_than.right(2)(1)) << "int";
            expect(less_than.right(2.)(1.)) << "double";
            expect(less_than.right("ab"s)("aa"s)) << "string";

            expect(not less_than.right(1)(1)) << "int";
            expect(not less_than.right(1)(1)) << "double < int";
            expect(not less_than.right("x"s)("x")) << "string < char const *";
        };
        should("be callable with a pair argument") = [=] {
            expect(less_than.tuple(std::pair{1, 2})) << "int";
            expect(less_than.tuple(std::pair{1., 2.})) << "double";
            expect(less_than.tuple(std::pair{"a"s, "aa"})) << "string";

            expect(not less_than.tuple(std::pair{1, 1})) << "int";
            expect(not less_than.tuple(std::pair{1, 1})) << "double < int";
            expect(not less_than.tuple(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(less_than.tuple(std::tuple{1, 2})) << "int";
            expect(less_than.tuple(std::tuple{1., 2.})) << "double";
            expect(less_than.tuple(std::tuple{"a"s, "aa"})) << "string";
        };
    };

    "less_equal_fn"_test = [] {
        using callables::less_equal;
        should("immediately evaluable") = [=] {
            expect(less_equal(1, 2)) << "int";
            expect(less_equal(1., 2)) << "double < int";
            expect(less_equal("a"s, "aa")) << "string < char const *";

            expect(less_equal(1, 1)) << "int";
            expect(less_equal(1., 1)) << "double < int";
            expect(less_equal("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(less_equal(2)(1)) << "int";
            expect(less_equal(2.)(1.)) << "double";
            expect(less_equal("aa"s)("a"s)) << "string";

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
            expect(less_equal.tuple(std::pair{1, 2})) << "int";
            expect(less_equal.tuple(std::pair{1., 2.})) << "double";
            expect(less_equal.tuple(std::pair{"a"s, "aa"})) << "string";

            expect(less_equal.tuple(std::pair{1, 1})) << "int";
            expect(less_equal.tuple(std::pair{1, 1})) << "double < int";
            expect(less_equal.tuple(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(less_equal.tuple(std::tuple{1, 2})) << "int";
            expect(less_equal.tuple(std::tuple{1., 2.})) << "double";
            expect(less_equal.tuple(std::tuple{"a"s, "aa"})) << "string";
        };
    };


    "greater_fn"_test = [] {
        using callables::greater_than;
        should("immediately evaluable") = [=] {
            expect(greater_than(3, 2)) << "int";
            expect(greater_than(3., 2)) << "double < int";
            expect(greater_than("aaa"s, "aa")) << "string < char const *";

            expect(not greater_than(1, 1)) << "int";
            expect(not greater_than(1., 1)) << "double < int";
            expect(not greater_than("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(greater_than(2)(3)) << "int";
            expect(greater_than(2.)(3.)) << "double";
            expect(greater_than("aa"s)("aaa"s)) << "string";

            expect(not greater_than(1)(1)) << "int";
            expect(not greater_than(1)(1)) << "double < int";
            expect(not greater_than("x"s)("x")) << "string < char const *";
        };
        should("left-bindable") = [=] {
            expect(greater_than.left(3)(2)) << "int";
            expect(greater_than.left(3.)(2.)) << "double";
            expect(greater_than.left("aaa"s)("aa"s)) << "string";

            expect(not greater_than.left(1)(1)) << "int";
            expect(not greater_than.left(1)(1)) << "double < int";
            expect(not greater_than.left("x"s)("x")) << "string < char const *";
        };
        should("right-bindable") = [=] {
            expect(greater_than.right(2)(3)) << "int";
            expect(greater_than.right(2.)(3.)) << "double";
            expect(greater_than.right("ab"s)("aba"s)) << "string";

            expect(not greater_than.right(1)(1)) << "int";
            expect(not greater_than.right(1)(1)) << "double < int";
            expect(not greater_than.right("x"s)("x")) << "string < char const *";
        };
        should("be callable with a pair argument") = [=] {
            expect(greater_than.tuple(std::pair{3, 2})) << "int";
            expect(greater_than.tuple(std::pair{3., 2.})) << "double";
            expect(greater_than.tuple(std::pair{"aaa"s, "aa"})) << "string";

            expect(not greater_than.tuple(std::pair{1, 1})) << "int";
            expect(not greater_than.tuple(std::pair{1, 1})) << "double < int";
            expect(not greater_than.tuple(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(greater_than.tuple(std::tuple{3, 2, 1.0})) << "int";
            expect(greater_than.tuple(std::tuple{3., 2., 1})) << "double";
            expect(greater_than.tuple(std::tuple{"aaa"s, "aa", "a"sv})) << "string";
        };
    };

    "greater_equal_fn"_test = [] {
        using callables::greater_equal;
        should("immediately evaluable") = [=] {
            expect(greater_equal(3, 2)) << "int";
            expect(greater_equal(3., 2)) << "double < int";
            expect(greater_equal("aaa"s, "aa")) << "string < char const *";

            expect(greater_equal(1, 1)) << "int";
            expect(greater_equal(1., 1)) << "double < int";
            expect(greater_equal("x"s, "x")) << "string < char const *";
        };
        should("partial-applicable") = [=] {
            expect(greater_equal(2)(3)) << "int";
            expect(greater_equal(2.)(3.)) << "double";
            expect(greater_equal("aa"s)("aaa"s)) << "string";

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
            expect(greater_equal.tuple(std::pair{3, 2})) << "int";
            expect(greater_equal.tuple(std::pair{3., 2.})) << "double";
            expect(greater_equal.tuple(std::pair{"aaa"s, "aa"})) << "string";

            expect(greater_equal.tuple(std::pair{1, 1})) << "int";
            expect(greater_equal.tuple(std::pair{1, 1})) << "double < int";
            expect(greater_equal.tuple(std::pair{"x"s, "x"})) << "string < char const *";
        };
        should("callable with a binary tuple argument") = [=] {
            expect(greater_equal.tuple(std::tuple{3, 2, 0.1})) << "int";
            expect(greater_equal.tuple(std::tuple{3., 2., 1})) << "double";
            expect(greater_equal.tuple(std::tuple{"aaa"s, "aa", "a"sv})) << "string";
        };
    };
}
