#include <iostream>
#include "compile_time_typename.hpp"

int main() {
    constexpr auto name = static_typename([](){});
    std::cout << name << std::endl;
}