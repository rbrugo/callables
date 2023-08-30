# callables
A set of composable and partially-applicable function objects to simplify the use of algorithms and views

## Example of usage:
```cpp
namespace svw = std::views;
namespace srg = std::ranges;

struct box
{
    std::string label;
    float weight;
};

struct store
{
    store(auto) {}
};

auto boxes = std::vector<box>{...};
std::ranges::sort(boxes, brun::less_equal);

auto heavy = boxes | svw::filter(boxes, brun::greater_equal(10), &box::weight);
auto my_boxes = heavy | svw::filter(heavy, brun::equals("Joe"), &box::label);
auto take_stuff = my_boxes | svw::transform(brun::minus(10), &box::weight);
auto store_all = take_stuff | svw::transform(brun::construct<store>);
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
- `identity`
- `construct<T>`, `construct<T>.from_tuple`
- `get<N>`

***Equality and ordering:***
- `equal_to`
- `not_equal_to`
- `less`
- `less_equal`
- `greater`
- `greater_equal`

***Arithmetic:***
- `plus`
- `minus`
- `multiplies`
- `divides`
- `negate`
