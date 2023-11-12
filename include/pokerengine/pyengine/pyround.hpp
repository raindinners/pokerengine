//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYROUND_HPP
#define POKERENGINE_PYROUND_HPP

#include "engine/round.hpp"

#include "python.hpp"

namespace python {
auto setup_pyround(py::module_ &module_) -> void {
    auto round = module_.def_submodule("round");

    py::class_< pokerengine::round_manager >(round, "Round", py::module_local())
                    .def(py::init< pokerengine::enums::round, bool >(), py::arg("round"), py::arg("flop_dealt"))
                    .def("reset", &pokerengine::round_manager::reset)
                    .def_property_readonly("showdown", &pokerengine::round_manager::is_showdown)
                    .def_property("round",
                                  &pokerengine::round_manager::get_round,
                                  &pokerengine::round_manager::set_round)
                    .def_property("flop_dealt",
                                  &pokerengine::round_manager::get_flop_dealt,
                                  &pokerengine::round_manager::set_flop_dealt);
}
} // namespace python

#endif // POKERENGINE_PYROUND_HPP
