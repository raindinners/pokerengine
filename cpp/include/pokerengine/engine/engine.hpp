//
// Created by copper_boy on 10/6/23.
//

#ifndef POKERENGINE_ENGINE_HPP
#define POKERENGINE_ENGINE_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <compare>
#include <cstdint>
#include <functional>
#include <map>
#include <numeric>
#include <span>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "card/cards.hpp"
#include "engine/player.hpp"
#include "engine/positions.hpp"
#include "engine/pot.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
namespace enums {
enum class action_t : uint8_t {
    fold = 0,
    check = 1,
    call = 2,
    bet = 3,
    raise = 4,
    allin = 5,
    no_action = 6,
};
} // namespace enums

struct player_action {
    int32_t amount;
    enums::action_t action;
    enums::position_t position;

    auto operator<=>(const player_action &other) const noexcept -> std::strong_ordering = delete;
    auto operator==(const player_action &other) const noexcept -> bool = default;
};

namespace v1 {
auto is_no_actions_available(enums::rounds_t round, enums::state_t state) noexcept -> bool {
    return (state == enums::state_t::out || state == enums::state_t::allin) ||
                    round == enums::rounds_t::showdown;
}

auto get_fold_or_check(enums::position_t player, int32_t highest_round_bet, int32_t committed) noexcept
                -> player_action {
    if (committed == highest_round_bet) {
        return player_action{ 0, enums::action_t::check, player };
    }
    return player_action{ 0, enums::action_t::fold, player };
}

auto is_bet_available(int32_t bb_bet, int32_t highest_round_bet, int32_t remaining) noexcept -> bool {
    return !highest_round_bet && remaining > bb_bet;
}

auto is_call_available(int32_t highest_round_bet, int32_t committed, int32_t remaining) noexcept -> bool {
    return highest_round_bet && (committed < highest_round_bet && (committed + remaining) > highest_round_bet);
}

auto is_raise_available(
                enums::state_t state,
                int32_t highest_round_bet,
                int32_t min_raise,
                int32_t committed,
                int32_t remaining) -> bool {
    if ((committed + remaining) <= highest_round_bet + min_raise) {
        return false;
    }

    return (state == enums::state_t::init || state == enums::state_t::alive) ||
                    (committed < highest_round_bet && (highest_round_bet - committed >= min_raise));
}

auto is_allin_available(int32_t remaining) noexcept -> bool {
    return remaining > 0;
}

auto get_possible_actions(
                enums::rounds_t round,
                enums::position_t player,
                enums::state_t state,
                bool is_left,
                int32_t bb_bet,
                int32_t min_raise,
                int32_t highest_round_bet,
                int32_t committed,
                int32_t remaining) -> std::vector< player_action > {
    if (is_no_actions_available(round, state)) {
        return std::vector< player_action >{ player_action{
                        0, enums::action_t::no_action, enums::position_t::no_position } };
    }

    std::vector< player_action > actions{ get_fold_or_check(player, highest_round_bet, committed) };

    if (is_left) {
        return actions;
    }

    if (is_bet_available(bb_bet, highest_round_bet, remaining)) {
        actions.emplace_back(bb_bet, enums::action_t::bet, player);
    }

    if (is_call_available(highest_round_bet, committed, remaining)) {
        actions.emplace_back(highest_round_bet - committed, enums::action_t::call, player);
    }

    if (is_raise_available(state, highest_round_bet, min_raise, committed, remaining)) {
        actions.emplace_back(remaining, enums::action_t::raise, player);
    }

    if (is_allin_available(remaining)) {
        actions.emplace_back(remaining, enums::action_t::allin, player);
    }

    return actions;
}

auto is_normal_action(
                const player_action &pa,
                const std::vector< player_action > &all_actions,
                enums::position_t player,
                int32_t min_raise) -> bool {
    return pa.position == player && player != enums::position_t::no_position &&
                    (std::find_if(all_actions.cbegin(), all_actions.cend(), [&](const auto &element) -> bool {
                         return (element == pa) ||
                                         (pa.action == enums::action_t::raise &&
                                          element.action == enums::action_t::raise &&
                                          pa.amount < element.amount && pa.amount >= min_raise);
                     }) != all_actions.cend());
}

auto execute_action(const player_action &pa, player &player, int32_t min_raise, int32_t highest_round_bet)
                -> int32_t {
    int32_t new_min_raise = min_raise;

    switch (pa.action) {
    case enums::action_t::fold: {
        player.state = enums::state_t::out;
    } break;
    case enums::action_t::check: {
        player.state = enums::state_t::alive;
    } break;
    case enums::action_t::call:
    case enums::action_t::bet:
    case enums::action_t::raise:
    case enums::action_t::allin: {
        int32_t raise_size = pa.amount + player.round_bet - highest_round_bet;
        if (raise_size > min_raise) {
            new_min_raise = raise_size;
        }

        player.behind -= pa.amount;
        player.front += pa.amount;
        player.round_bet += pa.amount;

        player.state = player.behind == 0 ? enums::state_t::allin : enums::state_t::alive;
    } break;
    default: {
        throw std::runtime_error{ "Got invalid action to execute" };
    }
    }

    return new_min_raise;
}
} // namespace v1

