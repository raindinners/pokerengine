//
// Created by copper_boy on 7/4/23.
//

#ifndef POKERENGINE_RESULT_HPP
#define POKERENGINE_RESULT_HPP

#include <compare>
#include <cstdint>
#include <string>

#include "bits.hpp"
#include "card/card.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
namespace constants {
const uint8_t OFFSET_MINOR = RANKS;
const uint8_t OFFSET_MAJOR = OFFSET_MINOR + 4;
const uint8_t OFFSET_TYPE = OFFSET_MAJOR + 4;

const std::array< const char *, 9 > COMBINATIONS = {
    "high card,", "a pair of",     "two pairs,",      "three of a kind,", "a straight,",
    "a flush,",   "a full house,", "four of a kind,", "a straight flush,"
};
} // namespace constants
namespace enums {
enum class combination_t : uint8_t {
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
} // namespace enums

class result {
public:
    result(enums::combination_t type, uint8_t major, uint8_t minor, uint16_t kickers) noexcept
            : result_{ static_cast< uint32_t >(
                              (static_cast< uint8_t >(type) << constants::OFFSET_TYPE) |
                              (major << constants::OFFSET_MAJOR) | (minor << constants::OFFSET_MINOR) |
                              (kickers & constants::MASK_RANKS)) } {
    }

    result(uint8_t type, uint8_t major, uint8_t minor, uint16_t kickers) noexcept
            : result_{ static_cast< uint32_t >(
                              (type << constants::OFFSET_TYPE) | (major << constants::OFFSET_MAJOR) |
                              (minor << constants::OFFSET_MINOR) | (kickers & constants::MASK_RANKS)) } {
    }

    auto operator<=>(const result &other) const -> std::strong_ordering = default;

    [[nodiscard]] auto get_result() const noexcept -> uint32_t {
        return result_;
    }

    [[nodiscard]] auto type() const noexcept -> uint8_t {
        return static_cast< uint8_t >(get_result() >> constants::OFFSET_TYPE);
    }

    [[nodiscard]] auto major_rank() const noexcept -> rank {
        return rank{ enums::rank_t(
                        constants::MASK_RANKS_NUMBER & ((get_result() >> constants::OFFSET_MAJOR) - 0)) };
    }

    [[nodiscard]] auto minor_rank() const noexcept -> rank {
        return rank{ enums::rank_t(
                        constants::MASK_RANKS_NUMBER & ((get_result() >> constants::OFFSET_MINOR) - 0)) };
    }

    [[nodiscard]] auto kickers() const noexcept -> uint32_t {
        return (constants::MASK_RANKS & get_result());
    }

    [[nodiscard]] auto as_bitset() const noexcept -> uint32_t {
        return get_result();
    }

    [[nodiscard]] auto as_string() const -> std::string {
        auto result = enums::combination_t{ type() };
        auto index = static_cast< uint8_t >(result);

        switch (result) {
        case enums::combination_t::no_pair: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            std::string{
                                rank{ enums::rank_t{ bits::cross_idx_high16(kickers()) } }.as_string_long()
                            };
        }
        case enums::combination_t::one_pair:
        case enums::combination_t::three_of_a_kind: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long();
        }
        case enums::combination_t::four_of_a_kind: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long();
        }
        case enums::combination_t::two_pair: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long() + std::string{ " and " } +
                            minor_rank().as_string_long();
        }
        case enums::combination_t::full_house: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long() + std::string{ " full of " } +
                            minor_rank().as_string_long();
        }
        case enums::combination_t::straight:
        case enums::combination_t::straight_flush: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            std::string{ major_rank().as_string_long() } + std::string{ " high" };
        }
        case enums::combination_t::flush: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            std::string{
                                rank{ enums::rank_t{ bits::cross_idx_high16(kickers()) } }.as_string_long()
                            } +
                            std::string{ " high" };
        }
        default: {
            throw std::runtime_error{ "Invalid evaluation result" };
        }
        }
    }

    [[nodiscard, maybe_unused]] auto as_string_long() const -> std::string {
        auto result = enums::combination_t(type());

        switch (result) {
        case enums::combination_t::no_pair:
        case enums::combination_t::one_pair:
        case enums::combination_t::three_of_a_kind:
        case enums::combination_t::four_of_a_kind:
        case enums::combination_t::two_pair:
        case enums::combination_t::flush: {
            return as_string() + ", kicker(s): " + str_kickers();
        }
        default: {
            return as_string();
        }
        }
    }

private:
    [[nodiscard]] auto str_kickers() const -> std::string {
        std::string str;
        uint16_t k = kickers();

        int8_t index = constants::RANKS;
        for (uint64_t mask = uint64_t(1) << constants::RANKS; mask; mask >>= 1, index--) {
            if (k & mask) {
                str += std::string{ rank(static_cast< enums::rank_t >(index)).as_string_long() } + " ";
            }
        }
        str.pop_back();

        return str;
    }

    uint32_t result_;
};
} // namespace pokerengine

#endif // POKERENGINE_RESULT_HPP
