//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYENGINE_HPP
#define POKERENGINE_PYENGINE_HPP

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "engine/engine.hpp"

#include "python.hpp"

namespace python {
auto setup_pyengine(py::module_ &module_) -> void {
    auto engine = module_.def_submodule("engine");

    py::class_< pokerengine::player_action >(engine, "PlayerAction", py::module_local())
                    .def(py::init< int32_t, pokerengine::enums::action_t, pokerengine::enums::position_t >(),
                         py::arg("amount"),
                         py::arg("amount"),
                         py::arg("position"))
                    .def(py::self == py::self, py::arg("other")) // NOLINT
                    .def_readwrite("amount", &pokerengine::player_action::amount)
                    .def_readwrite("action", &pokerengine::player_action::action)
                    .def_readwrite("position", &pokerengine::player_action::position);

    py::class_< pokerengine::engine_traits >(engine, "EngineTraits", py::module_local())
                    .def(py::init< int32_t, int32_t, uint8_t >(),
                         py::arg("sb_bet"),
                         py::arg("bb_bet"),
                         py::arg("bb_mult"))
                    .def_property("sb_bet",
                                  &pokerengine::engine_traits::get_sb_bet,
                                  &pokerengine::engine_traits::set_sb_bet)
                    .def_property("bb_bet",
                                  &pokerengine::engine_traits::get_bb_bet,
                                  &pokerengine::engine_traits::set_bb_bet)
                    .def_property("bb_mult",
                                  &pokerengine::engine_traits::get_bb_mult,
                                  &pokerengine::engine_traits::set_bb_mult)
                    .def_property("min_raise",
                                  &pokerengine::engine_traits::get_min_raise,
                                  &pokerengine::engine_traits::set_min_raise);

    py::class_< pokerengine::engine< 0, 1 > >(engine, "EngineRake01", py::module_local())
                    .def(py::init< const pokerengine::engine_traits & >(), py::arg("traits"))
                    .def("start", &pokerengine::engine< 0, 1 >::start, py::arg("new_game"))
                    .def("stop", &pokerengine::engine< 0, 1 >::stop)
                    .def("in_terminal_state", &pokerengine::engine< 0, 1 >::in_terminal_state)
                    .def("is_showdown", &pokerengine::engine< 0, 1 >::is_showdown)
                    .def("execute", &pokerengine::engine< 0, 1 >::execute_action, py::arg("action"))
                    .def_readonly("players_manager", &pokerengine::engine< 0, 1 >::players)
                    .def_readonly("round_manager", &pokerengine::engine< 0, 1 >::round)
                    .def_readonly("positions_manager", &pokerengine::engine< 0, 1 >::positions)
                    .def_readonly("pot_manager", &pokerengine::engine< 0, 1 >::pot)
                    .def_property_readonly("actions", &pokerengine::engine< 0, 1 >::get_possible_actions)
                    .def_property("traits",
                                  &pokerengine::engine< 0, 1 >::get_engine_traits,
                                  &pokerengine::engine< 0, 1 >::set_engine_traits);
}
} // namespace python

#endif // POKERENGINE_PYENGINE_HPP
