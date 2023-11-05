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
                    .def(py::init< pokerengine::enums::combination_t, uint8_t, uint8_t, uint16_t >(),
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
                    .def("as_bitset", &pokerengine::result::as_bitset)
                    .def("as_string", &pokerengine::result::as_string)
                    .def("as_string_long", &pokerengine::result::as_string_long)
                    .def_property_readonly("type", &pokerengine::result::type)
                    .def_property_readonly("minor_rank", &pokerengine::result::minor_rank)
                    .def_property_readonly("major_rank", &pokerengine::result::major_rank)
                    .def_property_readonly("kickers", &pokerengine::result::kickers)
                    .def_property_readonly("result", &pokerengine::result::get_result);
}
} // namespace python

#endif // POKERENGINE_PYRESULT_HPP
