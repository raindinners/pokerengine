//
// Created by copper_boy on 10/29/23.
//

#ifndef POKERENGINE_UTILS_HPP
#define POKERENGINE_UTILS_HPP

#include <cstdint>

#include "pokerengine.hpp"

namespace pokerengine::utils {
template < class T >
class reference {
public:
    reference() = delete;

    explicit reference(T &value) : value_{ value } {
    }
    virtual ~reference() = default;

protected:
    T &value_;
};
} // namespace pokerengine::utils

#endif // POKERENGINE_UTILS_HPP
