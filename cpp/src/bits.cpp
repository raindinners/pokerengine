//
// Created by copper_boy on 7/6/23.
//

#include "bits.hpp"

#include <bit>

namespace pokerengine::bits {
uint8_t cross_idx_low16(uint16_t mask) {
    return mask == 0 ? 0 : static_cast< uint8_t >(std::countr_zero(mask));
}

uint8_t cross_idx_high16(uint16_t mask) {
    return mask == 0 ? 0 : static_cast< uint8_t >(15 - std::countl_zero(mask));
}
} // namespace pokerengine::bits
