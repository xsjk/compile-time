#include <iostream>
#include <array>

#include "compile_time_arithmetic.hpp"
#include "compile_time_typename.hpp"

int main() {
    using namespace meta::literals;

    std::cout << 3.14_f << std::endl;

    constexpr auto res = (1_i < 2_d) && !(3_f > 4_ld);
    std::cout << static_typename(res) << "::value = " << res << std::endl; 
    
    constexpr auto res2 = meta::var<std::array{1, 2, 3, 4, 5}>{}[2_ull];
    std::cout << static_typename(res2) << "::value = " << res2 << std::endl;

    constexpr auto res3 = meta::var<[](auto x, auto y) { return x + y; }>{};
    std::cout << static_typename(res3) << "::value = " << res3(2_ld, 3_f) << std::endl;    

    return 0;
}
