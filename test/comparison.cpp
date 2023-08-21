/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : comparison
 * @created     : Monday Aug 21, 2023 19:17:59 CEST
 * @description :
 */

#include <brun/callables/comparison.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

using namespace std::literals;

struct {
    template <typename T> bool operator==(T &&) const noexcept { return true; }
} doppleganger;
struct {
    template <typename T> bool operator==(T &&) const noexcept { return false; }
} oddball;

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;

    "equal_to_fn"_test = [] {
        using brun::equal_to;
        auto v = std::vector<int>{};
        should("immediately evaluable") = [=] {
            expect(equal_to(1, 1));
            expect(equal_to(3., 3.));
            expect(equal_to("a"s, "a"s));
            expect(equal_to(v.begin(), v.end()));
            expect(equal_to(doppleganger, v.end()));
            expect(not equal_to(12, 30));
            expect(not equal_to(nullptr, oddball));
        };
        should("partial-applicable") = [=] {
            expect(equal_to(1)(1));
            expect(equal_to(-1.)(-1.));
            expect(equal_to("rock"s)("rock"));
            expect(equal_to(v.begin())(v.end()));
            expect(equal_to(doppleganger)(0));
        };
        should("left-bindable") = [=] {
            expect(equal_to.left(1)(1));
            expect(equal_to.left(-1.)(-1.));
            expect(equal_to.left("a"s)("a"));
            expect(equal_to.left(v.begin())(v.end()));
            expect(equal_to.left(doppleganger)(std::string{}));
        };
        should("right-bindable") = [=] {
            expect(equal_to.right(5)(5));
            expect(equal_to.right(-10.)(-10.));
            expect(equal_to.right("s")("s"s));
            expect(equal_to.right(v.begin())(v.end()));
            expect(equal_to.right(nullptr)(doppleganger));
        };
        should("callable with a pair argument") = [=] {
            expect(equal_to(std::pair{1, 1}));
            expect(equal_to(std::pair{1., 1.}));
            expect(equal_to(std::pair{"hello"s, "hello"}));
            expect(equal_to(std::pair{v.begin(), v.end()}));
        };
        should("callable with a binary tuple argument") = [=] {
            expect(equal_to(std::tuple{125, 125}));
            expect(equal_to(std::tuple{-150., -150.}));
            expect(equal_to(std::tuple{""s, ""}));
            expect(equal_to(std::tuple{v.begin(), v.end()}));
        };
    };

    "not_equal_to_fn"_test = [] {
        using brun::not_equal_to;
        auto v = std::vector<int>{1};
        should("immediately evaluable") = [=] {
            expect(not_equal_to(1, 2));
            expect(not_equal_to(3., 4.));
            expect(not_equal_to("a"s, "b"s));
            expect(not_equal_to(v.begin(), v.end()));
            expect(not_equal_to(oddball, v.end()));

            expect(not not_equal_to(-3, -3));
            expect(not not_equal_to(nullptr, doppleganger));
        };
        should("partial-applicable") = [=] {
            expect(not_equal_to(1)(2));
            expect(not_equal_to(-1.)(-2.));
            expect(not_equal_to("rock"s)("water"));
            expect(not_equal_to(v.begin())(v.end()));
            expect(not_equal_to(oddball)(0));
        };
        should("left-bindable") = [=] {
            expect(not_equal_to.left(1)(2));
            expect(not_equal_to.left(1.)(-2.));
            expect(not_equal_to.left("one"s)("1"));
            expect(not_equal_to.left(v.begin())(v.end()));
            expect(not_equal_to.left(oddball)(std::string{}));
        };
        should("right-bindable") = [=] {
            expect(not_equal_to.right(5)(6));
            expect(not_equal_to.right(10.)(-11.));
            expect(not_equal_to.right("s")("S"s));
            expect(not_equal_to.right(v.begin())(v.end()));
            expect(not_equal_to.right(nullptr)(oddball));
        };
        should("be callable with a pair argument") = [=] {
            expect(not_equal_to(std::pair{1, 2}));
            expect(not_equal_to(std::pair{1., 0.}));
            expect(not_equal_to(std::pair{"hello"s, "bye"}));
            expect(not_equal_to(std::pair{v.begin(), v.end()}));
        };
        should("be callable with a binary tuple argument") = [=] {
            expect(not_equal_to(std::tuple{125, 126}));
            expect(not_equal_to(std::tuple{150., -151.}));
            expect(not_equal_to(std::tuple{""s, "0"}));
            expect(not_equal_to(std::tuple{v.begin(), v.end()}));
        };
    };
}
