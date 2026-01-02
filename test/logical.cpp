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

    "and_fn"_test = [&] {
        constexpr auto re = std::array{true, false, false, false};
        static_assert((a1 and a2) == re);

        using callables::and_;
        should("immediately evaluable") = [&]{
            expect(and_(true, true) == true);
            expect(and_(true, false) == false);
            expect(and_(false, true) == false);
            expect(and_(false, false) == false);

            expect(and_(a1, a2) == re);
        };
        should("partially-applicable") = [&]{
            expect(and_(true)(true) == true);
            expect(and_(true)(false) == false);
            expect(and_(false)(true) == false);
            expect(and_(false)(false) == false);

            expect(and_(a1)(a2) == re);
        };
        should("left-bindable") = []{
            expect(and_.left(true)(true) == true);
            expect(and_.left(true)(false) == false);
            expect(and_.left(false)(true) == false);
            expect(and_.left(false)(false) == false);
        };
        should("right-bindable") = []{
            expect(and_.right(true)(true) == true);
            expect(and_.right(true)(false) == false);
            expect(and_.right(false)(true) == false);
            expect(and_.right(false)(false) == false);
        };
        should("callable with a pair or tuple argument") = [] {
            expect(and_.tuple(std::pair{true, true}) == true);
            expect(and_.tuple(std::pair{true, false}) == false);
            expect(and_.tuple(std::pair{false, true}) == false);
            expect(and_.tuple(std::pair{false, false}) == false);

            expect(and_.tuple(std::tuple{true, true}) == true);
            expect(and_.tuple(std::tuple{true, false}) == false);
            expect(and_.tuple(std::tuple{false, true}) == false);
            expect(and_.tuple(std::tuple{false, false}) == false);
        };
    };
    "or_fn"_test = [&] {
        constexpr auto re = std::array{true, true, true, false};
        static_assert((a1 or a2) == re);

        using callables::or_;
        should("immediately evaluable") = [&]{
            expect(or_(true, true) == true);
            expect(or_(true, false) == true);
            expect(or_(false, true) == true);
            expect(or_(false, false) == false);

            expect(or_(a1, a2) == re);
        };
        should("partially-applicable") = [&]{
            expect(or_(true)(true) == true);
            expect(or_(true)(false) == true);
            expect(or_(false)(true) == true);
            expect(or_(false)(false) == false);

            expect(or_(a1)(a2) == re);
        };
        should("left-bindable") = []{
            expect(or_.left(true)(true) == true);
            expect(or_.left(true)(false) == true);
            expect(or_.left(false)(true) == true);
            expect(or_.left(false)(false) == false);
        };
        should("right-bindable") = []{
            expect(or_.right(true)(true) == true);
            expect(or_.right(true)(false) == true);
            expect(or_.right(false)(true) == true);
            expect(or_.right(false)(false) == false);
        };
        should("callable with a pair or tuple argument") = [] {
            expect(or_.tuple(std::pair{true, true}) == true);
            expect(or_.tuple(std::pair{true, false}) == true);
            expect(or_.tuple(std::pair{false, true}) == true);
            expect(or_.tuple(std::pair{false, false}) == false);

            expect(or_.tuple(std::tuple{true, true}) == true);
            expect(or_.tuple(std::tuple{true, false}) == true);
            expect(or_.tuple(std::tuple{false, true}) == true);
            expect(or_.tuple(std::tuple{false, false}) == false);
        };
    };
    "xor_fn"_test = [&] {
        constexpr auto re = std::array{false, true, true, false};
        static_assert((a1 xor a2) == re);

        using callables::xor_;
        should("immediately evaluable") = [&]{
            expect(xor_(true, true) == false);
            expect(xor_(true, false) == true);
            expect(xor_(false, true) == true);
            expect(xor_(false, false) == false);

            expect(xor_(a1, a2) == re);
        };
        should("partially-applicable") = [&]{
            expect(xor_(true)(true) == false);
            expect(xor_(true)(false) == true);
            expect(xor_(false)(true) == true);
            expect(xor_(false)(false) == false);

            expect(xor_(a1)(a2) == re);
        };
        should("left-bindable") = []{
            expect(xor_.left(true)(true) == false);
            expect(xor_.left(true)(false) == true);
            expect(xor_.left(false)(true) == true);
            expect(xor_.left(false)(false) == false);
        };
        should("right-bindable") = []{
            expect(xor_.right(true)(true) == false);
            expect(xor_.right(true)(false) == true);
            expect(xor_.right(false)(true) == true);
            expect(xor_.right(false)(false) == false);
        };
        should("callable with a pair or tuple argument") = [] {
            expect(xor_.tuple(std::pair{true, true}) == false);
            expect(xor_.tuple(std::pair{true, false}) == true);
            expect(xor_.tuple(std::pair{false, true}) == true);
            expect(xor_.tuple(std::pair{false, false}) == false);

            expect(xor_.tuple(std::tuple{true, true}) == false);
            expect(xor_.tuple(std::tuple{true, false}) == true);
            expect(xor_.tuple(std::tuple{false, true}) == true);
            expect(xor_.tuple(std::tuple{false, false}) == false);
        };
    };
    "not_fn"_test = []{
        using callables::not_;
        should("immediately evaluable") = []{
            expect(not_(true) == false);
            expect(not_(false) == true);

            expect(not std::array{true, false} == std::array{false, true});
        };
    };
}
