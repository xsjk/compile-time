#include <iostream>
#include <type_traits>
#include <string>
#include <string_view>
#include <tuple>
#include <array>
#include <ranges>
#include <algorithm>
#include "string.hpp"
#include "type.hpp"



int main() {

    using namespace std::string_view_literals;
    using namespace meta::literals;
    using namespace std::string_literals;

    constexpr auto s1 = "Hello"_s;
    constexpr auto s2 = "World"_s;

    std::cout << std::boolalpha;
    std::cout << s1.size() << s1 << std::endl;
    std::cout << s1[0_i] << std::endl;
    std::cout << s1.substr(1_i, 3_i) << std::endl;
    std::cout << s1 + s2 << std::endl;
    std::cout << 2_i * s1  << std::endl;
    std::cout << s1.compare(s2) << std::endl;
    std::cout << (s1 < s2) << std::endl;
    std::cout << s1.find_first_not_of("ll"_s) << std::endl;
    std::cout << s1.replace("l"_s, "L"_s) << std::endl;
    std::cout << (" "_s + s1 + " "_s).rstrip() + "|"_s << std::endl;

    constexpr auto d = s1.data();
    constexpr auto t = meta::type(d);

    std::cout << t << ' ' << d << std::endl;
    
    for  (auto c: s1)
        std::cout << c << ' ';
    std::cout << std::endl;

    for  (auto c: std::ranges::reverse_view(s1))
        std::cout << c << ' ';
    std::cout << std::endl;

    // meta::var<"123"> v;
    // std::cout << v << std::endl;

    return 0;

}


