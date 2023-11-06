//
// Created by copper_boy on 10/8/23.
//

#ifndef POKERENGINE_POSITIONS_HPP
#define POKERENGINE_POSITIONS_HPP

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>

#include "engine/player.hpp"
#include "engine/round.hpp"
#include "pokerengine.hpp"
#include "utils.hpp"

namespace pokerengine {
namespace enums {
enum class position_t : uint8_t {
    sb = 0,
    bb = 1,
    utg = 2,
    lwj [[maybe_unused]] = 3,
    hij [[maybe_unused]] = 4,
    cof [[maybe_unused]] = 5,
    btn [[maybe_unused]] = 6,
    no_position = 7,
};
} // namespace enums

namespace constants {
uint8_t MIN_PLAYERS = 2;
uint8_t MAX_PLAYERS = 7;
} // namespace constants

namespace v1 {
auto get_players(const std::vector< player > &players, bool rotate = false) -> std::vector< player > {
    std::vector< player > result;
    std::copy_if(players.cbegin(), players.cend(), std::back_inserter(result), [](const auto &player) -> bool {
        return !(player.stack <= 0 || player.is_left);
    });

    if (result.size() < constants::MIN_PLAYERS || result.size() > constants::MAX_PLAYERS) {
        throw std::runtime_error{ "Players size invalid" };
    }

    if (rotate) {
        std::rotate(result.rbegin(), result.rbegin() + 1, result.rend());
    }

    return result;
}

auto is_all_allin(const std::vector< player > &players) -> bool {
    if (players.size() != constants::MIN_PLAYERS) {
        return false;
    }

    return players[static_cast< uint8_t >(enums::position_t::sb)].state == enums::state_t::allin &&
                    players[static_cast< uint8_t >(enums::position_t::bb)].state == enums::state_t::allin;
}

auto set_blinds(std::vector< player > &players, int32_t sb_bet, int32_t bb_bet) -> void {
    std::for_each(players.begin(), players.end(), [&, index = 0](auto &player) mutable -> void {
        player.state = enums::state_t::init;
        player.behind = player.stack;
        player.round_bet = 0;
        player.front = 0;

        if (index < 2) {
            if (players.size() == constants::MIN_PLAYERS) {
                player.front = index == 0 ? bb_bet : sb_bet;
            } else {
                player.front = index == 0 ? sb_bet : bb_bet;
            }

            player.state = enums::state_t::alive;

            if (player.front > player.behind) {
                player.front = player.behind;
            }

            player.behind -= player.front;
            player.round_bet = player.front;

            if (player.behind == 0) {
                player.state = enums::state_t::allin;
            }
        }

        index++;
    });
}
} // namespace v1

class positions_manager : public utils::reference< players_set > {
public:
    using utils::reference< players_set >::reference;

    [[nodiscard]] auto get_current() const noexcept -> enums::position_t {
        return current_;
    }

    [[nodiscard]] auto get_player() const -> player {
        auto position = get_current();
        if (position == enums::position_t::no_position) {
            throw std::runtime_error{ "There is no player" };
        }

        return value_.get_player(static_cast< uint8_t >(position));
    }

    [[nodiscard]] auto get_player() -> player & {
        auto position = get_current();
        if (position == enums::position_t::no_position) {
            throw std::runtime_error{ "There is no player" };
        }

        return value_.get_player(static_cast< uint8_t >(position));
    }

    [[nodiscard]] auto get_number_alive() const -> int {
        auto iterable = value_.get_players();
        return std::accumulate(
                        iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
                            return element.state != enums::state_t::out ? value + 1 : value;
                        });
    }

    [[nodiscard]] auto is_showdown() const -> bool {
        return get_number_alive() > 1;
    }

    auto set_current(enums::position_t position) -> void {
        current_ = position;
    }

    auto set_next_current() -> void {
        auto iterable_size = value_.get_players().size();
        do {
            set_current(static_cast< enums::position_t >(
                            (static_cast< uint8_t >(get_current()) + 1) % iterable_size));
        } while (get_player().state == enums::state_t::out || get_player().state == enums::state_t::allin);
    }

    auto set_next_round_player() -> void {
        set_current(enums::position_t::sb);

        auto iterable_size = value_.get_players().size();
        while (get_player().state == enums::state_t::out || get_player().state == enums::state_t::allin) {
            set_current(static_cast< enums::position_t >(
                            (static_cast< uint8_t >(get_current()) + 1) % iterable_size));
        }
    }

private:
    enums::position_t current_ = enums::position_t::no_position;
};
} // namespace pokerengine

#endif // POKERENGINE_POSITIONS_HPP
