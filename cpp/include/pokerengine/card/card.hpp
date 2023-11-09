//
// Created by copper_boy on 7/4/23.
//

#ifndef POKERENGINE_CARD_HPP
#define POKERENGINE_CARD_HPP

#include <algorithm>
#include <compare>
#include <cstdint>
#include <limits>
#include <random>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "constants.hpp"
#include "enums.hpp"
#include "pokerengine.hpp"
#include "types.hpp"

namespace pokerengine {
template < typename T >
concept BitSetType = std::numeric_limits< T >::is_integer;

namespace abc {
template < typename T >
concept ValidEnum = std::is_enum< T >::value;

template < ValidEnum EnumType, typename ValueType, BitSetType AsBitSetType >
class representation_abc {
public:
    explicit representation_abc< EnumType, ValueType, AsBitSetType >(EnumType value)
            : value_{ from_enum(value) } {
    }

    virtual ~representation_abc() = default;

    auto operator<=>(const representation_abc &other) const noexcept -> std::strong_ordering = default;

    [[nodiscard]] auto as_string() const noexcept -> std::string {
        return std::string{ 1, to_char() };
    }

    [[nodiscard, maybe_unused]] virtual auto as_string_long() const -> std::string = 0;

    [[nodiscard]] auto as_bitset() const noexcept -> AsBitSetType {
        return AsBitSetType{ 1 } << get_value();
    }

    [[nodiscard]] auto get_value() const noexcept -> ValueType {
        return value_;
    }

protected:
    [[nodiscard]] virtual auto from_char(char value) -> EnumType {
        throw std::runtime_error{ "Got unexpected char symbol" };
    }

    [[nodiscard]] virtual auto to_char() const -> char {
        throw std::runtime_error{ "Got unexpected enum value" };
    }

private:
    virtual auto from_enum(EnumType value) -> ValueType {
        return static_cast< ValueType >(value);
    }

    ValueType value_;
};
} // namespace abc

using rank_abc = abc::representation_abc< enums::rank, uint8_t, uint16_t >;
class rank : public rank_abc {
public:
    using abc::representation_abc< enums::rank, uint8_t, uint16_t >::representation_abc;

    explicit rank(char value)
            : abc::representation_abc< enums::rank, uint8_t, uint16_t >{ from_char(value) } {
    }

    [[nodiscard]] auto as_string_long() const -> std::string override {
        auto value = static_cast< enums::rank >(get_value());

        switch (value) {
        case enums::rank::two: {
            return "Two";
        }
        case enums::rank::three: {
            return "Three";
        }
        case enums::rank::four: {
            return "Four";
        }
        case enums::rank::five: {
            return "Five";
        }
        case enums::rank::six: {
            return "Six";
        }
        case enums::rank::seven: {
            return "Seven";
        }
        case enums::rank::eight: {
            return "Eight";
        }
        case enums::rank::nine: {
            return "Nine";
        }
        case enums::rank::ten: {
            return "Ten";
        }
        case enums::rank::jack: {
            return "Jack";
        }
        case enums::rank::queen: {
            return "Queen";
        }
        case enums::rank::king: {
            return "King";
        }
        case enums::rank::ace: {
            return "Ace";
        }
        default: {
            throw std::runtime_error{ "Got unexpected enum value" };
        }
        }
    }

protected:
    [[nodiscard]] auto from_char(char value) -> enums::rank override {
        switch (value) {
        case '2': {
            return enums::rank::two;
        }
        case '3': {
            return enums::rank::three;
        }
        case '4': {
            return enums::rank::four;
        }
        case '5': {
            return enums::rank::five;
        }
        case '6': {
            return enums::rank::six;
        }
        case '7': {
            return enums::rank::seven;
        }
        case '8': {
            return enums::rank::eight;
        }
        case '9': {
            return enums::rank::nine;
        }
        case 'T':
        case 't': {
            return enums::rank::ten;
        }
        case 'J':
        case 'j': {
            return enums::rank::jack;
        }
        case 'Q':
        case 'q': {
            return enums::rank::queen;
        }
        case 'K':
        case 'k': {
            return enums::rank::king;
        }
        case 'A':
        case 'a': {
            return enums::rank::ace;
        }
        default: {
            return abc::representation_abc< enums::rank, uint8_t, uint16_t >::from_char(value);
        }
        }
    }

