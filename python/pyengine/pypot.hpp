//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYPOT_HPP
#define POKERENGINE_PYPOT_HPP

#include <pybind11/stl.h>

#include "engine/pot.hpp"

#include "python.hpp"

namespace python {
auto setup_pypot(py::module_ &module_) -> void {
    auto pot = module_.def_submodule("pot");

    py::class_< pokerengine::pot_manager< 0, 1 > >(pot, "PotRake01", py::module_local())
                    .def(py::init< pokerengine::players_set & >(), py::arg("players"))
                    .def("pay", &pokerengine::pot_manager< 0, 1 >::pay, py::arg("cards"), py::arg("flop_dealt"))
                    .def("pay_noshowdown",
                         &pokerengine::pot_manager< 0, 1 >::pay_noshowdown,
                         py::arg("flop_dealt"))
                    .def("pot", &pokerengine::pot_manager< 0, 1 >::pot, py::arg("flop_dealt"))
                    .def("pot_rake", &pokerengine::pot_manager< 0, 1 >::pot_rake, py::arg("flop_dealt"))
                    .def_property_readonly("highest_bet", &pokerengine::pot_manager< 0, 1 >::get_highest_bet)
                    .def_property_readonly(
                                    "round_highest_bet",
                                    &pokerengine::pot_manager< 0, 1 >::get_round_highest_bet);
}
} // namespace python

#endif // POKERENGINE_PYPOT_HPP
