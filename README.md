# callables
A set of composable and partially-applicable function objects to simplify the use of algorithms and views

## Example of usage:
```cpp
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
std::ranges::sort(boxes, cb::less_equal);

auto volume(box const & b) { ... };

auto heavy = boxes | svw::filter(cb::greater_equal(10), &box::weight);
auto my_boxes = heavy | svw::filter(heavy, cb::equals("Joe"), &box::label);
auto take_stuff = my_boxes | svw::transform(cb::minus(10), &box::weight);
auto store_all = take_stuff | svw::transform(cb::construct<store>);
auto total_weight = my_boxes | std::views::transform(&box::weight) | fold(plus);
auto biggest = std::ranges::max(boxes, cb::on(volume, cb::less_equal));
auto manhattan_distance = cb::on(cb::abs, cb::plus).tuple;
auto nearest = std::ranges::min(boxes, manhattan_distance);
```

## Function objects
***Bit operators:***
- `bit_and`
- `bit_or`
- `bit_xor`
- `bit_not`

***Functional:***
- `apply`
- `compose`
- `on`: applies a binary function over a unary function
- `identity`
- `addressof`
- `dereference`
- `construct<T>`, `construct<T>.from_tuple`
- `get<N>`
- `at(N)`, `at[N]`
- `value_or`
- `from_container(cont)`
- `transform_at<N>`: applies the captured function to the nth element of the tuple

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

***Formatting***
- `to_string`
- `format<fmt>`, templated with a format string

***Range actions***
- `fold`


All bit, arithmetic, equality and ordering operators have a member `.tuple` that accepts a tuple-like
object and computes the operation on its members. For example:
`static_assert(plus.tuple(std::pair{2, 3}) == 5)`

TODO: switch to C++23, replace all CRTPs with `deducing this`
