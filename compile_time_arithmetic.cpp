#include <iostream>

#include "compile_time_arithmetic.hpp"
#include "compile_time_typename.hpp"

int main() {
    using namespace meta::literals;

    // constexpr auto compileTimeInt = 42i;
    // std::cout << compileTimeInt << std::endl;  // 输出 42

    constexpr auto myInt = 42_v;
    std::cout << myInt << std::endl;  // 输出 42



    // std::cout << meta::add<meta::type<int, 1>, meta::type<int, 2>>::value << std::endl;
    constexpr auto res = (meta::var<1>{} < meta::var<2.0>{});

    std::cout << static_typename(res) << std::endl;
    std::cout << res << std::endl;


    


    return 0;
}
