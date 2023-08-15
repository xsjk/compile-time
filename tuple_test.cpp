#include <tuple>

#include "var.hpp"
#include "type.hpp"
#include "tuple.hpp"
#include "utils.hpp"

int main() {
    using namespace meta::literals;

    constexpr auto tuple1 = meta::make_tuple(1_i, "hello"_s, 3.14_f);
    tuple1.for_each([](auto x) { std::cout << x << ' '; });
    std::cout << std::endl;

    constexpr auto f = [](auto x) { return x * x; };
    constexpr auto g = [](auto x, auto y) { return x + y; };
    constexpr auto h = [](auto x) { return x % 2_i == 1_i; };
    std::cout << meta::range(10_i).map(f) << std::endl;
    std::cout << meta::range(10_i).map(f).slice(2_i, 10_i).accumulate(g).filter(h).max() << std::endl;
    std::cout << meta::type(
        meta::make_tuple(
            meta::make_tuple(1_i, 2_i),
            meta::make_tuple(3_i, 4_i)
        ).map([](auto x) { return x.map([](auto y) { return y * y; }); })
    ) << std::endl;

    return 0;
}