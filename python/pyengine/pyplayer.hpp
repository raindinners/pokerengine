//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYPLAYER_HPP
#define POKERENGINE_PYPLAYER_HPP

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "engine/player.hpp"

#include "python.hpp"

namespace python {
auto setup_pyplayer(py::module_ &module_) -> void {
    auto player = module_.def_submodule("player");

    py::class_< pokerengine::player >(player, "Player", py::module_local())
                    .def(py::init< bool, int32_t, int32_t, int32_t, int32_t, pokerengine::enums::state_t >(),
                         py::arg("is_left"),
                         py::arg("stack"),
                         py::arg("behind"),
                         py::arg("front"),
                         py::arg("round_bet"),
                         py::arg("state"))
                    .def(py::self == py::self, py::arg("other")) // NOLINT
                    .def_readwrite("is_left", &pokerengine::player::is_left)
                    .def_readwrite("stack", &pokerengine::player::stack)
                    .def_readwrite("behind", &pokerengine::player::behind)
                    .def_readwrite("front", &pokerengine::player::front)
                    .def_readwrite("round_bet", &pokerengine::player::round_bet)
                    .def_readwrite("state", &pokerengine::player::state);
    py::class_< pokerengine::players_set >(player, "Players", py::module_local())
                    .def(py::init())
                    .def("get_player", &pokerengine::players_set::get_player, py::arg("index"))
                    .def("set_players", &pokerengine::players_set::set_players, py::arg("players"))
                    .def("add_player",
                         &pokerengine::players_set::add_player,
                         py::arg("stack"),
                         py::arg("state") = pokerengine::enums::state_t::init)
                    .def("remove_player", &pokerengine::players_set::remove_player, py::arg("index"))
                    .def_property_readonly("players", &pokerengine::players_set::get_players);
}
} // namespace python

#endif // POKERENGINE_PYPLAYER_HPP
