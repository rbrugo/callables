# callables
A set of composable and partially-applicable function objects to simplify the use of algorithms and views

## Example of usage:
```cpp
#include <brun/callables.hpp>
#include <brun/callables/actions.hpp>  // sort, fold, sum
#include <brun/callables/math.hpp>     // abs

namespace cb = callables;
namespace svw = std::views;
namespace srg = std::ranges;

struct box
{
    std::string label;
    float weight;
    std::array<int, 3> size;
    std::array<float, 2> coordinates;
};

struct store
{
    store(auto) {}
};

auto boxes = std::vector<box>{...};
srg::sort(boxes, cb::less_than, &box::weight);

auto volume(box const & b) { ... }

auto heavy = boxes | svw::filter(cb::compose(cb::greater_equal(10), &box::weight));
auto my_boxes = heavy | svw::filter(cb::compose(cb::equal_to("Joe"), cb::member<"label">));  // needs reflection
auto take_stuff = my_boxes | svw::transform(cb::compose(cb::minus.right(10), &box::weight));
auto store_all = take_stuff | svw::transform(cb::construct<store>);
auto total_weight = my_boxes | svw::transform(&box::weight) | cb::sum(0.f);
auto biggest = srg::max(boxes, cb::on(volume, cb::less_than));
auto manhattan_distance = cb::on(cb::abs, cb::plus).tuple;
auto nearest = srg::min(boxes, cb::less_than, cb::compose(manhattan_distance, &box::coordinates));
```

## Function objects
***Bit operators:***
- `bit_and`
- `bit_or`
- `bit_xor`
- `bit_not`

***Functions:***
- `apply`
- unary `operator+` (in `callables::operators`): `+fn` is `apply(fn)`
- `compose`
- binary `operator*` (in `callables::operators`): `f * g` is `compose(f, g)`, so `(f * g)(x...) == f(g(x...))`
- `on`: applies a binary function over a unary function
- `flip`: applies arguments in reversed order
- `curry`: make a _Callable_ curriable once for any number of arguments
- `identity`
- `decay_copy`
- `addressof`
- `dereference`
- `not_`
- `construct<T>`, `construct<T>.from_tuple`
- `cast<T>`: perform a static cast
- `get<N>`
- `member<"name">`: extracts a public data member by name, `member<"a", "b">` a tuple of them (requires reflection)
- `at(N)`, `at[N]`
- `front`: the first element of a range
- `value_or`
- `from_container(cont, N)`
- `transform_at<N>`: applies the captured function to the nth element of the tuple
- `if_then_else`
- `graph(fn, x...)`: returns `tuple{x..., fn(x...)}`

Wherever these take a function (`compose`, `on`, `flip`, `curry`, `apply`, `graph`, `operator*`),
a plain function or a pointer to member works too, e.g. `compose(twice, &point::x)`.

***Equality and ordering:***
- `equal_to`
- `not_equal_to`
- `less_than`
- `less_equal`
- `greater_than`
- `greater_equal`

***Arithmetic:***
- `plus`
- `minus`
- `multiplies`
- `divides`
- `negate`

***Math:***
- `abs`
- `between`

***Logical:***
- `logical_and`
- `logical_or`
- `logical_xor`
- `logical_not`

***Formatting***
- `to_string`
- `format<fmt>`, templated with a format string
- `ston<T, B, P>` string-to-number, templated with number type,  base (`10`) and result policy (default: `use_exception`)

***Range actions***
- `fold` (without projection support)
- `sum` = `fold(plus)`: `range | sum` yields an optional (empty for an empty range), `range | sum(0.)` a value;
  `sum.from(init)` takes an initial value that is itself a range. Any `fold(fn)` can be seeded the same way
- `sort`, also with a comparator and a projection (`range | sort(less_than, &item::weight)`): an lvalue
  range is sorted in place and returned by reference, an rvalue range is returned by value

***Result Policies***
As for now, only `ston` uses result policies.
- `policy::use_exception`: result will be returned as it is; in case of failure, an exception will be thrown
- `policy::use_pair_with_errc`: result will be returned in a `pair<T, std::errc>`
- `policy::use_optional`: result will be returned in an `optional<T>`, that will be empty in case of failure
- `policy::use_expected`: result or error will be returned in an `expected<T, std::errc>`


All bit, arithmetic, equality and ordering operators have a member `.tuple` that accepts a tuple-like
object and computes the operation on its members. For example:
`static_assert(plus.tuple(std::pair{2, 3}) == 5)`

TODO: switch to C++23, replace all CRTPs with `deducing this`
