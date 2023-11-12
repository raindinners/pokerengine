//
// Created by copper_boy on 10/8/23.
//

#ifndef POKERENGINE_POSITIONS_HPP
#define POKERENGINE_POSITIONS_HPP

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <string>

#include "constants.hpp"
#include "engine/player.hpp"
#include "engine/round.hpp"
#include "enums.hpp"
#include "pokerengine.hpp"
#include "utils.hpp"
namespace pokerengine {
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

    return players[static_cast< uint8_t >(enums::position::sb)].state == enums::state::allin &&
                    players[static_cast< uint8_t >(enums::position::bb)].state == enums::state::allin;
}

auto set_blinds(std::vector< player > &players, int32_t sb_bet, int32_t bb_bet) -> void {
    std::for_each(players.begin(), players.end(), [&, index = 0](auto &player) mutable -> void {
        player.state = enums::state::init;
        player.behind = player.stack;
        player.round_bet = 0;
        player.front = 0;

        if (index < 2) {
            if (players.size() == constants::MIN_PLAYERS) {
                player.front = index == 0 ? bb_bet : sb_bet;
            } else {
                player.front = index == 0 ? sb_bet : bb_bet;
            }

            player.state = enums::state::alive;

            if (player.front > player.behind) {
                player.front = player.behind;
            }

            player.behind -= player.front;
            player.round_bet = player.front;

            if (player.behind == 0) {
                player.state = enums::state::allin;
            }
        }

        index++;
    });
}
} // namespace v1

class positions_manager : public utils::reference< players_set > {
public:
    using utils::reference< players_set >::reference;

    [[nodiscard]] auto get_current() const noexcept -> enums::position {
        return current_;
    }

    [[nodiscard]] auto get_player() const -> player {
        return value_.get_player(static_cast< int8_t >(get_current()));
    }

    [[nodiscard]] auto get_player() -> player & {
        return value_.get_player(static_cast< int8_t >(get_current()));
    }

    [[nodiscard]] auto get_number_alive() const -> int {
        auto iterable = value_.get_players();
        return std::accumulate(
                        iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
                            return element.state != enums::state::out ? value + 1 : value;
                        });
    }

    [[nodiscard]] auto get_actionable(int32_t highest_round_bet) const -> int {
        auto iterable = value_.get_players();
        return std::accumulate(iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
            return (element.state == enums::state::init ||
                    (element.state == enums::state::alive && element.round_bet < highest_round_bet)) ?
                            value + 1 :
                            value;
        });
    }

    [[nodiscard]] auto get_future_actionable() const -> int {
        auto iterable = value_.get_players();
        return std::accumulate(iterable.cbegin(), iterable.cend(), 0, [&](int value, auto const &element) -> int {
            return element.state != enums::state::out && element.state != enums::state::allin ? value + 1 :
                                                                                                value;
        });
    }

    [[nodiscard]] auto in_terminal_state() const -> bool {
        return get_number_alive() > 1;
    }

    auto set_current(enums::position position) -> void {
        current_ = position;
    }

    auto set_next_current() -> void {
        auto iterable_size = value_.get_players().size();
        do {
            set_current(static_cast< enums::position >(
                            (static_cast< uint8_t >(get_current()) + 1) % iterable_size));
        } while (get_player().state == enums::state::out || get_player().state == enums::state::allin);
    }

    auto set_next_round_player() -> void {
        set_current(enums::position::sb);

        auto iterable_size = value_.get_players().size();
        while (get_player().state == enums::state::out || get_player().state == enums::state::allin) {
            set_current(static_cast< enums::position >(
                            (static_cast< uint8_t >(get_current()) + 1) % iterable_size));
        }
    }

private:
    enums::position current_ = enums::position::none;
};
} // namespace pokerengine

#endif // POKERENGINE_POSITIONS_HPP
