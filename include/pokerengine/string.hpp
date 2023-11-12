//
// Created by copper_boy on 11/12/23.
//

#ifndef POKERENGINE_STRING_HPP
#define POKERENGINE_STRING_HPP

#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "pokerengine.hpp"

namespace pokerengine::string {
auto split(std::string_view string, std::string_view separator) -> std::vector< std::string_view > {
    std::vector< std::string_view > result;
    size_t from = 0, to = 0;
    while (std::string::npos != (to = string.find(separator, from))) {
        result.push_back(string.substr(from, to - from));
        from = to + separator.length();
    }
    result.push_back(string.substr(from, to));
    return result;
}

auto capitalize(std::string_view value) -> std::string {
    std::string result{ value };
    std::stringstream stream;
    stream << std::uppercase << value[0] << std::nouppercase << value.substr(1);
    stream >> result;

    return result;
}

template < typename T >
auto copy(const std::vector< T > &value) -> std::string {
    std::ostringstream stream;
    stream << "[";
    for (size_t index = 0; index < value.size() - 1; index++) {
        stream << std::string{ value[index] } << ", ";
    }
    stream << std::string{ value.back() };
    stream << "]";

    return stream.str();
}
} // namespace pokerengine::string

#endif // POKERENGINE_STRING_HPP
