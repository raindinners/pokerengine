//
// Created by copper_boy on 7/4/23.
//

#ifndef POKERENGINE_BITS_HPP
#define POKERENGINE_BITS_HPP

#include <cstdint>

#include "pokerengine.hpp"

namespace pokerengine::bits {
uint8_t cross_idx_low16(uint16_t mask);

uint8_t cross_idx_high16(uint16_t mask);
} // namespace pokerengine::bits

#endif // POKERENGINE_BITS_HPP
