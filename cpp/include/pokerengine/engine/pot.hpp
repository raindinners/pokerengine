//
// Created by copper_boy on 10/28/23.
//

#ifndef POKERENGINE_POT_HPP
#define POKERENGINE_POT_HPP

#include <cstdint>
#include <numeric>
#include <vector>

#include "card/cards.hpp"
#include "engine/player.hpp"
#include "engine/positions.hpp"
#include "evaluator/evaluation_result.hpp"
#include "pokerengine.hpp"
#include "vector.hpp"

namespace pokerengine {
namespace constants {
template < size_t A = 0, size_t B = 1 >
    requires(A >= 0 && B > 0 && A < B)
const float RAKE = 1.0f * A / B;
template < size_t A = 0, size_t B = 1 >
    requires(A >= 0 && B > 0 && A < B)
const float RAKE_MULTI = 1.0f - RAKE< A, B >;
} // namespace constants

namespace v1 {
auto get_chips_to_return(const std::vector< player > &players, int32_t highest_bet)
                -> std::pair< enums::position_t, int32_t > {
    if (std::count_if(players.cbegin(), players.cend(), [&](const auto &element) -> bool {
            return element.front == highest_bet;
        }) < 2) {
        std::vector< int32_t > chips_front;
        std::for_each(players.cbegin(), players.cend(), [&](const auto &element) -> void {
            chips_front.push_back(element.front);
        });


        auto position = std::distance(
                        players.cbegin(),
                        std::find_if(players.cbegin(), players.cend(), [&](const auto &element) -> bool {
                            return element.front == highest_bet;
                        }));

        std::sort(chips_front.begin(), chips_front.end(), std::greater{});
        return std::make_pair(enums::position_t(position), highest_bet - chips_front[1]);
    } else {
        return std::make_pair(enums::position_t{ 0 }, 0);
    }
}

auto get_chips_front(const std::vector< player > &players, int32_t highest_bet) -> std::vector< int32_t > {
    std::vector< int32_t > chips;
    for (const auto &player : players) {
        chips.push_back(player.front);
    }

    auto chips_return = get_chips_to_return(players, highest_bet);
    if (chips_return.second == 0) {
        return chips;
    }
    chips[static_cast< uint8_t >(chips_return.first)] -= chips_return.second;
    return chips;
}

auto get_all_pots(const std::vector< player > &players, int32_t highest_bet)
                -> std::vector< std::tuple< std::vector< uint8_t >, int32_t, int32_t > > {
    auto chips_front = get_chips_front(players, highest_bet);

    std::vector< std::pair< int32_t, uint8_t > > chips_and_players;
    for (size_t i = 0; i < chips_front.size(); i++) {
        chips_and_players.emplace_back(chips_front[i], i);
    }
    std::sort(chips_and_players.begin(),
              chips_and_players.end(),
              [](const auto &lhs, const auto &rhs) -> bool { return lhs.first > rhs.first; });

    std::vector< uint8_t > main_pot_players;
    std::vector< std::tuple< std::vector< uint8_t >, int32_t, int32_t > > pots;

    int32_t upper = chips_and_players[0].first;
    std::for_each(chips_and_players.cbegin(), chips_and_players.cend(), [&](const auto &pair) -> void {
        if (players[pair.second].state == enums::state_t::out) {
            return;
        } else if (int32_t lower = chips_front[pair.second]; lower == upper) {
            main_pot_players.push_back(pair.second);
        } else if (players[pair.second].state == enums::state_t::allin) {
            pots.emplace_back(main_pot_players, upper, lower);
            upper = lower;
            main_pot_players.push_back(pair.second);
        }
    });
    pots.emplace_back(main_pot_players, upper, 0);

    return pots;
}

template < uint8_t A = 0, uint8_t B = 1 >
    requires(A >= 0 && B > 0 && A < B)
auto get_adjust_pot(const std::vector< player > &players, int32_t highest_bet, bool flop_dealt) -> int32_t {
    std::vector< int32_t > chips_front;
    std::for_each(players.cbegin(), players.cend(), [&](auto const &element) {
        chips_front.push_back(element.front);
    });

    auto pot = std::reduce(chips_front.cbegin(), chips_front.cend());
    if (flop_dealt && constants::RAKE< A, B > != 0.0) {
        auto chips_returned = get_chips_to_return(players, highest_bet).second;
        return static_cast< int32_t >((pot - chips_returned) * constants::RAKE< A, B > + chips_returned);
    } else {
        return pot;
    }
}

auto adjust_side_pot(const std::vector< player > &players, int32_t upper_bound, int32_t lower_bound) noexcept
                -> std::vector< int32_t > {
    std::vector< int32_t > result;
    for (auto const &player : players) {
        auto chips = player.front;
        result.push_back(
                        chips <= lower_bound                ? 0 :
                                        chips > upper_bound ? upper_bound - lower_bound :
                                                              chips - lower_bound);
    }

    return result;
}

template < uint8_t A = 0, uint8_t B = 1 >
    requires(A >= 0 && B > 0 && A < B)
auto get_side_pot_redistribution(
                const players_set &ps,
                const cards &cards,
                const std::vector< uint8_t > &players,
                bool flop_dealt,
                int32_t upper_bound,
                int32_t lower_bound) -> std::vector< int32_t > {
    auto iterable = ps.get_players();
    auto winners = get_evaluation_result(cards, players);
    auto chips_adjusted = adjust_side_pot(iterable, upper_bound, lower_bound);

    auto total_pot = static_cast< int32_t >(
                    std::accumulate(chips_adjusted.cbegin(), chips_adjusted.cend(), 0) *
                    (flop_dealt ? constants::RAKE_MULTI< A, B > : 1.0f));
    int32_t amount_each_winner = total_pot / static_cast< int32_t >(winners.size());

    std::vector< int32_t > result;
    for (size_t index = 0; index < iterable.size(); index++) {
        auto winner = std::find_if(winners.cbegin(), winners.cend(), [&](const auto &element) {
            return element.second == index;
        });
        if (winner != winners.cend()) {
            result.push_back(-chips_adjusted[index] + amount_each_winner);
        } else {
            result.push_back(-chips_adjusted[index]);
        }
    }

    return result;
}
} // namespace v1

template < uint8_t A = 0, uint8_t B = 1 >
    requires(A >= 0 && B > 0 && A < B)
class pot_manager : public utils::reference< players_set > {
public:
    using utils::reference< players_set >::reference;

