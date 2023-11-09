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
#include "constants.hpp"
#include "enums.hpp"
#include "pokerengine.hpp"

namespace pokerengine {
class result {
public:
    result(enums::combination type, uint8_t major, uint8_t minor, uint16_t kickers) noexcept
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
        return rank{ enums::rank(
                        constants::MASK_RANKS_NUMBER & ((get_result() >> constants::OFFSET_MAJOR) - 0)) };
    }

    [[nodiscard]] auto minor_rank() const noexcept -> rank {
        return rank{ enums::rank(
                        constants::MASK_RANKS_NUMBER & ((get_result() >> constants::OFFSET_MINOR) - 0)) };
    }

    [[nodiscard]] auto kickers() const noexcept -> uint32_t {
        return (constants::MASK_RANKS & get_result());
    }

    [[nodiscard]] auto as_bitset() const noexcept -> uint32_t {
        return get_result();
    }

    [[nodiscard]] auto as_string() const -> std::string {
        auto result = enums::combination{ type() };
        auto index = static_cast< uint8_t >(result);

        switch (result) {
        case enums::combination::no_pair: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            std::string{
                                rank{ enums::rank{ bits::cross_idx_high16(kickers()) } }.as_string_long()
                            };
        }
        case enums::combination::one_pair:
        case enums::combination::three_of_a_kind: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long();
        }
        case enums::combination::four_of_a_kind: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long();
        }
        case enums::combination::two_pair: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long() + std::string{ " and " } +
                            minor_rank().as_string_long();
        }
        case enums::combination::full_house: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            major_rank().as_string_long() + std::string{ " full of " } +
                            minor_rank().as_string_long();
        }
        case enums::combination::straight:
        case enums::combination::straight_flush: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            std::string{ major_rank().as_string_long() } + std::string{ " high" };
        }
        case enums::combination::flush: {
            return std::string{ constants::COMBINATIONS[index] } + std::string{ " " } +
                            std::string{
                                rank{ enums::rank{ bits::cross_idx_high16(kickers()) } }.as_string_long()
                            } +
                            std::string{ " high" };
        }
        default: {
            throw std::runtime_error{ "Invalid evaluation result" };
        }
        }
    }

    [[nodiscard, maybe_unused]] auto as_string_long() const -> std::string {
        auto result = enums::combination(type());

        switch (result) {
        case enums::combination::no_pair:
        case enums::combination::one_pair:
        case enums::combination::three_of_a_kind:
        case enums::combination::four_of_a_kind:
        case enums::combination::two_pair:
        case enums::combination::flush: {
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
                str += std::string{ rank(static_cast< enums::rank >(index)).as_string_long() } + " ";
            }
        }
        str.pop_back();

        return str;
    }

    uint32_t result_;
};
} // namespace pokerengine

#endif // POKERENGINE_RESULT_HPP
