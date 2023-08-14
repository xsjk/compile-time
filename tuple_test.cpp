#include <tuple>

#include "var.hpp"
#include "type.hpp"
#include "tuple.hpp"
#include "len.hpp"


template<auto I>
constexpr auto f(meta::var<I> i) {
    return i * i;
}


int main() {
    using namespace meta::literals;

    constexpr auto tuple = meta::tuple(1_i, "hello"_s, 3.14_f);

    tuple.for_each([](auto x) {
        std::cout << x << ' ';
    });
    std::cout << std::endl;

    constexpr auto tuple2 = meta::range(10_i).map([](auto x) { return x * x; }).slice(2_i, 5_i);
    std::cout << tuple2 << std::endl;

    return 0;
}