//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYRESULT_HPP
#define POKERENGINE_PYRESULT_HPP

#include <pybind11/operators.h>

#include "evaluator/result.hpp"

#include "python.hpp"

namespace python {
auto setup_pyresult(py::module_ &module_) -> void {
    auto result = module_.def_submodule("result");

    py::class_< pokerengine::result >(result, "Result", py::module_local())
                    .def(py::init< pokerengine::enums::combination, uint8_t, uint8_t, uint16_t >(),
                         py::arg("type"),
                         py::arg("major"),
                         py::arg("minor"),
                         py::arg("kickers"))
                    .def(py::init< uint8_t, uint8_t, uint8_t, uint16_t >(),
                         py::arg("type"),
                         py::arg("major"),
                         py::arg("minor"),
                         py::arg("kickers"))
                    .def(py::self == py::self, py::arg("other")) // NOLINT
                    .def(py::self != py::self, py::arg("other")) // NOLINT
                    .def(py::self > py::self, py::arg("other"))  // NOLINT
                    .def(py::self < py::self, py::arg("other"))  // NOLINT
                    .def("__str__",
                         [](pokerengine::result &self) -> std::string { return std::string{ self }; })
                    .def("__int__",
                         [](pokerengine::result &self) -> pokerengine::types::bit_set {
                             return self.as_bitset();
                         })
                    .def_property_readonly("type", &pokerengine::result::get_type)
                    .def_property_readonly("minor_rank", &pokerengine::result::get_minor_rank)
                    .def_property_readonly("major_rank", &pokerengine::result::get_major_rank)
                    .def_property_readonly("kickers", &pokerengine::result::get_kickers)
                    .def_property_readonly("result", &pokerengine::result::get_result);
}
} // namespace python

#endif // POKERENGINE_PYRESULT_HPP