    [[nodiscard]] auto to_char() const -> char override {
        auto value = static_cast< enums::rank >(get_value());

        switch (value) {
        case enums::rank::two: {
            return '2';
        }
        case enums::rank::three: {
            return '3';
        }
        case enums::rank::four: {
            return '4';
        }
        case enums::rank::five: {
            return '5';
        }
        case enums::rank::six: {
            return '6';
        }
        case enums::rank::seven: {
            return '7';
        }
        case enums::rank::eight: {
            return '8';
        }
        case enums::rank::nine: {
            return '9';
        }
        case enums::rank::ten: {
            return 'T';
        }
        case enums::rank::jack: {
            return 'J';
        }
        case enums::rank::queen: {
            return 'Q';
        }
        case enums::rank::king: {
            return 'K';
        }
        case enums::rank::ace: {
            return 'A';
        }
        default: {
            return abc::representation_abc< enums::rank, uint8_t, uint16_t >::to_char();
        }
        }
    }
};

using suit_abc = abc::representation_abc< enums::suit, uint8_t, uint16_t >;
class suit : public suit_abc {
public:
    using abc::representation_abc< enums::suit, uint8_t, uint16_t >::representation_abc;

    explicit suit(char value)
            : abc::representation_abc< enums::suit, uint8_t, uint16_t >{ from_char(value) } {
    }

    [[nodiscard, maybe_unused]] auto as_string_pretty() const -> std::string {
        auto value = static_cast< enums::suit >(get_value());

        switch (value) {
        case enums::suit::clubs: {
            return "♣";
        }
        case enums::suit::diamonds: {
            return "♦️";
        }
        case enums::suit::hearts: {
            return "❤️";
        }
        case enums::suit::spades: {
            return "♠️";
        }
        default: {
            throw std::runtime_error{ "Got unexpected enum value" };
        }
        }
    }

    [[nodiscard]] auto as_string_long() const -> std::string override {
        auto value = static_cast< enums::suit >(get_value());

        switch (value) {
        case enums::suit::clubs: {
            return "Clubs";
        }
        case enums::suit::diamonds: {
            return "Diamonds";
        }
        case enums::suit::hearts: {
            return "Hearts";
        }
        case enums::suit::spades: {
            return "Spades";
        }
        default: {
            throw std::runtime_error{ "Got unexpected enum value" };
        }
        }
    }

protected:
    [[nodiscard]] auto from_char(char value) -> enums::suit override {
        switch (value) {
        case 'C':
        case 'c': {
            return enums::suit::clubs;
        }
        case 'D':
        case 'd': {
            return enums::suit::diamonds;
        }
        case 'H':
        case 'h': {
            return enums::suit::hearts;
        }
        case 'S':
        case 's': {
            return enums::suit::spades;
        }
        default: {
            return abc::representation_abc< enums::suit, uint8_t, uint16_t >::from_char(value);
        }
        };
    }

    [[nodiscard]] auto to_char() const -> char override {
        auto value = static_cast< enums::suit >(get_value());

        switch (value) {
        case enums::suit::clubs: {
            return 'c';
        }
        case enums::suit::diamonds: {
            return 'd';
        }
        case enums::suit::hearts: {
            return 'h';
        }
        case enums::suit::spades: {
            return 's';
        }
        default: {
            return abc::representation_abc< enums::suit, uint8_t, uint16_t >::to_char();
        }
        }
    }
};

class card {
public:
    card() = delete;

    card(const rank &rank_value, const suit &suit_value) noexcept
            : card_{ static_cast< uint8_t >(
                              rank_value.get_value() + (suit_value.get_value() * constants::RANKS)) } {
    }

    card(enums::rank rank_value, enums::suit suit_value) : card{ rank{ rank_value }, suit{ suit_value } } {
    }

    explicit card(uint8_t value) : card_{ value } {
        if (value != std::clamp(value, constants::CARD_INDEX_MIN, constants::CARD_INDEX_MAX)) {
            throw std::runtime_error{ "Invalid argument value for card" };
        }
    }

    explicit card(std::string_view value) : card{ rank{ value[0] }, suit{ value[1] } } {
    }

    auto operator<=>(const card &other) const noexcept -> std::strong_ordering {
        std::strong_ordering result = get_card() % constants::RANKS <=> other.get_card() % constants::RANKS;
        if (result != std::strong_ordering::equal) {
            return result;
        }

        return (get_card() / constants::SUITS) <=> (other.get_card() / constants::SUITS);
    }

    auto operator==(const card &other) const noexcept -> bool {
        return get_card() == other.get_card();
    }

    [[nodiscard]] auto as_bitset() const noexcept -> types::bit_set {
        return types::bit_set{ 1 } << card_;
    }

    [[nodiscard]] auto as_string() const noexcept -> std::string {
        return std::string{ constants::CARDS_STRING.substr(static_cast< size_t >(card_) * 2, 2) };
    }

    [[nodiscard]] auto get_card() const noexcept -> uint8_t {
        return card_;
    }