class engine_traits {
public:
    engine_traits() = delete;

    engine_traits(int32_t sb_bet, int32_t bb_bet, uint8_t bb_mult)
            : sb_bet_{ sb_bet }, bb_bet_{ bb_bet }, bb_mult_{ bb_mult } {
        min_raise_ = bb_bet_;
    }

    [[nodiscard]] auto get_sb_bet() const noexcept -> int32_t {
        return sb_bet_;
    }

    [[nodiscard]] auto get_bb_bet() const noexcept -> int32_t {
        return bb_bet_;
    }

    [[nodiscard]] auto get_bb_mult() const noexcept -> uint8_t {
        return bb_mult_;
    }

    [[nodiscard]] auto get_min_raise() const noexcept -> int32_t {
        return min_raise_;
    }

    auto set_sb_bet(int32_t sb_bet) noexcept -> void {
        sb_bet_ = sb_bet;
    }

    auto set_bb_bet(int32_t bb_bet) noexcept -> void {
        bb_bet_ = bb_bet;
    }

    auto set_bb_mult(uint8_t bb_mult) noexcept -> void {
        bb_mult_ = bb_mult;
    }

    auto set_min_raise(int32_t min_raise) noexcept -> void {
        min_raise_ = min_raise;
    }

private:
    int32_t sb_bet_;
    int32_t bb_bet_;
    uint8_t bb_mult_;

    int32_t min_raise_;
};


template < uint8_t A = 0, uint8_t B = 1 >
    requires(A >= 0 && B > 0 && A < B)
class engine {
public:
    engine() = delete;

    explicit engine(const engine_traits &engine_traits) : engine_traits_{ engine_traits } {
    }

    auto start(bool is_new_game = false) -> void {
        stop();

        auto new_players = actual::get_players(players.get_players(), is_new_game);
        actual::set_blinds(new_players, engine_traits_.get_sb_bet(), engine_traits_.get_bb_bet());

        players.set_players(new_players);

        if (actual::is_all_allin(new_players)) {
            round.set_round(enums::rounds_t::showdown);
        } else {
            positions.set_current(
                            new_players.size() > constants::MIN_PLAYERS ? enums::position_t::utg :
                                                                          enums::position_t::bb);

            if (positions.get_player().state == enums::state_t::allin) {
                positions.set_next_current();
            }
        }
    }

    auto stop() -> void {
        engine_traits_.set_min_raise(engine_traits_.get_bb_bet());
        round.set_next_game();
    }

    [[nodiscard]] auto in_terminal_state() const noexcept -> bool {
        return round.get_round() == enums::rounds_t::showdown;
    }

    [[nodiscard]] auto is_showdown() const -> bool {
        return get_number_alive() > 1;
    }

