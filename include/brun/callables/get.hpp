/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Monday Feb 02, 2026 16:54:42 CET
 * @description : 
 * */

#ifndef CB_GET_HPP
#define CB_GET_HPP

#include <tuple>
#include "detail/fixed_string.hpp"

#include "detail/_config_begin.hpp"

#if CB_HAS_REFLECTION != 0
#include <meta>
#include <string>
#include <string_view>
#include <type_traits>
#endif

namespace callables
{

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ....................................GET..................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
template <std::size_t ...Ns>
struct get_fn
{
    template <typename Obj>
    constexpr CB_STATIC
    auto operator()(Obj && obj) CB_CONST noexcept((noexcept(get<Ns>(CB_FWD(obj))) and ...))
        -> decltype(auto)
    {
        return std::tuple<decltype(get<Ns>(CB_FWD(obj)))...>{get<Ns>(CB_FWD(obj))...};
    }
};

template <std::size_t N>
struct get_fn<N>
{
    template <typename Obj>
    constexpr CB_STATIC
    auto operator()(Obj && obj) CB_CONST noexcept(noexcept(get<N>(CB_FWD(obj))))
        -> decltype(auto)
    { return get<N>(CB_FWD(obj)); }
};

template <std::size_t ...Ns>
constexpr inline get_fn<Ns...> get;

#if CB_HAS_REFLECTION != 0
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
// ...................................MEMBER................................... //
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... //
namespace detail
{
// The public non-static data member of `T` called `Member`, or a null reflection
template <typename T, fixed_string Member>
consteval auto find_data_member() -> std::meta::info
{
    if (not std::meta::is_class_type(^^T) and not std::meta::is_union_type(^^T)) {
        return {};
    }
    for (auto member : std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unprivileged())) {
        if (std::meta::has_identifier(member) and std::meta::identifier_of(member) == std::string_view{Member}) {
            return member;
        }
    }
    return {};
}

template <typename T, fixed_string Member>
consteval auto no_member_message() -> std::string_view
{
    auto const message = std::string{"type '"}
                       + std::string{std::meta::display_string_of(^^T)}
                       + "' has no public data member named '"
                       + std::string{std::string_view{Member}}
                       + "'";
    return std::define_static_string(message);
}

template <fixed_string Member, typename T>
constexpr auto get_member(T && obj) noexcept -> decltype(auto)
{
    using type = std::remove_cvref_t<T>;
    constexpr auto member = find_data_member<type, Member>();

    if constexpr (member == std::meta::info{}) {
        static_assert(false, no_member_message<type, Member>());
    } else if constexpr (std::meta::is_bit_field(member)) {
        return CB_FWD(obj).[:member:];
    } else {
        return (CB_FWD(obj).[:member:]);
    }
}
}  // namespace detail

template <fixed_string ...Members>
    requires (sizeof...(Members) > 0)
struct member_fn
{
    template <typename Obj>
    constexpr CB_STATIC
    auto operator()(Obj && obj) CB_CONST noexcept -> decltype(auto)
    {
        if constexpr (sizeof...(Members) == 1) {
            return detail::get_member<Members...[0]>(CB_FWD(obj));
        } else {
            return std::tuple<decltype(detail::get_member<Members>(CB_FWD(obj)))...>{
                detail::get_member<Members>(CB_FWD(obj))...
            };
        }
    }
};

template <fixed_string ...Members>
constexpr inline member_fn<Members...> member;
#endif

} // namespace callables

#include "detail/_config_end.hpp"  // IWYU pragma: export

#endif /* CB_GET_HPP */
