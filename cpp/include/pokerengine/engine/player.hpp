//
// Created by copper_boy on 10/8/23.
//

#ifndef POKERENGINE_PLAYER_HPP
#define POKERENGINE_PLAYER_HPP

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "pokerengine.hpp"

namespace pokerengine {
namespace enums {
enum class state_t {
    init = 0,
    out = 1,
    alive = 2,
    allin = 3,
};
} // namespace enums

struct player {
    bool is_left = false;

    int32_t stack;
    int32_t behind;
    int32_t front;
    int32_t round_bet;

    enums::state_t state;

    auto operator<=>(const player &other) const noexcept -> std::strong_ordering = delete;
    auto operator==(const player &other) const noexcept -> bool = default;
};

class players_set {
public:
    players_set() = default;

    [[nodiscard]] auto get_players() const noexcept -> std::vector< player > {
        return players_;
    }

    [[nodiscard]] auto get_player(uint8_t index) -> player & {
        return *(players_.begin() + index);
    }

    auto set_players(const std::vector< player > &players) noexcept -> void {
        players_ = players;
    }

    auto add_player(int32_t stack, enums::state_t state = enums::state_t::init) -> void {
        players_.emplace_back(false, stack, 0, 0, 0, state);
    }

    auto remove_player(uint8_t index) -> void {
        players_[index].is_left = true;
    }

private:
    std::vector< player > players_;
};
} // namespace pokerengine

#endif // POKERENGINE_PLAYER_HPP
