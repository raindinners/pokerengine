//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYHAND_HPP
#define POKERENGINE_PYHAND_HPP

#include <pybind11/operators.h>

#include "card/hand.hpp"

#include "python.hpp"

namespace python {
auto setup_pyhand(py::module_ &module_) -> void {
    auto hand = module_.def_submodule("hand");

    py::class_< pokerengine::hand >(hand, "Hand", py::module_local())
                    .def(py::init< pokerengine::card, pokerengine::card >())
                    .def(py::init< uint8_t, uint8_t >())
                    .def(py::init< std::string_view >())
                    .def(py::self == py::self, py::arg("other")) // NOLINT
                    .def(py::self != py::self, py::arg("other")) // NOLINT
                    .def(py::self > py::self, py::arg("other"))  // NOLINT
                    .def(py::self < py::self, py::arg("other"))  // NOLINT
                    .def("as_string", &pokerengine::hand::as_string)
                    .def("as_bitset", &pokerengine::hand::as_bitset)
                    .def("as_cardset", &pokerengine::hand::as_cardset)
                    .def_property_readonly("value", &pokerengine::hand::get_value);
}
} // namespace python

#endif // POKERENGINE_PYHAND_HPP
