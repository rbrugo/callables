#include <array>
#include <string>
#include <vector>
#include <optional>
#include <type_traits>
#include <brun/callables/actions.hpp>
#include <brun/callables/ordering.hpp>
#include <brun/callables/arithmetic.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

using namespace std::literals;

namespace
{

struct item
{
    std::string name;
    int weight;
};

// a catch-all `operator|`
namespace greedy
{
struct hijacked {};
template <typename F, template <typename...> class T, typename... Ts>
constexpr auto operator|(F const &, T<Ts...> const &) { return hijacked{}; }
}  // namespace greedy

}  // namespace

int main()
{
    // not `using namespace boost::ut`, as it brings in ut's own catch-all `operator|`,
    using boost::ut::expect, boost::ut::should, boost::ut::operator==;
    using boost::ut::operator""_test, boost::ut::operator""_i, boost::ut::operator""_d;

    "fold_fn"_test = [] {
        using callables::fold, callables::plus, callables::multiplies;
        should("fold a range from its first element") = [] {
            expect(fold(std::vector{1, 2, 3}, plus) == std::optional{6});
            expect(fold(std::vector<int>{}, plus) == std::nullopt);
        };
        should("fold a range from an initial value") = [] {
            expect(fold(std::vector{1, 2, 3}, 10, plus) == 16_i);
            expect(fold(std::vector{"a"s, "b"s}, "> "s, plus) == "> ab"s);
        };
        should("leave an lvalue initial value untouched") = [] {
            auto const strs = std::array{"a"s, "b"s};
            auto seed = "> "s;
            expect(fold(strs, seed, plus) == "> ab"s);
            expect(seed == "> "s);
        };
        should("fold an iterator pair") = [] {
            auto const v = std::array{1, 2, 3};
            expect(fold(v.begin(), v.end(), plus) == std::optional{6});
            expect(fold(v.begin(), v.end(), 1, multiplies) == 6_i);
        };
        should("be pipeable") = [] {
            expect((std::vector{1, 2, 3} | fold(plus)) == std::optional{6});
            expect((std::vector{1, 2, 3} | fold(plus, 10)) == 16_i);
        };
        should("follow the result type of std::ranges::fold_left") = [] {
            expect((std::vector{1, 2, 3} | fold(plus, 0.5)) == 6.5_d);
            expect((std::vector{0.5, 0.5} | fold(plus, 0)) == 1._d);  // no truncation to int
        };
        should("be seedable once partially applied") = [] {
            expect((std::vector{1, 2, 3} | fold(plus)(10)) == 16_i);
            expect((std::vector{1, 2, 3} | fold(plus).from(10)) == 16_i);
            auto const offset = fold([k = 10](int a, int b) { return a + b + k; });  // stateful
            expect((std::vector{1, 2, 3} | offset) == std::optional{26});
            expect((std::vector{1, 2, 3} | offset(100)) == 136_i);
        };
        should("be evaluable at compile time") = [] {
            static_assert(*(std::array{1, 2, 3} | fold(plus)) == 6);
            static_assert((std::array{1, 2, 3} | fold(multiplies, 1)) == 6);
        };
    };

    "sum"_test = [] {
        using callables::sum;
        should("yield an optional, empty on an empty range") = [] {
            expect((std::vector{1, 2, 3} | sum) == std::optional{6});
            expect((std::vector<int>{} | sum) == std::nullopt);
        };
        should("yield a value when seeded") = [] {
            expect((std::vector{1, 2, 3} | sum(0.)) == 6._d);
            expect((std::vector{1, 2, 3} | sum.from(10)) == 16_i);
        };
        should("take range-like seeds only through `from`") = [] {
            expect((std::vector{"a"s, "b"s} | sum.from("> "s)) == "> ab"s);
            expect(sum(std::vector{1, 2, 3}) == std::optional{6});  // a range is folded, not a seed
        };
        should("be evaluable at compile time") = [] {
            static_assert(*(std::array{1, 2, 3} | sum) == 6);
            static_assert((std::array{1, 2, 3} | sum(10)) == 16);
        };
    };

    "operator|"_test = [] {
        using callables::sum, callables::fold, callables::plus;
        should("accept const ranges") = [] {
            auto const v = std::vector{1, 2, 3};
            expect((v | sum) == std::optional{6});
            expect((v | fold(plus, 10)) == 16_i);
        };
        should("win over a catch-all operator| in scope") = [] {
            using namespace greedy;
            auto v = std::vector{1, 2, 3};
            auto const cv = std::vector{1, 2, 3};
            static_assert(std::is_same_v<decltype(std::vector{1, 2, 3} | sum), std::optional<int>>);
            static_assert(std::is_same_v<decltype(v | sum), std::optional<int>>);
            static_assert(std::is_same_v<decltype(v | fold(plus)), std::optional<int>>);
            static_assert(std::is_same_v<decltype(cv | fold(plus)), std::optional<int>>);
            static_assert(std::is_same_v<decltype(cv | sum(10)), int>);
            // `cv | sum` is the one case it can't win: both operands bind identically,
            // and the catch-all's `T<Ts...>` pattern is more specialized
        };
    };

    "sort_fn"_test = [] {
        using callables::sort, callables::greater_than, callables::less_than;
        should("sort a range in place and return it") = [] {
            auto v = std::vector{3, 1, 2};
            auto & r = sort(v);
            expect(v == std::vector{1, 2, 3});
            expect(&r == &v);
            static_assert(std::is_same_v<decltype(v | sort()), std::vector<int> &>);
        };
        should("return a sorted temporary by value") = [] {
            static_assert(std::is_same_v<decltype(sort(std::vector{3, 1, 2})), std::vector<int>>);
            static_assert(std::is_same_v<decltype(std::vector{3, 1, 2} | sort()), std::vector<int>>);
            auto const v = sort(std::vector{3, 1, 2});
            expect(v == std::vector{1, 2, 3});
            auto const w = std::vector{3, 1, 2} | sort(greater_than);
            expect(w == std::vector{3, 2, 1});
        };
        should("accept a comparator and a projection") = [] {
            auto v = std::vector{1, 3, 2};
            sort(v, greater_than);
            expect(v == std::vector{3, 2, 1});

            auto items = std::vector<item>{{"b", 2}, {"c", 3}, {"a", 1}};
            sort(items, less_than, &item::weight);
            expect(items[0].name == "a"s);
            expect(items[1].name == "b"s);
            expect(items[2].name == "c"s);
        };
        should("be pipeable") = [] {
            auto v = std::vector{3, 1, 2};
            v | sort();
            expect(v == std::vector{1, 2, 3});
            v | sort(greater_than);
            expect(v == std::vector{3, 2, 1});

            auto items = std::vector<item>{{"b", 2}, {"c", 3}, {"a", 1}};
            items | sort(less_than, &item::weight);
            expect(items[0].name == "a"s);
            expect(items[2].name == "c"s);
            auto const sorted = std::vector<item>{{"b", 2}, {"c", 3}, {"a", 1}} | sort(greater_than, &item::weight);
            expect(sorted[0].name == "c"s);
            expect(sorted[2].name == "a"s);
        };
        should("be evaluable at compile time") = [] {
            static_assert(sort(std::array{3, 2, 1}) == std::array{1, 2, 3});
        };
    };
}
