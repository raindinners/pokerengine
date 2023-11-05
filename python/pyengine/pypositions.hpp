//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYPOSITIONS_HPP
#define POKERENGINE_PYPOSITIONS_HPP

#include "engine/positions.hpp"

#include "python.hpp"

namespace python {
auto setup_pypositions(py::module_ &module_) -> void {
    auto positions = module_.def_submodule("positions");

    py::class_< pokerengine::positions_manager >(positions, "Positions", py::module_local())
                    .def(py::init< pokerengine::players_set & >(), py::arg("players"))
                    .def("is_showdown", &pokerengine::positions_manager::is_showdown)
                    .def("set_next_current", &pokerengine::positions_manager::set_next_current)
                    .def("set_next_round_player", &pokerengine::positions_manager::set_next_round_player)
                    .def_property_readonly("number_alive", &pokerengine::positions_manager::get_number_alive)
                    .def_property_readonly(
                                    "player", py::overload_cast<>(&pokerengine::positions_manager::get_player))
                    .def_property_readonly(
                                    "player",
                                    py::overload_cast<>(&pokerengine::positions_manager::get_player, py::const_))
                    .def_property("current",
                                  &pokerengine::positions_manager::get_current,
                                  &pokerengine::positions_manager::set_current);
}
} // namespace python

#endif // POKERENGINE_PYPOSITIONS_HPP
