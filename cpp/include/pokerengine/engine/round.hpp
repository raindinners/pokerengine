//
// Created by copper_boy on 10/28/23.
//

#ifndef POKERENGINE_ROUND_HPP
#define POKERENGINE_ROUND_HPP

#include <cstdint>

#include "pokerengine.hpp"

namespace pokerengine {
namespace enums {
enum class rounds_t : uint8_t {
    preflop = 0,
    flop = 1,
    turn = 2,
    river = 3,
    showdown = 4,
};
} // namespace enums

class round_manager {
public:
    explicit round_manager(enums::rounds_t round = enums::rounds_t::preflop, bool flop_dealt = false)
            : round_{ round }, flop_dealt_{ flop_dealt } {
    }

    [[nodiscard]] auto get_round() const noexcept -> enums::rounds_t {
        return round_;
    }

    [[nodiscard]] auto get_flop_dealt() const noexcept -> bool {
        return flop_dealt_;
    }

    auto set_round(enums::rounds_t round) noexcept -> void {
        round_ = round;
    }

    auto set_flop_dealt(bool flop_dealt) noexcept -> void {
        flop_dealt_ = flop_dealt;
    }

    auto set_next_round() noexcept -> void {
        set_round(static_cast< enums::rounds_t >(static_cast< uint8_t >(round_) + 1));
        if (round_ == enums::rounds_t::flop) {
            set_flop_dealt(flop_dealt_);
        }
    }

    auto set_next_game() noexcept -> void {
        set_round(enums::rounds_t::preflop);
        set_flop_dealt(false);
    }

private:
    enums::rounds_t round_;
    bool flop_dealt_;
};
} // namespace pokerengine

#endif // POKERENGINE_ROUND_HPP
