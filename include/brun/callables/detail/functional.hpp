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

#define FWD(x) static_cast<decltype(x) &&>(x)

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
    { FWD(t).apply(FWD(fn)) };
};

template <typename Fn, typename Tuple>
concept direct_applicable = requires(Fn && fn, Tuple && args) {
    { apply(FWD(fn), FWD(args)) };
};

template <typename Fn, typename Tuple>
concept applicable = direct_applicable<Fn, Tuple> or has_member_apply_with<Tuple, Fn>;

#undef FWD

} // namespace brun::detail

#endif /* BRUN_CALLABLES_DETAIL_FUNCTIONAL_HPP */
