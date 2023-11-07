//
// Created by copper_boy on 10/28/23.
//

#ifndef POKERENGINE_ROUND_HPP
#define POKERENGINE_ROUND_HPP

#include <cstdint>

#include "enums.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
class round_manager {
public:
    explicit round_manager(enums::round round = enums::round::preflop, bool flop_dealt = false)
            : round_{ round }, flop_dealt_{ flop_dealt } {
    }

    [[nodiscard]] auto get_round() const noexcept -> enums::round {
        return round_;
    }

    [[nodiscard]] auto get_flop_dealt() const noexcept -> bool {
        return flop_dealt_;
    }

    auto set_round(enums::round round) noexcept -> void {
        round_ = round;
    }

    auto set_flop_dealt(bool flop_dealt) noexcept -> void {
        flop_dealt_ = flop_dealt;
    }

    auto set_next_round() noexcept -> void {
        set_round(static_cast< enums::round >(static_cast< uint8_t >(round_) + 1));
        if (round_ == enums::round::flop) {
            set_flop_dealt(flop_dealt_);
        }
    }

    auto set_next_game() noexcept -> void {
        set_round(enums::round::preflop);
        set_flop_dealt(false);
    }

private:
    enums::round round_;
    bool flop_dealt_;
};
} // namespace pokerengine

#endif // POKERENGINE_ROUND_HPP
