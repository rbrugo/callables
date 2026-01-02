/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Friday Jan 02, 2026 22:03:08 CET
 * @description : 
 */

#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

#include <array>

template <std::size_t N>
constexpr auto operator and(std::array<bool, N> a, std::array<bool, N> const & b) noexcept
{
    for (auto i = 0ul; i < a.size(); ++i) {
        a[i] = a[i] and b[i];
    }
    return a;
}

template <std::size_t N>
constexpr auto operator or(std::array<bool, N> a, std::array<bool, N> b)
{
    for (auto i = 0ul; i < a.size(); ++i) {
        a[i] = a[i] or b[i];
    }
    return a;
}

template <std::size_t N>
constexpr auto operator xor(std::array<bool, N> a, std::array<bool, N> const & b) noexcept
{
    for (auto i = 0ul; i < a.size(); ++i) {
        a[i] = a[i] xor b[i];
    }
    return a;
}

template <std::size_t N>
constexpr auto operator not(std::array<bool, N> a) noexcept
{
    for (auto & elem : a) {
        elem = not elem;
    }
    return a;
}

#include <brun/callables/logical.hpp>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;
    constexpr auto a1 = std::array{true, true, false, false};
    constexpr auto a2 = std::array{true, false, true, false};

    "logical_and_fn"_test = [&] {
        constexpr auto re = std::array{true, false, false, false};
        static_assert((a1 and a2) == re);

        using callables::logical_and;
        should("immediately evaluable") = [&]{
            expect(logical_and(true, true) == true);
            expect(logical_and(true, false) == false);
            expect(logical_and(false, true) == false);
            expect(logical_and(false, false) == false);

            expect(logical_and(a1, a2) == re);
        };
        should("partially-applicable") = [&]{
            expect(logical_and(true)(true) == true);
            expect(logical_and(true)(false) == false);
            expect(logical_and(false)(true) == false);
            expect(logical_and(false)(false) == false);

            expect(logical_and(a1)(a2) == re);
        };
        should("left-bindable") = []{
            expect(logical_and.left(true)(true) == true);
            expect(logical_and.left(true)(false) == false);
            expect(logical_and.left(false)(true) == false);
            expect(logical_and.left(false)(false) == false);
        };
        should("right-bindable") = []{
            expect(logical_and.right(true)(true) == true);
            expect(logical_and.right(true)(false) == false);
            expect(logical_and.right(false)(true) == false);
            expect(logical_and.right(false)(false) == false);
        };
        should("callable with a pair or tuple argument") = [] {
            expect(logical_and.tuple(std::pair{true, true}) == true);
            expect(logical_and.tuple(std::pair{true, false}) == false);
            expect(logical_and.tuple(std::pair{false, true}) == false);
            expect(logical_and.tuple(std::pair{false, false}) == false);

            expect(logical_and.tuple(std::tuple{true, true}) == true);
            expect(logical_and.tuple(std::tuple{true, false}) == false);
            expect(logical_and.tuple(std::tuple{false, true}) == false);
            expect(logical_and.tuple(std::tuple{false, false}) == false);
        };
    };
    "logical_or_fn"_test = [&] {
        constexpr auto re = std::array{true, true, true, false};
        static_assert((a1 or a2) == re);

        using callables::logical_or;
        should("immediately evaluable") = [&]{
            expect(logical_or(true, true) == true);
            expect(logical_or(true, false) == true);
            expect(logical_or(false, true) == true);
            expect(logical_or(false, false) == false);

            expect(logical_or(a1, a2) == re);
        };
        should("partially-applicable") = [&]{
            expect(logical_or(true)(true) == true);
            expect(logical_or(true)(false) == true);
            expect(logical_or(false)(true) == true);
            expect(logical_or(false)(false) == false);

            expect(logical_or(a1)(a2) == re);
        };
        should("left-bindable") = []{
            expect(logical_or.left(true)(true) == true);
            expect(logical_or.left(true)(false) == true);
            expect(logical_or.left(false)(true) == true);
            expect(logical_or.left(false)(false) == false);
        };
        should("right-bindable") = []{
            expect(logical_or.right(true)(true) == true);
            expect(logical_or.right(true)(false) == true);
            expect(logical_or.right(false)(true) == true);
            expect(logical_or.right(false)(false) == false);
        };
        should("callable with a pair or tuple argument") = [] {
            expect(logical_or.tuple(std::pair{true, true}) == true);
            expect(logical_or.tuple(std::pair{true, false}) == true);
            expect(logical_or.tuple(std::pair{false, true}) == true);
            expect(logical_or.tuple(std::pair{false, false}) == false);

            expect(logical_or.tuple(std::tuple{true, true}) == true);
            expect(logical_or.tuple(std::tuple{true, false}) == true);
            expect(logical_or.tuple(std::tuple{false, true}) == true);
            expect(logical_or.tuple(std::tuple{false, false}) == false);
        };
    };
    "logical_xor_fn"_test = [&] {
        constexpr auto re = std::array{false, true, true, false};
        static_assert((a1 xor a2) == re);

        using callables::logical_xor;
        should("immediately evaluable") = [&]{
            expect(logical_xor(true, true) == false);
            expect(logical_xor(true, false) == true);
            expect(logical_xor(false, true) == true);
            expect(logical_xor(false, false) == false);

            expect(logical_xor(a1, a2) == re);
        };
        should("partially-applicable") = [&]{
            expect(logical_xor(true)(true) == false);
            expect(logical_xor(true)(false) == true);
            expect(logical_xor(false)(true) == true);
            expect(logical_xor(false)(false) == false);

            expect(logical_xor(a1)(a2) == re);
        };
        should("left-bindable") = []{
            expect(logical_xor.left(true)(true) == false);
            expect(logical_xor.left(true)(false) == true);
            expect(logical_xor.left(false)(true) == true);
            expect(logical_xor.left(false)(false) == false);
        };
        should("right-bindable") = []{
            expect(logical_xor.right(true)(true) == false);
            expect(logical_xor.right(true)(false) == true);
            expect(logical_xor.right(false)(true) == true);
            expect(logical_xor.right(false)(false) == false);
        };
        should("callable with a pair or tuple argument") = [] {
            expect(logical_xor.tuple(std::pair{true, true}) == false);
            expect(logical_xor.tuple(std::pair{true, false}) == true);
            expect(logical_xor.tuple(std::pair{false, true}) == true);
            expect(logical_xor.tuple(std::pair{false, false}) == false);

            expect(logical_xor.tuple(std::tuple{true, true}) == false);
            expect(logical_xor.tuple(std::tuple{true, false}) == true);
            expect(logical_xor.tuple(std::tuple{false, true}) == true);
            expect(logical_xor.tuple(std::tuple{false, false}) == false);
        };
    };
    "logical_not_fn"_test = []{
        using callables::logical_not;
        should("immediately evaluable") = []{
            expect(logical_not(true) == false);
            expect(logical_not(false) == true);

            expect(logical_not(std::array{true, false}) == std::array{false, true});
        };
    };
}
