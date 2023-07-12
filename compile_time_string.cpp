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
    using namespace std::literals;

    constexpr auto s1 = ("Hello"_ss + "World");
    constexpr auto s2 = meta::string('H', 'e', 'l', 'l', 'o') + " World";
    constexpr auto sv = "Hello"sv;
    constexpr auto s3 = meta::string<sv.size()>(sv);
    constexpr auto s4 = ("Hello"_ss + "World").find("el", 1);
    // constexpr auto s5 = meta::string<2>("Hello"_ss, 1);

    // constexpr auto s5 = std::string_view("Hello", 2);
    constexpr std::array<char, 5> arr{'H', 'e', 'l', 'l', 'o'};
    // constexpr auto s5 = std::string_view(
    //     arr.data()
    // );

    constexpr auto s6 = "Hello"_ss.substr<1, 2>();


    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;
    // std::cout << s5 << std::endl;
    std::cout << s6 << std::endl;

    std::cout << std::boolalpha << (s1 != s2) << std::endl;
    std::cout << std::endl;


    // constexpr auto l1 = meta::length('a');
    // constexpr auto l2 = meta::length("Hello");
    // constexpr auto l3 = meta::length("Hello"sv);
    // constexpr auto l4 = meta::length("Hello"_ss);
    // constexpr auto l5 = meta::length('a', "Hello", "Hello"sv, "Hello"_ss);
    // std::cout << l1 << ' ' << l2 << ' ' << l3 << ' ' << l4 << ' ' << l5 << std::endl;



    // using MyString = meta::static_string<'H', 'e', 'l', 'l', 'o'>;
    // std::cout << MyString::find<'l', 'o'> << std::endl;  // 输出: true

    // using SubstringType = MyString::substr<1, 3>;
    // std::cout << SubstringType{} << std::endl;  // 输出: ell

    // return 0;

    return 0;

}


