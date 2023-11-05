//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYENGINE__INIT___HPP
#define POKERENGINE_PYENGINE__INIT___HPP

#include "pyengine/pyengine.hpp"
#include "pyengine/pyplayer.hpp"
#include "pyengine/pypositions.hpp"
#include "pyengine/pypot.hpp"
#include "pyengine/pyround.hpp"
#include "python.hpp"

namespace python {
auto setup_pyengine_all(py::module_ &module_) -> void {
    auto engine = module_.def_submodule("engine");

    setup_pyengine(engine);
    setup_pyplayer(engine);
    setup_pypositions(engine);
    setup_pypot(engine);
    setup_pyround(engine);
}

auto setup_pyengine_main(py::module_ &module_) -> void {
    setup_pyengine_all(module_);
}
} // namespace python

#endif // POKERENGINE_PYENGINE__INIT___HPP