    [[nodiscard]] auto get_highest_bet() const -> int32_t {
        auto iterable = value_.get_players();
        return std::max_element(iterable.cbegin(), iterable.cend(), [](const auto &lhs, const auto &rhs) -> bool {
                   return lhs.front < rhs.front;
               })->front;
    }

    [[nodiscard]] auto get_round_highest_bet() const -> int32_t {
        auto iterable = value_.get_players();
        return std::max_element(iterable.cbegin(), iterable.cend(), [](const auto &lhs, const auto &rhs) -> bool {
                   return lhs.round_bet < rhs.round_bet;
               })->round_bet;
    }

    [[nodiscard]] auto pot(bool flop_dealt) const noexcept -> int32_t {
        auto iterable = value_.get_players();
        std::vector< int32_t > chips_front;
        for (auto const &player : iterable) {
            chips_front.push_back(player.front);
        }

        auto reduce = std::reduce(chips_front.cbegin(), chips_front.cend());
        return flop_dealt ? reduce - actual::get_chips_to_return(iterable, get_highest_bet()).second : reduce;
    }

    [[nodiscard]] auto pot_rake(bool flop_dealt) noexcept -> int32_t {
        return flop_dealt ? static_cast< int32_t >(pot(flop_dealt) * constants::RAKE_MULTI< A, B >) :
                            pot(flop_dealt);
    }

    [[maybe_unused]] auto pay(const cards &cards, bool flop_dealt)
                    -> std::vector< std::pair< result, int32_t > > {
        auto iterable = value_.get_players();
        auto pots = actual::get_all_pots(iterable, get_highest_bet());
        auto chips = std::accumulate(
                        pots.cbegin(),
                        pots.cend(),
                        std::vector< int32_t >{},
                        [&](auto value, const auto &element) {
                            return value +
                                            actual::get_side_pot_redistribution(
                                                            value_,
                                                            cards,
                                                            std::get< 0 >(element),
                                                            flop_dealt,
                                                            std::get< 1 >(element),
                                                            std::get< 2 >(element));
                        });

        std::vector< std::pair< result, int32_t > > results;
        std::for_each(iterable.begin(), iterable.end(), [&, index = 0](auto &player) mutable -> void {
            auto result = results.emplace_back(get_evaluation_result_one(cards, index), chips[index++]);
            player.stack += result.second;
        });
        value_.set_players(iterable);

        return results;
    }

    [[maybe_unused]] auto pay_noshowdown(bool flop_dealt) -> std::vector< int32_t > {
        auto iterable = value_.get_players();
        auto adjusted_pot = actual::get_adjust_pot< A, B >(iterable, get_highest_bet(), flop_dealt);
        auto winner = std::distance(
                        iterable.cbegin(),
                        std::find_if(iterable.cbegin(), iterable.cend(), [](const auto &element) {
                            return element.state != enums::state_t::out;
                        }));

        std::vector< int32_t > results;
        std::for_each(iterable.begin(), iterable.end(), [&, index = 0](auto &player) mutable -> void {
            auto result = results.emplace_back(
                            index++ == winner ? -player.front + adjusted_pot : -player.front);
            player.stack += result;
        });
        value_.set_players(iterable);

        return results;
    }
};
} // namespace pokerengine

#endif // POKERENGINE_POT_HPP
