/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @created     : Tuesday Aug 22, 2023 14:27:37 CEST
 * @description :
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

#ifndef CB_DETAIL_FUNCTIONAL_HPP
#define CB_DETAIL_FUNCTIONAL_HPP

#include <concepts>

namespace callables::detail
{

#define CB_FWD(x) static_cast<decltype(x) &&>(x)

template <typename T1, typename ...Pack1>
struct pairwise_constructible
{
    template <typename T2, typename ...Pack2>
        requires (sizeof...(Pack2) == sizeof...(Pack1))
    constexpr static inline bool from = std::constructible_from<T1, T2> and
                                        pairwise_constructible<Pack1...>::template from<Pack2...>;
};

template <typename T1>
struct pairwise_constructible<T1>
{
    template <typename T2>
    constexpr static inline bool from = std::constructible_from<T1, T2>;
};

static_assert(pairwise_constructible<int, int>::from<int, int>);

template <typename ...Args> struct type_list    {};
template <typename T>       struct type_list<T> { using type = T;    };
template <>                 struct type_list< > { using type = void; };

template <typename T, typename Fn>
concept has_member_apply_with = requires(T && t, Fn && fn) {
    { CB_FWD(t).apply(CB_FWD(fn)) };
};

template <typename Fn, typename Tuple>
concept direct_applicable = requires(Fn && fn, Tuple && args) {
    { apply(CB_FWD(fn), CB_FWD(args)) };
};

template <typename Fn, typename Tuple>
concept applicable = direct_applicable<Fn, Tuple> or has_member_apply_with<Tuple, Fn>;


template <typename T>
concept character = std::same_as<char, T>
                 or std::same_as<unsigned char, T> or std::same_as<signed char, T>
                 or std::same_as<wchar_t, T> or std::same_as<char8_t, T>;

template <typename T>
concept numeric = std::integral<T> and not std::same_as<bool, T> and not character<T>;

#undef CB_FWD

} // namespace callables::detail

#endif /* CB_DETAIL_FUNCTIONAL_HPP */
