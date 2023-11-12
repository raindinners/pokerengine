//
// Created by copper_boy on 11/12/23.
//

#ifndef POKERENGINE_ACTION_HPP
#define POKERENGINE_ACTION_HPP

#include <algorithm>
#include <vector>

#include "engine/player.hpp"
#include "enums.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
struct player_action {
    int32_t amount;
    enums::action action;
    enums::position position;

    auto operator<=>(const player_action &other) const noexcept -> std::strong_ordering = delete;
    auto operator==(const player_action &other) const noexcept -> bool = default;

    explicit operator std::string() const {
        using namespace std::literals;

        return "PlayerAction<amount="s + std::to_string(amount) + ", action="s +
                        std::string{ magic_enum::enum_name(action) } + ", position="s +
                        std::string{ magic_enum::enum_name(position) } + ">"s;
    }
};

namespace v1 {
auto is_no_actions_available(enums::round round, enums::state state) noexcept -> bool {
    return (state == enums::state::out || state == enums::state::allin) || round == enums::round::showdown;
}

auto get_fold_or_check(enums::position player, int32_t highest_round_bet, int32_t committed) noexcept
                -> player_action {
    if (committed == highest_round_bet) {
        return player_action{ 0, enums::action::check, player };
    }
    return player_action{ 0, enums::action::fold, player };
}

auto is_bet_available(int32_t bb_bet, int32_t highest_round_bet, int32_t remaining) noexcept -> bool {
    return !highest_round_bet && remaining > bb_bet;
}

auto is_call_available(int32_t highest_round_bet, int32_t committed, int32_t remaining) noexcept -> bool {
    return highest_round_bet && (committed < highest_round_bet && (committed + remaining) > highest_round_bet);
}

auto is_raise_available(
                enums::state state,
                int32_t highest_round_bet,
                int32_t min_raise,
                int32_t committed,
                int32_t remaining) -> bool {
    if ((committed + remaining) <= highest_round_bet + min_raise) {
        return false;
    }

    return (state == enums::state::init || state == enums::state::alive) ||
                    (committed < highest_round_bet && (highest_round_bet - committed >= min_raise));
}

auto is_allin_available(int32_t remaining) noexcept -> bool {
    return remaining > 0;
}

auto get_possible_actions(
                enums::round round,
                enums::position player,
                enums::state state,
                bool is_left,
                int32_t bb_bet,
                int32_t min_raise,
                int32_t highest_round_bet,
                int32_t committed,
                int32_t remaining) -> std::vector< player_action > {
    if (is_no_actions_available(round, state)) {
        return std::vector< player_action >{ player_action{ 0, enums::action::none, enums::position::none } };
    }

    std::vector< player_action > actions{ get_fold_or_check(player, highest_round_bet, committed) };

    if (is_left) {
        return actions;
    }

    if (is_bet_available(bb_bet, highest_round_bet, remaining)) {
        actions.emplace_back(bb_bet, enums::action::bet, player);
    }

    if (is_call_available(highest_round_bet, committed, remaining)) {
        actions.emplace_back(highest_round_bet - committed, enums::action::call, player);
    }

    if (is_raise_available(state, highest_round_bet, min_raise, committed, remaining)) {
        actions.emplace_back(remaining, enums::action::raise, player);
    }

    if (is_allin_available(remaining)) {
        actions.emplace_back(remaining, enums::action::allin, player);
    }

    return actions;
}

auto is_normal_action(
                const player_action &pa,
                const std::vector< player_action > &all_actions,
                enums::position player,
                int32_t min_raise) -> bool {
    return pa.position == player && player != enums::position::none &&
                    (std::find_if(all_actions.cbegin(), all_actions.cend(), [&](const auto &element) -> bool {
                         return (element == pa) ||
                                         (pa.action == enums::action::raise &&
                                          element.action == enums::action::raise &&
                                          pa.amount < element.amount && pa.amount >= min_raise);
                     }) != all_actions.cend());
}

auto execute_action(const player_action &pa, player &player, int32_t min_raise, int32_t highest_round_bet)
                -> int32_t {
    int32_t new_min_raise = min_raise;

    switch (pa.action) {
    case enums::action::fold: {
        player.state = enums::state::out;
    } break;
    case enums::action::check: {
        player.state = enums::state::alive;
    } break;
    case enums::action::call:
    case enums::action::bet:
    case enums::action::raise:
    case enums::action::allin: {
        int32_t raise_size = pa.amount + player.round_bet - highest_round_bet;
        if (raise_size > min_raise) {
            new_min_raise = raise_size;
        }

        player.behind -= pa.amount;
        player.front += pa.amount;
        player.round_bet += pa.amount;

        player.state = player.behind == 0 ? enums::state::allin : enums::state::alive;
    } break;
    default: {
        throw std::runtime_error{ "Got invalid action to execute" };
    }
    }

    return new_min_raise;
}
} // namespace v1
} // namespace pokerengine

#endif // POKERENGINE_ACTION_HPP
