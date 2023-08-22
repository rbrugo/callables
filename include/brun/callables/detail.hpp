/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : detail
 * @created     : Monday Dec 13, 2021 13:20:27 CET
 * @description : 
 * */

#ifndef BRUN_CALLABLES_DETAIL_HPP
#define BRUN_CALLABLES_DETAIL_HPP

#include <tuple>

namespace brun
{

namespace detail
{
template <typename>
struct is_tuple_specialization_impl : std::false_type {};
template <typename ...Args>
struct is_tuple_specialization_impl<std::tuple<Args...>> : std::true_type {};
template <typename T>
constexpr inline auto is_tuple_specialization = is_tuple_specialization_impl<std::remove_cvref_t<T>>::value;

template <typename>
struct is_pair_specialization_impl : std::false_type {};
template <typename ...Args>
struct is_pair_specialization_impl<std::pair<Args...>> : std::true_type {};
template <typename T>
constexpr inline auto is_pair_specialization = is_pair_specialization_impl<std::remove_cvref_t<T>>::value;

template <typename T>
constexpr inline auto tuple_size = std::tuple_size_v<std::remove_cvref_t<T>>;

template <typename Fn, typename Tuple>
concept applicable = requires(Fn && fn, Tuple && args) {
    { std::apply(std::forward<Fn>(fn), std::forward<Tuple>(args)) };
};


// template <typename Tuple1, typename Tuple2>
// using combined_tuple_t = std::invoke_result_t<std::tuple_cat, Tuple1, Tuple2>;

struct _combined_tuple
{
    template <typename ...Ts, typename ...Us>
    static auto _impl(std::tuple<Ts...>, std::tuple<Us...>) -> std::tuple<Ts..., Us...>;
    template <typename ...Ts, typename U, typename V>
    static auto _impl(std::tuple<Ts...>, std::pair<U, V>) -> std::tuple<Ts..., U, V>;
    template <typename U, typename V, typename ...Ts>
    static auto _impl(std::pair<U, V>, std::tuple<Ts...>) -> std::tuple<U, V, Ts...>;
    template <typename S, typename T, typename U, typename V>
    static auto _impl(std::pair<S, T>, std::pair<U, V>) -> std::tuple<S, T, U, V>;

    template <typename Tuple1, typename Tuple2, typename ...Tuples>
    static auto _impl(Tuple1 && t1, Tuple2 && t2, Tuples &&... ts) {
        return _impl(
            _impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2)),
            std::forward<Tuples>(ts)...
        );
    }

    template <typename ...Ts>
    using type = decltype(_impl(std::declval<Ts>()...));
};

template <typename ...Ts>
using combined_tuple_t = _combined_tuple::type<Ts...>;

static_assert(std::same_as<combined_tuple_t<std::tuple<int, float>, std::tuple<int>>, std::tuple<int, float, int>>);
static_assert(std::same_as<combined_tuple_t<std::tuple<int>, std::pair<int, double>>, std::tuple<int, int, double>>);

template <typename Fn, typename T, typename U>
concept can_select_integral_overload = std::regular_invocable<Fn, T, U>// and Left
                 and std::integral<std::remove_cvref_t<T>>
                 and std::integral<std::remove_cvref_t<U>>
                 and (not std::same_as<std::remove_cvref_t<T>, char>)
                 and (not std::same_as<std::remove_cvref_t<U>, char>);

} // namespace detail

} // namespace brun

#endif /* BRUN_CALLABLES_DETAIL_HPP */
