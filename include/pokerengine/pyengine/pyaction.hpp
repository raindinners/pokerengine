//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYACTION_HPP
#define POKERENGINE_PYACTION_HPP

#include <pybind11/operators.h>

#include "engine/action.hpp"

#include "python.hpp"

namespace python {
auto setup_pyaction(py::module_ &module_) -> void {
    auto action = module_.def_submodule("action");

    py::class_< pokerengine::player_action >(action, "PlayerAction", py::module_local())
                    .def(py::init< int32_t, pokerengine::enums::action, pokerengine::enums::position >(),
                         py::arg("amount"),
                         py::arg("amount"),
                         py::arg("position"))
                    .def(py::self == py::self, py::arg("other")) // NOLINT
                    .def("__str__",
                         [](pokerengine::player_action &self) -> std::string { return std::string{ self }; })
                    .def_readwrite("amount", &pokerengine::player_action::amount)
                    .def_readwrite("action", &pokerengine::player_action::action)
                    .def_readwrite("position", &pokerengine::player_action::position);
}
} // namespace python

#endif // POKERENGINE_PYACTION_HPP
