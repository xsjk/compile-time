#include <iostream>
#include <type_traits>
#include <string>
#include <string_view>
#include <tuple>
#include <array>
#include <ranges>
#include <algorithm>
#include "compile_time_typename.hpp"
#include "compile_time_string.hpp"



int main() {

    using namespace std::string_view_literals;
    using namespace meta::literals;
    using namespace std::string_literals;

    constexpr auto s9 = "Hello"_s;
    constexpr auto s10 = "World"_s;


    std::cout << std::boolalpha;
    std::cout << s9.size() << s9 << std::endl;
    std::cout << s9[0_i] << std::endl;
    std::cout << s9.substr(1_i, 3_i) << std::endl;
    std::cout << s9 + s10 << std::endl;
    std::cout << 2_i * s9  << std::endl;
    std::cout << s9.compare(s10) << std::endl;
    std::cout << (s9 < s10) << std::endl;
    std::cout << s9.find_first_not_of("ll"_s) << std::endl;
    std::cout << s9.replace("l"_s, "L"_s) << std::endl;
    std::cout << (" "_s + s9 + " "_s).rstrip() + "|"_s << std::endl;

    constexpr auto d = s9.data();

    std::cout << type(d) << ' ' << d << std::endl;
    


    // constexpr auto l1 = meta::length('a');
    // constexpr auto l2 = meta::length("Hello");
    // constexpr auto l3 = meta::length("Hello"sv);
    // constexpr auto l4 = meta::length("Hello"_s);
    // constexpr auto l5 = meta::length('a', "Hello", "Hello"sv, "Hello"_s);
    // std::cout << l1 << ' ' << l2 << ' ' << l3 << ' ' << l4 << ' ' << l5 << std::endl;

    for  (auto c: s9)
        std::cout << c << ' ';
    std::cout << std::endl;

    for  (auto c: std::ranges::reverse_view(s9))
        std::cout << c << ' ';
    std::cout << std::endl;


    return 0;

}


