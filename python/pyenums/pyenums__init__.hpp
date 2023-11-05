//
// Created by copper_boy on 11/1/23.
//

#ifndef POKERENGINE_PYENUMS__INIT___HPP
#define POKERENGINE_PYENUMS__INIT___HPP

#include "pyenums/pyenums.hpp"
#include "python.hpp"

namespace python {
auto setup_enums_all(py::module_ &module_) -> void {
    auto enums = module_.def_submodule("enums");

    setup_action_enum(enums);
    setup_combination_enum(enums);
    setup_position_enum(enums);
    setup_rank_enum(enums);
    setup_round_enum(enums);
    setup_state_enum(enums);
    setup_suit_enum(enums);
}

auto setup_pyenums_main(py::module_ &module_) -> void {
    setup_enums_all(module_);
}
} // namespace python

#endif // POKERENGINE_PYENUMS__INIT___HPP
