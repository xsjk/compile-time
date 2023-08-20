#pragma once

#include "var.hpp"

namespace meta {

    template<typename T>
    consteval auto len(T) {
        return var<eval(T{}.size())>{};
    }

    constexpr auto min(auto a, auto b) {
        return a < b ? a : b;
    }

    template<auto A, auto B>
    consteval auto min(var<A>, var<B>) {
        return var<min(A, B)>{};
    }
    
    constexpr auto max(auto a, auto b) {
        return a > b ? a : b;
    }

    template<auto A, auto B>
    consteval auto max(var<A>, var<B>) {
        return var<max(A, B)>{};
    }

}