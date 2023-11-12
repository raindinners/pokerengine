//
// Created by copper_boy on 10/28/23.
//

#ifndef POKERENGINE_ROUND_HPP
#define POKERENGINE_ROUND_HPP

#include <cstdint>

#include "enums.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
namespace v1 {
auto get_next_round(enums::round round) -> std::tuple< enums::round, bool > {
    auto result = static_cast< enums::round >(static_cast< uint8_t >(round) + 1);
    return std::make_tuple(result, result == enums::round::flop);
}
} // namespace v1

class round_manager {
public:
    explicit round_manager(enums::round round = enums::round::preflop, bool flop_dealt = false)
            : round_{ round }, flop_dealt_{ flop_dealt } {
    }

    auto reset() noexcept -> void {
        set_round(enums::round::preflop);
        set_flop_dealt(false);
    }

    [[nodiscard]] auto is_showdown() const noexcept -> bool {
        return get_round() == enums::round::showdown;
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

private:
    enums::round round_;
    bool flop_dealt_;
};
} // namespace pokerengine

#endif // POKERENGINE_ROUND_HPP
