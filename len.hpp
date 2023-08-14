#pragma once

namespace meta {

    template<typename T>
    consteval auto len(T) {
        return var<T{}.size()>{};
    }

}