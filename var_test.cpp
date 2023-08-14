#include <iostream>
#include <array>
#include <tuple>

#include "var.hpp"
#include "type.hpp"
#include "string.hpp"

template<meta::string S>
struct var {};

int main() {
    using namespace meta::literals;

    std::cout << meta::type(1_i) << " is ";
    if constexpr (!meta::is_var(1_i))
        std::cout << "not ";
    std::cout << "a meta::var" << std::endl;

    constexpr auto res = (1_i < 2_d) && !(3_f > 4_ld);
    std::cout << meta::type(res) << "::value = " << res << std::endl; 
    
    constexpr auto res2 = meta::var<std::array{1, 2, 3, 4, 5}>{}[2_ull];
    std::cout << meta::type(res2) << "::value = " << res2 << std::endl;

    constexpr auto res3 = meta::var<[](auto x, auto y) { return x + y; }>{};
    std::cout << meta::type(res3) << "::value = " << res3(2_ld, 3_f) << std::endl;    

    constexpr auto res4 = meta::eval(meta::var<meta::var<1>{}>{});
    std::cout << meta::type(res4) << "::value = " << res4 << std::endl;

    return 0;
}
