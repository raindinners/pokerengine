//
// Created by copper_boy on 11/1/23.
//

#ifndef POKERENGINE_PYENUMS_HPP
#define POKERENGINE_PYENUMS_HPP

#include "card/card.hpp"
#include "engine/engine.hpp"
#include "engine/player.hpp"
#include "engine/round.hpp"
#include "evaluator/result.hpp"

#include "python.hpp"

namespace python {
auto setup_action_enum(py::module_ &module_) -> void {
    auto action = module_.def_submodule("action");

    py::enum_< pokerengine::enums::action_t >(action, "ActionEnum", py::module_local())
                    .value("FOLD", pokerengine::enums::action_t::fold)
                    .value("CHECK", pokerengine::enums::action_t::check)
                    .value("CALL", pokerengine::enums::action_t::call)
                    .value("BET", pokerengine::enums::action_t::bet)
                    .value("RAISE", pokerengine::enums::action_t::raise)
                    .value("ALLIN", pokerengine::enums::action_t::allin)
                    .value("NO_ACTION", pokerengine::enums::action_t::no_action);
}

auto setup_combination_enum(py::module_ &module_) -> void {
    auto combination = module_.def_submodule("combination");

    py::enum_< pokerengine::enums::combination_t >(combination, "CombinationEnum", py::module_local())
                    .value("NO_PAIR", pokerengine::enums::combination_t::no_pair)
                    .value("ONE_PAIR", pokerengine::enums::combination_t::one_pair)
                    .value("TWO_PAIR", pokerengine::enums::combination_t::two_pair)
                    .value("THREE_OF_A_KIND", pokerengine::enums::combination_t::three_of_a_kind)
                    .value("STRAIGHT", pokerengine::enums::combination_t::straight)
                    .value("FLUSH", pokerengine::enums::combination_t::flush)
                    .value("FULL_HOUSE", pokerengine::enums::combination_t::full_house)
                    .value("FOUR_OF_A_KIND", pokerengine::enums::combination_t::four_of_a_kind)
                    .value("STRAIGHT_FLUSH", pokerengine::enums::combination_t::straight_flush);
}

auto setup_position_enum(py::module_ &module_) -> void {
    auto position = module_.def_submodule("position");

    py::enum_< pokerengine::enums::position_t >(position, "PositionEnum", py::module_local())
                    .value("SB", pokerengine::enums::position_t::sb)
                    .value("BB", pokerengine::enums::position_t::bb)
                    .value("UTG", pokerengine::enums::position_t::utg)
                    .value("LWJ", pokerengine::enums::position_t::lwj)
                    .value("HIJ", pokerengine::enums::position_t::hij)
                    .value("COF", pokerengine::enums::position_t::cof)
                    .value("BTN", pokerengine::enums::position_t::btn)
                    .value("NO_POSITION", pokerengine::enums::position_t::no_position);
}

auto setup_rank_enum(py::module_ &module_) -> void {
    auto rank = module_.def_submodule("rank");

    py::enum_< pokerengine::enums::rank_t >(rank, "RankEnum", py::module_local())
                    .value("TWO", pokerengine::enums::rank_t::two)
                    .value("THREE", pokerengine::enums::rank_t::three)
                    .value("FOUR", pokerengine::enums::rank_t::four)
                    .value("FIVE", pokerengine::enums::rank_t::five)
                    .value("SIX", pokerengine::enums::rank_t::six)
                    .value("SEVEN", pokerengine::enums::rank_t::seven)
                    .value("EIGHT", pokerengine::enums::rank_t::eight)
                    .value("NINE", pokerengine::enums::rank_t::nine)
                    .value("TEN", pokerengine::enums::rank_t::ten)
                    .value("JACK", pokerengine::enums::rank_t::jack)
                    .value("QUEEN", pokerengine::enums::rank_t::queen)
                    .value("KING", pokerengine::enums::rank_t::king)
                    .value("ACE", pokerengine::enums::rank_t::ace);
}

auto setup_round_enum(py::module_ &module_) -> void {
    auto round = module_.def_submodule("round");

    py::enum_< pokerengine::enums::rounds_t >(round, "RoundEnum", py::module_local())
                    .value("PREFLOP", pokerengine::enums::rounds_t::preflop)
                    .value("FLOP", pokerengine::enums::rounds_t::flop)
                    .value("TURN", pokerengine::enums::rounds_t::turn)
                    .value("RIVER", pokerengine::enums::rounds_t::river)
                    .value("SHOWDOWN", pokerengine::enums::rounds_t::showdown);
}

auto setup_state_enum(py::module_ &module_) -> void {
    auto state = module_.def_submodule("state");

    py::enum_< pokerengine::enums::state_t >(state, "StateEnum", py::module_local())
                    .value("INIT", pokerengine::enums::state_t::init)
                    .value("OUT", pokerengine::enums::state_t::out)
                    .value("ALIVE", pokerengine::enums::state_t::alive)
                    .value("ALLIN", pokerengine::enums::state_t::allin);
}

auto setup_suit_enum(py::module_ &module_) -> void {
    auto suit = module_.def_submodule("suit");

    py::enum_< pokerengine::enums::suit_t >(suit, "SuitEnum", py::module_local())
                    .value("CLUBS", pokerengine::enums::suit_t ::clubs)
                    .value("DIAMONDS", pokerengine::enums::suit_t ::diamonds)
                    .value("HEARTS", pokerengine::enums::suit_t ::hearts)
                    .value("SPADES", pokerengine::enums::suit_t ::spades);
}
} // namespace python

#endif // POKERENGINE_PYENUMS_HPP
