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

    constexpr auto s1 = ("Hello"_s + "World");
    constexpr auto s2 = meta::string('H', 'e', 'l', 'l', 'o') + " World";
    constexpr auto s3 = meta::string<5>("Hello"sv);
    constexpr auto s4 = s1.find("World");
    constexpr auto s5 = "Hello"_s.substr<1, 3>();
    constexpr auto s6 = "Hello"_s.substr(1_i, 3_i);
    constexpr auto s7 = "Hello"_s.insert(1_i, "1");
    constexpr auto s8 = s1.substr<s1.find_last_not_of("l")>();


    std::cout << std::boolalpha;
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;
    std::cout << s5 << std::endl;
    std::cout << s6 << std::endl;
    std::cout << s7 << std::endl;
    std::cout << s8 << std::endl;


    // constexpr auto l1 = meta::length('a');
    // constexpr auto l2 = meta::length("Hello");
    // constexpr auto l3 = meta::length("Hello"sv);
    // constexpr auto l4 = meta::length("Hello"_s);
    // constexpr auto l5 = meta::length('a', "Hello", "Hello"sv, "Hello"_s);
    // std::cout << l1 << ' ' << l2 << ' ' << l3 << ' ' << l4 << ' ' << l5 << std::endl;



    // using MyString = meta::static_string<'H', 'e', 'l', 'l', 'o'>;
    // std::cout << MyString::find<'l', 'o'> << std::endl;  // 输出: true

    // using SubstringType = MyString::substr<1, 3>;
    // std::cout << SubstringType{} << std::endl;  // 输出: ell

    // return 0;

    return 0;

}


