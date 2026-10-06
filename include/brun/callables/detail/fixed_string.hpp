/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @description : a string usable as a template argument (`format<"...">`, `member<"...">`)
 * @license     : Boost Software License - Version 1.0
 * */

#ifndef CB_DETAIL_FIXED_STRING_HPP
#define CB_DETAIL_FIXED_STRING_HPP

#include <cstddef>
#include <string_view>

namespace callables
{

template <typename T, std::size_t Size>
struct fixed_string
{
    using size_type = decltype(Size);

    constexpr fixed_string(T const * str) {
        for (size_type  i = {}; i < Size; ++i) { data[i] = str[i]; }
        data[Size] = T();
    }
    [[nodiscard]] constexpr auto operator<=>(const fixed_string&) const = default;
    [[nodiscard]] constexpr operator std::string_view() const { return {data, Size}; }
    [[nodiscard]] constexpr auto size() const { return Size; }

    T data[Size + 1u];
};

template<class T, std::size_t Capacity, std::size_t Size = Capacity - 1>
fixed_string(const T (&str)[Capacity]) -> fixed_string<T, Size>;

}  // namespace callables

#endif /* CB_DETAIL_FIXED_STRING_HPP */
