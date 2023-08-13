#pragma once

#include "var.hpp"

namespace meta {


    template <auto ...V>
    struct tuple {
        template<auto ...W>
        requires (sizeof...(V) == sizeof...(W)) && (std::is_same_v<var<V>, var<W>> && ...)
        constexpr tuple(var<W>...) {}
        static constexpr auto value = std::tuple{eval(V)...};
        static consteval auto size() { return sizeof...(V); }
        static consteval auto empty() { return size() == 0; }
        template <auto I>
        consteval auto operator[](var<I> i) const { return var<std::get<I>(value)>{};; }

        template<typename F, std::size_t... I>
        static constexpr void for_each(F&& f, std::index_sequence<I...>) { (f(var<std::get<I>(value)>{}), ...); }
        template<typename F, auto ...W>
        static constexpr void for_each(F&& f, tuple<W...> t) { (f(var<std::get<W>(value)>{}), ...); }
        template<typename F>
        static constexpr void for_each(F&& f) { for_each(std::forward<F>(f), std::make_index_sequence<size()>()); }
        

    };

    template <auto ...V>
    tuple(var<V>...) -> tuple<V...>;

}