/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : nested_invoke
 * @created     : Tuesday Aug 22, 2023 14:27:37 CEST
 * @description : 
 * */

#ifndef BRUN_CALLABLES_DETAIL_FUNCTIONAL_HPP
#define BRUN_CALLABLES_DETAIL_FUNCTIONAL_HPP

#include <concepts>

namespace brun::detail
{

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

template <typename Fn, typename ...Fns>
struct nested_invoke_result
{
private:
    template <typename ...Args>
    static auto _with_impl(type_list<Args...>) {
        if constexpr (sizeof...(Fns) == 0) {
            return type_list<std::invoke_result_t<Fn, Args...>>{};
        } else {
            using nested_fs = nested_invoke_result<Fns...>;
            using result = nested_fs::template with<Args...>;
            return type_list<std::invoke_result_t<Fn, result>>{};
        }
    }
public:
    template <typename ...Args>
    using with = decltype(_with_impl(type_list<Args...>{}))::type;
};

} // namespace brun::detail

#endif /* BRUN_CALLABLES_DETAIL_FUNCTIONAL_HPP */
