#include <tuple>

#include "var.hpp"
#include "type.hpp"
#include "tuple.hpp"
#include "utils.hpp"

int main() {
    using namespace meta::literals;
    
    constexpr auto f = [](auto x) { return x * x; };
    constexpr auto g = [](auto x, auto y) { return x + y; };
    constexpr auto h = [](auto x) { return x % 2_i == 1_i; };
    std::cout << meta::range(10_i) << std::endl;
    std::cout << meta::range(10_i).map(f).slice(2_i, 10_i).accumulate(g).filter(h).append(1_i).sort().sum() << std::endl;

    constexpr auto tuple2 = meta::make_tuple(
        meta::make_tuple(1_i, 2_i),
        meta::make_tuple(3_i, 4_i)
    ).map([](auto x) { return x.map([](auto y) { return y * y; }); });
    std::cout << meta::type(tuple2) << std::endl;
    std::cout << tuple2[1_i][1_i] << std::endl;

    return 0;
}