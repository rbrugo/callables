/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : bit_operators
 * @created     : Monday Aug 21, 2023 17:02:15 CEST
 * @description : 
 */

#include <brun/callables/bit_operators.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

using namespace std::literals;

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::operators::terse;
    auto const input = std::to_array<std::pair<int, int>>({
        std::pair{0b000, 0b001},
        {0b111, 0b101},
        {0b11011, 0b10101},
        {123, -45}
    });

    "bit_and_fn"_test = [&input] {
        using brun::bit_and;
        should("immediately evaluable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_and(a, b) == _i(a & b));
            }
        };
        should("partial-applicable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_and(a)(b) == _i(a & b));
            }
        };
         should("left-bindable") = [&] {
             for (auto [a, b] : input) {
                 expect(bit_and.left(a)(b) == _i(a & b));
             }
         };
         should("right-bindable") = [&] {
             for (auto [a, b] : input) {
                 expect(bit_and.right(a)(b) == _i(a & b));
             }
         };
         should("callable with a tuple argument") = [&] {
             for (auto p : input) {
                 expect(bit_and(p) == _i(p.first & p.second));
             }
         };
    };

    "bit_or_fn"_test = [&input] {
        using brun::bit_or;
        should("immediately evaluable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_or(a, b) == _i(a | b));
            }
        };
        should("partial-applicable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_or(a)(b) == _i(a | b));
            }
        };
         should("left-bindable") = [&] {
             for (auto [a, b] : input) {
                 expect(bit_or.left(a)(b) == _i(a | b));
             }
         };
         should("right-bindable") = [&] {
             for (auto [a, b] : input) {
                 expect(bit_or.right(a)(b) == _i(a | b));
             }
         };
         should("callable with a tuple argument") = [&] {
             for (auto p : input) {
                 expect(bit_or(p) == _i(p.first | p.second));
             }
         };
    };

    "bit_xor_fn"_test = [&input] {
        using brun::bit_xor;
        should("immediately evaluable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_xor(a, b) == _i(a ^ b));
            }
        };
        should("partial-applicable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_xor(a)(b) == _i(a ^ b));
            }
        };
         should("left-bindable") = [&] {
             for (auto [a, b] : input) {
                 expect(bit_xor.left(a)(b) == _i(a ^ b));
             }
         };
         should("right-bindable") = [&] {
             for (auto [a, b] : input) {
                 expect(bit_xor.right(a)(b) == _i(a ^ b));
             }
         };
         should("callable with a tuple argument") = [&] {
             for (auto p : input) {
                 expect(bit_xor(p) == _i(p.first ^ p.second));
             }
         };
    };

    "bit_not_fn"_test = [&input] {
        using brun::bit_not;
        should("immediately evaluable") = [&] {
            for (auto [a, b] : input) {
                expect(bit_not(a) == _i(~a));
                expect(bit_not(b) == _i(~b));
            }
        };
    };
};
