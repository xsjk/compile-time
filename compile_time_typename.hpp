#pragma once
#include <string_view>
template <typename T>
constexpr auto _static_typename() {
    constexpr std::string_view pretty_function = __PRETTY_FUNCTION__;
    constexpr auto begin = pretty_function.find_first_not_of(' ', pretty_function.find_first_of('=') + 1);
    constexpr auto end = pretty_function.find_last_not_of(' ', pretty_function.find_last_of(']'));
    return pretty_function.substr(begin, end - begin);
}
#define static_typename(T) _static_typename<decltype(T)>()
