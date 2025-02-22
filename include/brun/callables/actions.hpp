/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Wednesday Feb 19, 2025 12:07:01 CET
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

#ifndef CB_ACTIONS_HPP
#define CB_ACTIONS_HPP

namespace callables
{

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ....oooOO0OOooo....                 FOLD                 ....oooOO0OOooo......
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

template <typename Action, typename BinaryOp, typename Init>
struct action_capture
{
    [[no_unique_address]] BinaryOp _fn;
    Init _init;

    template <std::ranges::input_range Rng>
        requires std::invocable<BinaryOp, Init, std::ranges::range_value_t<Rng>>
        and std::convertible_to<std::invoke_result_t<BinaryOp, Init, std::ranges::range_value_t<Rng>>, Init>
    constexpr auto operator()(Rng && rng) const -> decltype(auto)
    {
        return Action{}(std::forward<Rng>(rng), _init, std::cref(_fn));
    }
};

template <typename Action, typename BinaryOp>
struct action_capture<Action, BinaryOp, void>
{
    [[no_unique_address]] BinaryOp _fn;

    template <std::ranges::input_range Rng>
        requires std::invocable<BinaryOp, std::ranges::range_value_t<Rng>, std::ranges::range_value_t<Rng>>
        and std::convertible_to<
            std::invoke_result_t<BinaryOp, std::ranges::range_value_t<Rng>, std::ranges::range_value_t<Rng>>,
            std::ranges::range_value_t<Rng>
        >
    constexpr auto operator()(Rng && rng) const -> decltype(auto)
    {
        return Action{}(std::forward<Rng>(rng), std::cref(_fn));
    }
};

template <std::ranges::input_range Rng, typename Action, typename Cb, typename I>
    requires std::invocable<action_capture<Action, Cb, I>, Rng>
constexpr auto operator|(Rng && rng, action_capture<Action, Cb, I> const & capture) -> decltype(auto)
{
    return capture(std::forward<Rng>(rng));
}

struct fold_fn
{
    template <std::input_iterator I, std::sentinel_for<I> S, typename Init = std::iter_value_t<I>, typename Cb>
    constexpr static auto operator()(I first, S last, Init init, Cb binary_fn) -> decltype(auto)
    {
        return std::ranges::fold_left(std::forward<I>(first), std::forward<S>(last), std::move(init), std::move(binary_fn));
    }

    template <std::ranges::input_range Rng, typename Init = std::ranges::range_value_t<Rng>, typename Cb>
    constexpr static auto operator()(Rng && rng, Init init, Cb binary_fn) -> decltype(auto)
    {
        return std::ranges::fold_left(std::forward<Rng>(rng), std::move(init), std::move(binary_fn));
    }

    template <std::input_iterator I, std::sentinel_for<I> S, typename Cb>
    constexpr static auto operator()(I first, S last, Cb binary_fn) -> decltype(auto)
    {
        return std::ranges::fold_left_first(std::forward<I>(first), std::forward<S>(last), std::move(binary_fn));
    }

    template <std::ranges::input_range Rng, typename Cb>
    constexpr static auto operator()(Rng && rng, Cb binary_fn) -> decltype(auto)
    {
        return std::ranges::fold_left_first(std::forward<Rng>(rng), std::move(binary_fn));
    }

    template <typename Cb, typename Init>
        requires (not std::ranges::input_range<Cb>)
    constexpr static auto operator()(Cb binary_fn, Init init) noexcept
    { return action_capture<fold_fn, Cb, Init>{std::move(binary_fn), std::move(init)}; }

    template <typename Cb>
    constexpr static auto operator()(Cb binary_fn) noexcept
    { return action_capture<fold_fn, Cb, void>{std::move(binary_fn)}; }
};

constexpr inline fold_fn fold;

}  // namespace callables

#endif /* CB_ACTIONS_HPP */
