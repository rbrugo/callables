/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Tuesday Feb 11, 2025 19:22:56 CET
 * @description : utilities to convert stuff to string
 * @license     :
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * */

#ifndef CB_FORMAT_HPP
#define CB_FORMAT_HPP

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907

#include <format>

namespace callables
{

// to_string
// format

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// .................................TO_STRING.................................. //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
struct to_string_t
{
    template <typename T>
    static constexpr auto operator()(T const & t) -> std::string
    { return std::format("{}", t); }

};

constexpr inline to_string_t to_string;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................FORMAT................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
template <typename T, std::size_t Size>
struct fixed_string
{
    using size_type = decltype(Size);

    constexpr fixed_string(T const * str) {
        for (size_type  i = {}; i < Size; ++i) { data[i] = str[i]; }
        data[Size] = T();
    }
    [[nodiscard]] constexpr auto operator<=>(const fixed_string&) const = default;
    [[nodiscard]] constexpr operator std::string_view() const { return {std::data(data), Size}; }
    [[nodiscard]] constexpr auto size() const { return Size; }

    T data[Size + 1u];
};
template<class T, std::size_t Capacity, std::size_t Size = Capacity - 1>
fixed_string(const T (&str)[Capacity]) -> fixed_string<T, Size>;

template <fixed_string Fmt>
struct format_t
{
    template <typename T>
    static constexpr auto operator()(T && t) {
        return std::format(Fmt, std::forward<T>(t));
    }
};

template <fixed_string Fmt>
constexpr inline auto format = format_t<Fmt>{};

} // namespace callables

#endif /* defined(_cpp_lib_format) */
#endif /* CB_FORMAT_HPP */
