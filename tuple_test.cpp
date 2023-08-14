#include <tuple>

#include "var.hpp"
#include "type.hpp"
#include "tuple.hpp"
#include "len.hpp"

int main() {
    using namespace meta::literals;

    constexpr auto tuple = meta::tuple(1_i, "hello"_s, 3.14_f);

    tuple.for_each([](auto x) { std::cout << x << ' '; });
    std::cout << std::endl;

    constexpr meta::var<[](auto x) { return x * x; }> f;
    constexpr meta::var<[](auto x, auto y) { return x + y; }> g;
    constexpr meta::var<[](auto x) { return x % 2_i; }> h;
    std::cout << meta::range(10_i).map(f).slice(2_i, 6_i).accumulate(g).filter(h) << std::endl;

    // std::cout << meta::tuple(meta::tuple(1_i, 2_i), meta::tuple(3_i, 4_i)) << std::endl;

    return 0;
}