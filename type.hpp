#pragma once

#include "string.hpp"

namespace meta {
    template <typename T>
    constexpr auto type(const T&) {
        using namespace literals;
        #if defined(__clang__) || defined(__GNUC__)
            constexpr auto prefix = "T = "_s;
            constexpr auto suffix = "]"_s;
            constexpr var<string(__PRETTY_FUNCTION__)> function;
        #elif defined(__MSC_VER)
            constexpr auto prefix = "get_type_name<"_s;
            constexpr auto suffix = ">(void)"_s;
            constexpr var<string(__FUNCSIG__)> function;
        #else
            #error Unsupported compiler
        #endif
        constexpr auto start = function.find(prefix) + prefix.size();
        return function.substr(start, function.find_last_of(suffix) - start);
    }
}
