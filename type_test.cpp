#include <iostream>
#include "type.hpp"

int main() {
    constexpr auto name = meta::type([](){});
    std::cout << name << std::endl;
}