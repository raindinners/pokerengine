//
// Created by copper_boy on 11/8/23.
//

#ifndef POKERENGINE_ENUMS_HPP
#define POKERENGINE_ENUMS_HPP

#include <cstdint>

#include "pokerengine.hpp"

namespace pokerengine::enums {
enum class action : int {
    none = -1,
    fold = 0,
    check = 1,
    call = 2,
    bet = 3,
    raise = 4,
    allin = 5,
};

enum class combination : int {
    none = -1,
    no_pair = 0,
    one_pair = 1,
    two_pair = 2,
    three_of_a_kind = 3,
    straight = 4,
    flush = 5,
    full_house = 6,
    four_of_a_kind = 7,
    straight_flush = 8,
};

enum class position : int {
    none = -1,
    sb = 0,
    bb = 1,
    utg = 2,
    lwj [[maybe_unused]] = 3,
    hij [[maybe_unused]] = 4,
    cof [[maybe_unused]] = 5,
    btn [[maybe_unused]] = 6,
};

enum class rank : int {
    two = 0,
    three = 1,
    four = 2,
    five = 3,
    six = 4,
    seven = 5,
    eight = 6,
    nine = 7,
    ten = 8,
    jack = 9,
    queen = 10,
    king = 11,
    ace = 12,
};

enum class round : int {
    none = -1,
    preflop = 0,
    flop = 1,
    turn = 2,
    river = 3,
    showdown = 4,
};

enum class state : int {
    none = -1,
    init = 0,
    out = 1,
    alive = 2,
    allin = 3,
};

enum class suit : int32_t {
    clubs = 0,
    diamonds = 1,
    hearts = 2,
    spades = 3,
};
} // namespace pokerengine::enums

#endif // POKERENGINE_ENUMS_HPP
