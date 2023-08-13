#include <tuple>

#include "var.hpp"
#include "type.hpp"
#include "tuple.hpp"

int main() {
    using namespace meta::literals;
    constexpr auto tuple = meta::tuple(1_i, "hello"_s, 3.14_f);

    tuple.for_each([](auto x) {
        std::cout << x << std::endl;
    });
    
    return 0;
}