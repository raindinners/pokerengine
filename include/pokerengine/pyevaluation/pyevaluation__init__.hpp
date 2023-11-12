//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYEVALUATION__INIT___HPP
#define POKERENGINE_PYEVALUATION__INIT___HPP

#include "pyevaluation/pyevaluation_result.hpp"
#include "pyevaluation/pyresult.hpp"
#include "python.hpp"

namespace python {
auto setup_pyevaluation_all(py::module_ &module_) -> void {
    auto evaluation = module_.def_submodule("evaluation");

    setup_pyevaluation_result(evaluation);
    setup_pyresult(evaluation);
}

auto setup_pyevaluation_main(py::module_ &module_) -> void {
    setup_pyevaluation_all(module_);
}
} // namespace python

#endif // POKERENGINE_PYEVALUATION__INIT___HPP
