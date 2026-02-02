/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Monday Feb 02, 2026 16:54:42 CET
 * @description : 
 * */

#ifndef CB_GET_HPP
#define CB_GET_HPP

#include "detail/_config_begin.hpp"
#include <tuple>

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

} // namespace callables

#include "detail/_config_end.hpp"  // IWYU pragma: export

#endif /* CB_GET_HPP */
