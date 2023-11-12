//
// Created by copper_boy on 11/1/23.
//

#ifndef POKERENGINE_PYCARDS__INIT___HPP
#define POKERENGINE_PYCARDS__INIT___HPP

#include "pycard/pycard.hpp"
#include "pycard/pycards.hpp"
#include "pycard/pyhand.hpp"
#include "python.hpp"

namespace python {
auto setup_pycard_all(py::module_ &module_) -> void {
    auto card = module_.def_submodule("card");

    setup_pycard(card);
    setup_pycards(card);
    setup_pyhand(card);
}

auto setup_pycard_main(py::module_ &module_) -> void {
    setup_pycard_all(module_);
}
} // namespace python

#endif // POKERENGINE_PYCARDS__INIT___HPP
