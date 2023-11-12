//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYEVALUATION_RESULT_HPP
#define POKERENGINE_PYEVALUATION_RESULT_HPP

#include <pybind11/stl.h>

#include "evaluator/evaluation_result.hpp"

#include "python.hpp"

namespace python {
auto setup_pyevaluation_result(py::module_ &module_) -> void {
    auto evaluation_result = module_.def_submodule("evaluation_result");

    evaluation_result.def(
                    "get_evaluation_result",
                    &pokerengine::get_evaluation_result,
                    py::arg("cards"),
                    py::arg("players"));
    evaluation_result.def(
                    "get_evaluation_result_one",
                    &pokerengine::get_evaluation_result_one,
                    py::arg("cards"),
                    py::arg("for_"));
}
} // namespace python

#endif // POKERENGINE_PYEVALUATION_RESULT_HPP
