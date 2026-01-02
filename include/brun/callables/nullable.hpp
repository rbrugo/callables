/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Thursday Dec 04, 2025 23:38:08 CET
 * @description : 
 * */

#ifndef CB_NULLABLE_HPP
#define CB_NULLABLE_HPP

#include <optional>

#include "detail/_config_begin.hpp"
namespace callables
{
// constexpr inline struct null_t {} null;

// Mechanism 1: nullable_traits
template <typename T>
struct nullable_traits
{
    using value_type = typename T::value_type;
    /**
     * @brief check whether the parameter is null
     *
     * @param obj the object to be checked
     * @return whether it is null
     */
    static constexpr bool is_null(T const & obj);  // { return obj == null_value; }

    /**
     * @brief returns the content of the passed parameter, assuming it is not null
     *
     * @param obj the object whose value must be returned
     * @return the object's content
     */
    static constexpr auto get_value(T const & obj) -> value_type const &;  // { return *obj; }
    static constexpr auto get_value(T & obj) -> value_type &;  // { return *obj; }
};

template <typename T> struct nullable_traits<T const> : public nullable_traits<T> {};
template <typename T> struct nullable_traits<T &> : public nullable_traits<T> {};
template <typename T> struct nullable_traits<T &&> : public nullable_traits<T> {};

// Mechanism 2: is_null for always null types
template <typename T> static constexpr inline auto is_null_type = false;
template <typename T> static constexpr inline auto is_null_type<T const> = is_null_type<T>;
template <typename T> static constexpr inline auto is_null_type<T &> = is_null_type<T>;
template <typename T> static constexpr inline auto is_null_type<T &&> = is_null_type<T>;

// template <> constexpr inline auto is_null_type<null_t> = true;
template <> constexpr inline auto is_null_type<std::nullptr_t> = true;
template <> constexpr inline auto is_null_type<std::nullopt_t> = true;

template <typename T> requires is_null_type<T>
struct nullable_traits<T>
{
    using value_type = std::remove_cvref_t<T>;
    static constexpr bool is_null(T const &) noexcept { return true; }
    static constexpr auto get_value(T const & obj) noexcept -> decltype(auto) { return obj; }
};


// Mechanism 3: has is_null free function or member function
// TODO: should I also implement a getter? No point in implementing `is_null` without getter
// template <typename T>
// constexpr inline auto is_null_free_function = requires(T const & t) { { is_null(t) } -> std::same_as<bool>; };
//
// template <typename T>
// constexpr inline auto is_null_member_function = requires(T const & t) { { t.is_null() } -> std::same_as<bool>; };
//
// template <typename T> requires is_null_free_function<T>
// struct nullable_traits<T>
// {
//     using value_type = T;
//     static constexpr bool is_null(T const & obj) noexcept(noexcept(is_null(obj))) { return is_null(obj); }
// };
//
// template <typename T> requires is_null_member_function<T>
// struct nullable_traits<T>
// {
//     using value_type = T;
//     static constexpr bool is_null(T const & obj) noexcept(noexcept(obj.is_null())) { return obj.is_null(); }
// };

// Implementations
template <typename T>
struct nullable_traits<T *>
{
    using value_type = T;
    static constexpr bool is_null(T * const ptr) noexcept { return ptr == nullptr; }
    static constexpr auto get_value(T * const ptr) noexcept { return *ptr; }
};

template <typename T>
struct nullable_traits<std::optional<T>>
{
    using value_type = std::optional<T>::value_type;
    static constexpr bool is_null(std::optional<T> const & obj) noexcept { return obj == std::nullopt; }
    template <typename U = std::optional<T>>
    static constexpr auto get_value(U && obj) noexcept { return *CB_FWD(obj); }
};

// Concept
template <typename T> concept nullable = requires(T const & t) {
    typename nullable_traits<T>::value_type;
    nullable_traits<T>::is_null;
    { nullable_traits<T>::is_null(t) } -> std::same_as<bool>;
    { nullable_traits<T>::get_value(t) };
};

// static_assert(nullable<std::optional<int>>);
// static_assert(nullable_traits<std::optional<int>>::is_null({}));
// static_assert(std::optional<int>{} == null);

}  // namespace callables

#include "detail/_config_end.hpp"  // IWYU pragma: export
#endif /* CB_NULLABLE_HPP */