    [[nodiscard]] auto get_engine_traits() noexcept -> engine_traits & {
        return engine_traits_;
    }

    [[nodiscard]] auto get_possible_actions() const -> std::vector< player_action > {
        auto player = positions.get_player();
        return actual::get_possible_actions(
                        round.get_round(),
                        positions.get_current(),
                        player.state,
                        player.is_left,
                        engine_traits_.get_bb_bet(),
                        engine_traits_.get_min_raise(),
                        pot.get_round_highest_bet(),
                        player.front,
                        player.behind);
    }

    auto set_engine_traits(const engine_traits &engine_traits) noexcept -> void {
        engine_traits_ = engine_traits;
    }

    auto execute_action(const player_action &pa) -> void {
        if (!actual::is_normal_action(
                            pa, get_possible_actions(), positions.get_current(), engine_traits_.get_min_raise())) {
            throw std::runtime_error{ "Execute action failed" };
        }

        auto &player = positions.get_player();
        engine_traits_.set_min_raise(actual::execute_action(
                        pa, positions.get_player(), engine_traits_.get_min_raise(), pot.get_round_highest_bet()));

        auto iterable = players.get_players();
        if (auto actions = get_actionable(); get_number_alive() < constants::MIN_PLAYERS ||
            (actions == 0 && get_future_actionable() < constants::MIN_PLAYERS)) {
            if (!round.get_flop_dealt()) {
                round.set_flop_dealt(
                                std::find_if(iterable.cbegin(), iterable.cend(), [](const auto &value) -> bool {
                                    return value.state == enums::state_t::allin;
                                }) != iterable.cend());
            }

            round.set_round(enums::rounds_t::showdown);
        } else if (auto last_player = std::distance(
                                   iterable.cbegin(),
                                   std::find_if(iterable.cbegin(),
                                                iterable.cend(),
                                                [](const auto &element) -> bool {
                                                    return element.state != enums::state_t::out &&
                                                                    element.state != enums::state_t::allin;
                                                }));
                   get_actionable() == 1 && get_future_actionable() == 1 &&
                   iterable[last_player].round_bet == pot.get_round_highest_bet()) {
            round.set_flop_dealt(true), round.set_round(enums::rounds_t::showdown);
        } else if (actions == 0) {
            if (round.get_round() == enums::rounds_t::river) {
                round.set_round(enums::rounds_t::showdown);
            } else {
                positions.set_next_round_player();

                engine_traits_.set_min_raise(engine_traits_.get_bb_bet());
                round.set_next_round();

                std::for_each(iterable.begin(), iterable.end(), [](auto &element) {
                    element.round_bet = 0;
                    element.state = element.state == enums::state_t::alive ? enums::state_t::init :
                                                                             element.state;
                });
            }
        } else {
            positions.set_next_current();
        }
    }

    players_set players;

    round_manager round;
    positions_manager positions{ players };

    pot_manager< A, B > pot{ players };

protected:
    [[nodiscard]] auto get_number_alive() const -> int {
        auto iterable = players.get_players();
        return std::accumulate(
                        iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
                            return element.state != enums::state_t::out ? value + 1 : value;
                        });
    }

    [[nodiscard]] auto get_actionable() const -> int {
        auto iterable = players.get_players();
        return std::accumulate(
                        iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
                            return (element.state == enums::state_t::init ||
                                    (element.state == enums::state_t::alive &&
                                     element.round_bet < pot.get_round_highest_bet())) ?
                                            value + 1 :
                                            value;
                        });
    }

    [[nodiscard]] auto get_future_actionable() const -> int {
        auto iterable = players.get_players();
        return std::accumulate(iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
            return element.state != enums::state_t::out && element.state != enums::state_t::allin ? value + 1 :
                                                                                                    value;
        });
    }

private:
    engine_traits engine_traits_;
};
} // namespace pokerengine

#endif // POKERENGINE_ENGINE_HPP
