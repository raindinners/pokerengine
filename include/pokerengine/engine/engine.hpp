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

#include <magic_enum.hpp>

#include "card/cards.hpp"
#include "constants.hpp"
#include "engine/action.hpp"
#include "engine/player.hpp"
#include "engine/positions.hpp"
#include "engine/pot.hpp"
#include "enums.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
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
            round.set_round(enums::round::showdown);
        } else {
            positions.set_current(
                            new_players.size() > constants::MIN_PLAYERS ? enums::position::utg :
                                                                          enums::position::bb);

            if (positions.get_player().state == enums::state::allin) {
                positions.set_next_current();
            }
        }
    }

    auto stop() -> void {
        engine_traits_.set_min_raise(engine_traits_.get_bb_bet());
        round.reset();
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
        if (auto actions = positions.get_actionable(pot.get_round_highest_bet());
            positions.get_number_alive() < constants::MIN_PLAYERS ||
            (actions == 0 && positions.get_future_actionable() < constants::MIN_PLAYERS)) {
            if (!round.get_flop_dealt()) {
                round.set_flop_dealt(
                                std::find_if(iterable.cbegin(), iterable.cend(), [](const auto &value) -> bool {
                                    return value.state == enums::state::allin;
                                }) != iterable.cend());
            }

            round.set_round(enums::round::showdown);
        } else if (auto last_player = std::distance(
                                   iterable.cbegin(),
                                   std::find_if(iterable.cbegin(),
                                                iterable.cend(),
                                                [](const auto &element) -> bool {
                                                    return element.state != enums::state::out &&
                                                                    element.state != enums::state::allin;
                                                }));
                   positions.get_actionable(pot.get_round_highest_bet()) == 1 &&
                   positions.get_future_actionable() == 1 &&
                   iterable[last_player].round_bet == pot.get_round_highest_bet()) {
            round.set_flop_dealt(true), round.set_round(enums::round::showdown);
        } else if (actions == 0) {
            if (round.get_round() == enums::round::river) {
                round.set_round(enums::round::showdown);
            } else {
                positions.set_next_round_player();

                engine_traits_.set_min_raise(engine_traits_.get_bb_bet());
                auto next_round = actual::get_next_round(round.get_round());
                round.set_round(std::get< 0 >(next_round)), round.set_flop_dealt(std::get< 1 >(next_round));

                std::for_each(iterable.begin(), iterable.end(), [](auto &element) {
                    element.round_bet = 0;
                    element.state = element.state == enums::state::alive ? enums::state::init : element.state;
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

private:
    engine_traits engine_traits_;
};
} // namespace pokerengine

#endif // POKERENGINE_ENGINE_HPP
