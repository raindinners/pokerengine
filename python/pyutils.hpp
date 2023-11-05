//
// Created by copper_boy on 11/2/23.
//

#ifndef POKERENGINE_PYUTILS_HPP
#define POKERENGINE_PYUTILS_HPP

#include "engine/player.hpp"
#include "utils.hpp"

#include "python.hpp"

namespace python {
auto setup_pyutils_all(py::module_ &module_) -> void {
    auto utils = module_.def_submodule("utils");

    py::class_< pokerengine::utils::reference< pokerengine::players_set > >(
                    utils, "Reference", py::module_local());
}

auto setup_pyutils_main(py::module_ &module_) -> void {
    setup_pyutils_all(module_);
}
} // namespace python


#endif // POKERENGINE_PYUTILS_HPP
