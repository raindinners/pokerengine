//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYCARDS_HPP
#define POKERENGINE_PYCARDS_HPP

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "card/cards.hpp"

#include "python.hpp"

namespace python {
auto setup_pycards(py::module_ &module_) -> void {
    auto cards = module_.def_submodule("cards");

    py::class_< pokerengine::cards >(cards, "Cards", py::module_local())
                    .def(py::init< const std::vector< std::string > &, const std::vector< std::string > & >(),
                         py::arg("board"),
                         py::arg("hands"))
                    .def(py::init< const std::vector< pokerengine::card > & >(), py::arg("cards"))
                    .def(py::self == py::self, py::arg("other")) // NOLINT
                    .def(py::self != py::self, py::arg("other")) // NOLINT
                    .def(py::self > py::self, py::arg("other"))  // NOLINT
                    .def(py::self < py::self, py::arg("other"))  // NOLINT
                    .def("board_n", &pokerengine::cards::get_board_n)
                    .def_property_readonly("board", &pokerengine::cards::get_board)
                    .def_property_readonly("hands", &pokerengine::cards::get_hands);
}
} // namespace python

#endif // POKERENGINE_PYCARDS_HPP
