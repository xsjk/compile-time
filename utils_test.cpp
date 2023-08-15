#include "utils.hpp"
#include "type.hpp"
#include <iostream>

int main() {
    using namespace meta::literals;

    constexpr auto res = meta::min(1, 2);
    constexpr auto res_meta = meta::min(1_i, 2_i);
    std::cout << meta::type(res) << '{' << res << '}' << std::endl;
    std::cout << meta::type(res_meta) << '{' << res_meta << '}' << std::endl;
}