    [[nodiscard, maybe_unused]] auto get_rank() const noexcept -> rank {
        return rank{ enums::rank(uint8_t(card_ % constants::RANKS)) };
    }

    [[nodiscard, maybe_unused]] auto get_suit() const noexcept -> suit {
        return suit{ enums::suit(uint8_t(card_ / constants::RANKS)) };
    }

private:
    uint8_t card_;
};

class card_set {
public:
    card_set() = default;

    card_set(const std::initializer_list< const card > &values) {
        for (auto const &value : values) {
            cards_ |= value.as_bitset();
        }
    }

    explicit card_set(uint64_t bitset) : cards_{ bitset } {
        if (cards_ > constants::CARD_SET_FULL) {
            throw std::runtime_error{ "Card size reached" };
        }
    }

    explicit card_set(const std::span< const card > &sp) noexcept {
        for (auto &&c : sp) {
            cards_ |= c.as_bitset();
        }
    }

    explicit card_set(std::string_view value) {
        if (value.size() % 2 != 0) {
            throw std::runtime_error{ "String size must be divisible by 2" };
        }

        for (size_t index = 0; index < value.size(); index += 2) {
            cards_ |= card(value.substr(index, 2)).as_bitset();
        }
    }

    [[nodiscard, maybe_unused]] auto size() const noexcept -> size_t {
        return std::popcount(get_cards());
    }

    [[nodiscard, maybe_unused]] auto contains(const card &value) const noexcept -> bool {
        return (get_cards() & value.as_bitset()) != 0;
    }

    [[nodiscard, maybe_unused]] auto contains(const card_set &value) const noexcept -> bool {
        return (get_cards() | value.get_cards()) == get_cards();
    }

    [[nodiscard]] auto combine(const card &value) const noexcept -> card_set {
        return card_set{ get_cards() | value.as_bitset() };
    }

    [[nodiscard]] auto combine(const card_set &value) const noexcept -> card_set {
        return card_set{ get_cards() | value.get_cards() };
    }

    [[maybe_unused]] auto clear() noexcept -> void {
        set_cards(0);
    }

    [[maybe_unused]] auto fill() noexcept -> void {
        set_cards(constants::CARD_SET_FULL);
    }

    [[maybe_unused]] auto insert(const card &value) noexcept -> void {
        set_cards(get_cards() | value.as_bitset());
    }

    [[maybe_unused]] auto join(const card_set &value) noexcept -> void {
        set_cards(get_cards() | value.get_cards());
    }

    [[maybe_unused]] auto remove(const card &value) noexcept -> void {
        set_cards(get_cards() ^ value.as_bitset());
    }

    [[maybe_unused]] auto remove(const card_set value) noexcept -> void {
        set_cards(get_cards() ^ value.get_cards());
    }

    [[nodiscard]] auto get_cards() const noexcept -> uint64_t {
        return cards_;
    }

    auto set_cards(uint64_t value) noexcept -> void {
        cards_ = value;
    }

private:
    uint64_t cards_ = 0;
};

class card_generator {
public:
    explicit card_generator(uint16_t seed = 1927)
            : rng_(seed), dist_rank_(0, constants::RANKS), dist_suit_(0, constants::SUITS) {
    }

    [[nodiscard]] auto generate() -> card {
        return card{ rank{ enums::rank{ uint8_t(dist_rank_(rng_)) } },
                     suit{ enums::suit{ static_cast< uint8_t >(dist_suit_(rng_)) } } };
    }

    [[nodiscard]] auto generate_v(uint8_t n) -> std::vector< card > {
        if (n > ((constants::RANKS) * (constants::SUITS))) {
            throw std::runtime_error("N greater than number of specified unique cards");
        }

        std::vector< uint8_t > cards;
        cards.reserve((constants::RANKS) * (constants::SUITS));
        for (uint8_t i = 0; i <= constants::RANKS - 1; ++i) {
            for (uint8_t j = 0; j <= constants::SUITS - 1; ++j) {
                cards.emplace_back(static_cast< uint8_t >(i + j * constants::RANKS));
            }
        }
        std::shuffle(cards.begin(), cards.end(), rng_);
        cards.resize(n);

        std::vector< card > result;
        result.reserve(n);

        std::for_each(cards.cbegin(), cards.cend(), [&](uint8_t index) -> void { result.emplace_back(index); });
        return result;
    }

private:
    std::mt19937 rng_;
    std::uniform_int_distribution< int > dist_rank_;
    std::uniform_int_distribution< int > dist_suit_;
};
} // namespace pokerengine

#endif // POKERENGINE_CARD_HPP
