//
// Created by copper_boy on 10/29/23.
//

#include <pybind11/operators.h>

#include "card/card.hpp"

#include "pycard/pycard__init__.hpp"
#include "pyengine/pyengine__init__.hpp"
#include "pyenums/pyenums__init__.hpp"
#include "pyevaluation/pyevaluation__init__.hpp"
#include "pyutils.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace python {
auto setup_all(py::module_ &module_) -> void {
    setup_pyenums_main(module_);
    setup_pycard_main(module_);
    setup_pyevaluation_main(module_);
    setup_pyengine_main(module_);
    setup_pyutils_main(module_);
}

auto setup_main(py::module_ &module_) -> void {
    setup_all(module_);
}
} // namespace python

PYBIND11_MODULE(pokerengine_core, module_) {
    module_.doc() = "Poker Library";
    python::setup_main(module_);

#ifdef VERSION_INFO
    module_.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    module_.attr("__version__") = "dev";
#endif
}
